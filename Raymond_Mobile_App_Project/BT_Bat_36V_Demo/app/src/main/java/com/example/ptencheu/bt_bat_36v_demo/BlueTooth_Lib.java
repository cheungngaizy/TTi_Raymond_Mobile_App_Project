package com.example.ptencheu.bt_bat_36v_demo;

/**
 * Created by PTENCHEU on 8/30/2017.
 */
import android.Manifest;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.SystemClock;
import android.provider.Settings;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.UnsupportedEncodingException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Iterator;
import java.util.List;
import java.util.Set;
import java.util.UUID;

import static android.os.Build.VERSION_CODES.O;


public class BlueTooth_Lib {


    private final String TAG = MainActivity.class.getSimpleName();
    private Handler mHandler; // Our main handler that will receive callback notifications
    private ConnectedThread mConnectedThread; // bluetooth background worker thread to send and receive data
    private BluetoothSocket mBTSocket = null; // bi-directional client-to-client data path
    private BluetoothAdapter mBTAdapter;
    private ArrayAdapter<String> mBTArrayAdapter;
    private Set<BluetoothDevice> mPairedDeviceS;
    private BluetoothDevice mPairedDevice;


    private static final UUID BTMODULEUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB"); // "random" unique identifier

    // #defines for identifying shared types between calling functions
    private final static int REQUEST_ENABLE_BT = 1; // used to identify adding bluetooth names
    private final static int MESSAGE_READ = 2; // used in bluetooth handler to identify message update
    private final static int CONNECTING_STATUS = 3; // used in bluetooth handler to identify message status

    private Activity activity;


    public BlueTooth_Lib(Activity mainActivity){
        activity = mainActivity;
        check_for_permission_and_init();
        dialog_setup();
    }

    public final int Req_Handshake_Command = 0;
    final byte Req_Handshake[] = new byte[]{
            (byte) 0xaa, 0x00, 0x00, 0x03, 0x00, 0x01, 0x10, (byte) 0xec, 0x55
    };

    public final int Req_Bat_Profile_Command = 1;
    final byte Req_Bat_Profile[] = new byte[]{
            (byte) 0xaa, 0x00, 0x00, 0x03, 0x00, (byte)0x2e, 0x10, (byte) 0xbf, 0x55
    };

    public void bt_sent(int REQUEST){

        if(mConnectedThread != null && Connection_Success == true){

            byte input[];

            if(REQUEST == Req_Handshake_Command){
                input = Req_Handshake;
            }
            else if(REQUEST == Req_Bat_Profile_Command){
                input = Req_Bat_Profile;
            }


            else input = null;

            mConnectedThread.write_byte(input);
        }
        else return;
    }

    public boolean is_Connected(){
        return mBTSocket.isConnected();
    }


    int sequential_send = 0;

    public void driver_send_all() {
        if (mConnectedThread != null) {

            if(sequential_send == 2){
                sequential_send = 0;
            }
            else sequential_send ++;

            switch (sequential_send){
                case 0:{mConnectedThread.driver_send_command(0);break;}
                case 1:{mConnectedThread.driver_send_command(1);break;}
                case 2:{mConnectedThread.driver_send_command(2);break;}
            }

            if(Global_Variable.FLAG_first_start_time == false){
                Global_Variable.FLAG_first_start_time = true;
                Global_Variable.starting_time = System.currentTimeMillis();
            }

        }
    }


    public boolean is_handshaked(){
        if(mConnectedThread!=null) {
            return mConnectedThread.FLAG_HandShake_Success;
        }
        else return false;
    };

    public int Get_Data_Temperature(){
        if(mConnectedThread != null){
            return mConnectedThread.DATA_temperature;
        }
        else return 0;
    }


    public float Get_Data_Voltage(){
        if(mConnectedThread != null){
            return mConnectedThread.DATA_Voltage;
        }
        else return 0;
    }

    public float Get_Data_Current(){
        if(mConnectedThread != null){
            return mConnectedThread.DATA_current;
        }
        else return 0;
    }

    public float Get_Data_SOC(){
        if(mConnectedThread != null){
            return mConnectedThread.DATA_SOC;
        }
        else return 0;
    }


    private void check_for_permission_and_init(){

        //getting adapter and make sure bt is on
        mBTAdapter = BluetoothAdapter.getDefaultAdapter();
        if(!mBTAdapter.isEnabled()){
            try{
                mBTAdapter.enable();
                while(!mBTAdapter.isEnabled());//make it die
                Toast.makeText(activity,"BlueTooth Enabled",Toast.LENGTH_SHORT).show();
            }catch (Exception e){}
        }

        //message handler
        mHandler = new Handler(){
            public void handleMessage(android.os.Message msg){

                if(msg.what == MESSAGE_READ){
                    String readMessage = null;
                    try {
                        readMessage = new String((byte[]) msg.obj, "UTF-8");
                    } catch (UnsupportedEncodingException e) {
                        e.printStackTrace();
                    }
                    //Toast.makeText(activity,"READ"+readMessage,Toast.LENGTH_SHORT).show();
                   // Toast.makeText(activity,mConnectedThread.pending_msg.toString(),Toast.LENGTH_SHORT).show();
                   // Toast.makeText(activity, Arrays.toString(mConnectedThread.pending_msg),Toast.LENGTH_SHORT).show();


                    //mReadBuffer.setText(readMessage);
                }

                if(msg.what == CONNECTING_STATUS){
                    if(msg.arg1 == 1){
                        Toast.makeText(activity,"Connected to Device: " + (String)(msg.obj),Toast.LENGTH_SHORT).show();


                    }
                    else {
                        Toast.makeText(activity,"Reconnecting",Toast.LENGTH_SHORT).show();
                        mConnectedThread.cancel();
                        mConnectedThread = new ConnectedThread(mBTSocket);
                        mConnectedThread.start();
                    }
                }
            }
        };

        //make sure there is bluetooth in your phone
        if(mBTAdapter == null){
            Toast.makeText(activity,"no BT in device",Toast.LENGTH_SHORT).show();
        }
        else{

        }
    }

    List<String> ListStr;
    int btIndex;
    String address;
    private void dialog_setup(){

        mPairedDevice = null;
        ListStr = new ArrayList<>();
        mPairedDeviceS = mBTAdapter.getBondedDevices(); // it is a set

        // make a list of all the Bluetooth devices that you can connect to
        for(Iterator<BluetoothDevice> it = mPairedDeviceS.iterator(); it.hasNext();)
        {
            BluetoothDevice b = it.next();
            ListStr.add(b.getName());
        }
        String [] str = new String[ListStr.size()];
        for(int i = 0; i < ListStr.size(); i++)
        {
            str[i] = ListStr.get(i);
        }

        AlertDialog.Builder AlertDialogConn = new AlertDialog.Builder(activity);
        DialogInterface.OnClickListener ListClick = new DialogInterface.OnClickListener()
        {
            public void onClick(DialogInterface dialog, int which)
            {
                btIndex = which;

                for(Iterator<BluetoothDevice> it = mPairedDeviceS.iterator(); it.hasNext();)
                {
                    BluetoothDevice b = it.next();
                    if(b.getName().equals(ListStr.get(btIndex)))
                    {
                        address = b.getAddress();
                        mPairedDevice = mBTAdapter.getRemoteDevice(address);
                        try
                        {
                            Start_new_thread();
                        }
                        catch (Exception e)
                        {
                            // TODO Auto-generated catch block
                            //connected = false;
                            Toast.makeText(activity, "Failed", Toast.LENGTH_SHORT).show();
                        }
                    }
                }
            }
        };
        AlertDialogConn.setItems(str, ListClick);
        AlertDialogConn.show();

    }


    private void bluetoothOff(View view){
        mBTAdapter.disable(); // turn off
        Toast.makeText(activity,"Bluetooth turned Off", Toast.LENGTH_SHORT).show();
    }

//    private void discover(View view){
//        // Check if the device is already discovering
//        if(mBTAdapter.isDiscovering()){
//            mBTAdapter.cancelDiscovery();
//            Toast.makeText(activity,"Discovery stopped",Toast.LENGTH_SHORT).show();
//        }
//        else{
//            if(mBTAdapter.isEnabled()) {
//                mBTArrayAdapter.clear(); // clear items
//                mBTAdapter.startDiscovery();
//                Toast.makeText(activity, "Discovery started", Toast.LENGTH_SHORT).show();
//                activity.registerReceiver(blReceiver, new IntentFilter(BluetoothDevice.ACTION_FOUND));
//            }
//            else{
//                Toast.makeText(activity, "Bluetooth not on", Toast.LENGTH_SHORT).show();
//            }
//        }
//    }

//    final BroadcastReceiver blReceiver = new BroadcastReceiver() {
//        @Override
//        public void onReceive(Context context, Intent intent) {
//            String action = intent.getAction();
//            if(BluetoothDevice.ACTION_FOUND.equals(action)){
//                BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
//                // add the name to the list
//                //mBTArrayAdapter.add(device.getName() + "\n" + device.getAddress());
//                //mBTArrayAdapter.notifyDataSetChanged();
//            }
//        }
//    };
//
//    private void listPairedDevices(View view){
//        mPairedDevices = mBTAdapter.getBondedDevices();
//        if(mBTAdapter.isEnabled()) {
//            // put it's one to the adapter
//            for (BluetoothDevice device : mPairedDevices)
//                mBTArrayAdapter.add(device.getName() + "\n" + device.getAddress());
//
//            Toast.makeText(activity, "Show Paired Devices", Toast.LENGTH_SHORT).show();
//        }
//        else
//            Toast.makeText(activity, "Bluetooth not on", Toast.LENGTH_SHORT).show();
//    }


    private void Start_new_thread(){

// Spawn a new thread to avoid blocking the GUI one
        new Thread()
        {
            public void run() {

                boolean fail = false;

                BluetoothDevice device = mBTAdapter.getRemoteDevice(address);

                try {
                    mBTSocket = createBluetoothSocket(device);
                } catch (IOException e) {
                    fail = true;
                    Toast.makeText(activity, "Socket creation failed", Toast.LENGTH_SHORT).show();
                }
                // Establish the Bluetooth socket connection.
                try {
                    mBTSocket.connect();
                } catch (IOException e) {
                    try {
                        fail = true;
                        mBTSocket.close();
                        mHandler.obtainMessage(CONNECTING_STATUS, -1, -1).sendToTarget();
                    } catch (IOException e2) {
                        //insert code to deal with this
                        Toast.makeText(activity, "Socket creation failed", Toast.LENGTH_SHORT).show();
                    }
                }
                if(fail == false) {//connection build
                    Connection_Success = true;
                    mHandler.obtainMessage(CONNECTING_STATUS, 1, -1,device.getName()).sendToTarget();
                    mConnectedThread = new ConnectedThread(mBTSocket);
                    mConnectedThread.start();
                }
            }
        }.start();

    }

    boolean Connection_Success = false;

    public byte[] getPendingMsg(){
        if(mConnectedThread != null) {
            byte[] hihi = Req_Handshake;
            return hihi;
        }
        else{
            byte[] hi = new byte[1];
            return hi;
        }


    }

    public boolean get_connection_status(){
        //return mConnectedThread.check_connection_status();
        return false;
    }



    private BluetoothSocket createBluetoothSocket(BluetoothDevice device) throws IOException {
        try {
            final Method m = device.getClass().getMethod("createInsecureRfcommSocketToServiceRecord", UUID.class);
            Global_Variable.bt_name = device.getName();
            return (BluetoothSocket) m.invoke(device, BTMODULEUUID);
        } catch (Exception e) {
            Log.e(TAG, "Could not create Insecure RFComm Connection",e);
        }
        return  device.createRfcommSocketToServiceRecord(BTMODULEUUID);
    }

    //Thread that control the Input Output
    private class ConnectedThread extends Thread {
        private final BluetoothSocket mmSocket;
        private final InputStream mmInStream;
        private final OutputStream mmOutStream;

        public ConnectedThread(BluetoothSocket socket) {
            mmSocket = socket;
            InputStream tmpIn = null;
            OutputStream tmpOut = null;

            // Get the input and output streams, using temp objects because
            // member streams are final
            try {
                tmpIn = socket.getInputStream();
                tmpOut = socket.getOutputStream();
            } catch (IOException e) { }
            mmInStream = tmpIn;
            mmOutStream = tmpOut;
        }

        public boolean check_connection_status(){

            //return mmSocket.isConnected();
            return false;
        }


        byte[] buffer;  // buffer store for the stream
        int delay_counter = 0;

        public void run() {
            int bytes; // bytes returned from read()
            // Keep listening to the InputStream until an exception occurs
            while (true) {
                try {
                    // Read from the InputStream
                    bytes = mmInStream.available();// how many bytes are ready to be read?
                    if(delay_counter < 100006) {
                        delay_counter++;
                       //mmInStream.read();
                    }
                    if(bytes != 0 && delay_counter > 100000 && bytes < 30000){//there is some incoming data
                        try {
                            buffer = new byte[bytes+10000];
                            bytes = mmInStream.read(buffer, 0, mmInStream.available());//the actual byte that we get in total
                            analyst_data(buffer, bytes);
                        }
                        catch(Exception e){
                            Log.e("run",e.toString());
                        }//mHandler.obtainMessage(MESSAGE_READ, bytes, -1, buffer).sendToTarget(); // Send the obtained bytes to the UI activity
                    }

                } catch (IOException e) {
                    Log.e("run",e.toString());
                    break;
                }
            }
        }

        byte []pending_data = new byte[50000];
        public byte []pending_msg = new byte [300];
        int pending_data_count = 0;

        private void analyst_data(byte []input, int length){

            if(pending_data_count > 10000 || length > 7000 || (10000-pending_data_count)<length){
                pending_data_count = 0;
                for(int j = 0; j< 10000; j++){
                    pending_data[j] = 0;
                }
                Log.e("ClearAll0","length too long");
            }
            else {

                //copying data for analyst
                for (int i = 0; i < length; i++) {
                    pending_data[pending_data_count] = input[i];
                    pending_data_count++;
                }

                //start analyst
                int position_start = 50000;
                int position_end = -50000;

                //this is for the driver
                boolean FLAG_find_start = false;
                for (int j = 0; j < pending_data_count - 1; j++) {//check head
                    if (pending_data[j] == (byte) 0x55 && position_start == 50000) {
                        position_start = j;
                        FLAG_find_start = true;
                    }
                    if(FLAG_find_start == true){
                        if(pending_data[position_start+4] == (byte)0xaa){
                            //return;
                        }
                        else{
                            position_start = 50000;
                            FLAG_find_start = false;
                        }
                    }
                }


//                for (int k = position_start; k < pending_data_count - 1; k++) {//check tail
//                    if (pending_data[k] == (byte) 0xaa && position_end == -50000) {
//                        position_end = k;
//                    }
//                }
                if(position_start < 10000) {

                    try {
                        if (pending_data[position_start + 4] == (byte) 0xaa) {

                            int pending_msg_count = 0;

                            pending_msg = new byte[5];

                            //outlining the msg correctly
                            for (int k = position_start; k < position_start + 4; k++) {//putting the correct respond to a msg
                                pending_msg[pending_msg_count] = pending_data[k];
                                pending_msg_count++;
                            }

                            //do the data get stuff
                            if (pending_msg.length == 5) {
                                check_what_is_it_for_driver(pending_msg);
                            }

                            pending_data_count = 0;
                            for (int w = 0; w < 10000; w++) {
                                pending_data[w] = 0;
                            }

//                //clear the data stored and adjust the position
//                for(int w = 0 ; w < position_end; w++){
//                    pending_data[w] = pending_data[w+position_end+1];
//                }
//                pending_data_count = pending_data_count - position_end - 1 ;
                        }
                        else {
                            Log.e("Not in right 55aa","set to 0");
                            pending_data[position_start] = 0;
                        }

                    }
                    catch (Exception e) {
                        Log.e("analystData", e.toString());
                    }
                }

                else{//too much data clear all

                    if(pending_data_count>1000) {
                        Log.e("Clear allll","impossible");
                        pending_data_count = 0;
                        for (int w = 0; w < 10000; w++) {
                            pending_data[w] = 0;
                        }
                    }

                }
                //this is for battery
//            for(int j = 0; j < pending_data_count -1; j++){//check head
//                if(pending_data[j] == (byte)0xaa && position_start == 100){
//                    position_start = j;
//                }
//            }
//
//            for(int k = position_start; k < pending_data_count -1; k++){//check tail
//                if(pending_data[k] == (byte)0x55 && position_end == -100){
//                    position_end = k;
//                }
//            }
//
//            if(position_end > position_start) {
//
//                int pending_msg_count = 0;
//
//                pending_msg = new byte[position_end - position_start  + 1 ];
//
//                //outlining the msg correctly
//                for (int k = position_start; k < position_end+1; k++) {//putting the correct respond to a msg
//                    pending_msg[pending_msg_count] = pending_data[k];
//                    pending_msg_count++;
//                }
//
//
//                //do the data get stuff
//                if(pending_msg.length == 9 + pending_msg[1]) { // checking the length whether it is correct
//                    check_what_is_it(pending_msg);
//                }
//                //clear the data stored and adjust the position
//                for(int w = 0 ; w < position_end; w++){
//                    pending_data[w] = pending_data[w+position_end+1];
//                }
//                pending_data_count = pending_data_count - position_end - 1 ;
//            }
            }
        }


        int acceleration_target_speed = 2000;

        void acceleration_function(int target){

            if(Math.abs(target - acceleration_target_speed)>80) {
                if (target > acceleration_target_speed) {
                    acceleration_target_speed += 80;
                }
                else if (target < acceleration_target_speed) {
                    acceleration_target_speed -= 80;
                }
            }
        }


        public void driver_send_command(int ID){

            byte Ready_to_send[] = new byte[]{
                0x55, (byte) 0xff,0x00,0x00, (byte) 0xaa
            };

            switch (ID){
                case 0:{//target rpm
                    if(Global_Variable.Target_Speed > Global_Variable.Max_RPM){
                        Global_Variable.Target_Speed = Global_Variable.Max_RPM;
                    }

                    acceleration_function(Global_Variable.Target_Speed);

                    Ready_to_send[1] = 0x00;
                    Ready_to_send[2] = (byte) (acceleration_target_speed >> 8);
                    Ready_to_send[3] = (byte) (acceleration_target_speed);
                    break;
                }
                case 1:{//target Ampere
                    if(Global_Variable.Target_Ampere > 60000){
                        Global_Variable.Target_Ampere = 60000;
                    }
                    else if(Global_Variable.Target_Ampere < 0){
                        Global_Variable.Target_Ampere =0;
                    }
                    Ready_to_send[1] = 0x01;
                    Ready_to_send[2] = (byte) (Global_Variable.Target_Ampere >> 8);
                    Ready_to_send[3] = (byte) (Global_Variable.Target_Ampere);
                    break;
                }
                case 2:{

                    Ready_to_send[1] = 0x02;
                    if(Global_Variable.Target_Power_Limitation_On_Off == true){
                        Ready_to_send[2] = 0x11;
                        Ready_to_send[3] = 0x11;
                    }
                    else if(Global_Variable.Target_Power_Limitation_On_Off == false){
                        Ready_to_send[2] = 0x00;
                        Ready_to_send[3] = 0x00;
                    }
                    break;
                }
            }


            mConnectedThread.write_byte(Ready_to_send);

        }


        long last_record_current_time = 0;
        boolean record_first_time = false;

        public void check_what_is_it_for_driver(byte[] pending_Msg){


            switch (pending_Msg[1]){

                case 0x03:{//current
                    Global_Variable.Current = (float)(((pending_Msg[3]&0xff) | ((pending_Msg[2]&0xff)<<8))&0xffff)/1000;

                    if(record_first_time == false){last_record_current_time = System.currentTimeMillis();record_first_time = true;}
                    else if(Global_Variable.Current > 0 && (System.currentTimeMillis()-last_record_current_time)>0 && Global_Variable.Voltage > 0){
                        float Joule = Global_Variable.Current * Global_Variable.Voltage *  (((float)(System.currentTimeMillis()-last_record_current_time))/1000);

                        Global_Variable.Ampere_Hour = Global_Variable.Ampere_Hour + Joule/(3600*Global_Variable.Voltage);

                        last_record_current_time = System.currentTimeMillis();
                    }

                        //further data analyst
                    if(Global_Variable.Current>=0 && Global_Variable.Current < 5){
                        Global_Variable.counter_0_5++;
                    }
                    else if(Global_Variable.Current>=5 && Global_Variable.Current < 10){
                        Global_Variable.counter_5_10++;
                    }
                    else if(Global_Variable.Current>=10 && Global_Variable.Current < 15){
                        Global_Variable.counter_10_15++;
                    }
                    else if(Global_Variable.Current>=15 && Global_Variable.Current < 20){
                        Global_Variable.counter_15_20++;
                    }
                    else if(Global_Variable.Current>=20 && Global_Variable.Current < 25){
                        Global_Variable.counter_20_25++;
                    }
                    else if(Global_Variable.Current>=25 && Global_Variable.Current < 30){
                        Global_Variable.counter_25_30++;
                    }
                    else if(Global_Variable.Current>=30 && Global_Variable.Current < 35){
                        Global_Variable.counter_30_35++;
                    }
                    else if(Global_Variable.Current>=35 && Global_Variable.Current < 40){
                        Global_Variable.counter_35_40++;
                    }
                    else if(Global_Variable.Current>=40 && Global_Variable.Current < 45){
                        Global_Variable.counter_40_45++;
                    }
                    else if(Global_Variable.Current>=45 && Global_Variable.Current < 50){
                        Global_Variable.counter_45_50++;
                    }
                    else if(Global_Variable.Current>=50 && Global_Variable.Current < 55){
                        Global_Variable.counter_50_55++;
                    }
                    else if(Global_Variable.Current>=55){
                        Global_Variable.counter_55_60++;
                    }
                    else{}


                    //control logic by Current Value
                        //Data Analyst
                        if(Global_Variable.Current >= 0 && Global_Variable.Current < 10){
                            Global_Variable.set0_10_counter++;
                            Global_Variable.Target_Speed = Global_Variable.target_speed_0_10;
                        }
                        else if(Global_Variable.Current >= 10 && Global_Variable.Current < 20){
                            Global_Variable.set11_20_counter++;
                            Global_Variable.Target_Speed = Global_Variable.target_speed_11_20;
                        }
                        else if(Global_Variable.Current >= 20 && Global_Variable.Current < 30){
                            Global_Variable.set21_30_counter++;
                            Global_Variable.Target_Speed = Global_Variable.target_speed_21_30;
                        }
                        else if(Global_Variable.Current >= 30 && Global_Variable.Current < 40){
                            Global_Variable.set31_40_counter++;
                            Global_Variable.Target_Speed = Global_Variable.target_speed_31_40;
                        }
                        else if(Global_Variable.Current >= 40 && Global_Variable.Current <50){
                            Global_Variable.set41_50_counter++;
                            Global_Variable.Target_Speed = Global_Variable.target_speed_41_50;
                        }
                        else if(Global_Variable.Current >= 50){
                            Global_Variable.set51_60_counter++;
                            Global_Variable.Target_Speed = Global_Variable.target_speed_51_60;
                        }

                    break;
                }
                case 0x04:{//voltage

                    Global_Variable.Voltage = (float)(((pending_Msg[3]&0xff) | ((pending_Msg[2]&0xff)<<8))&0xffff)/1000;


                    break;
                }
                case 0x05:{//rpm
                    Global_Variable.Real_RPM = (((pending_Msg[3]&0xff) | ((pending_Msg[2]&0xff)<<8))&0xffff);
                    if(Global_Variable.Real_RPM > Global_Variable.Max_RPM){
                        Global_Variable.Real_RPM = Global_Variable.Max_RPM;
                    }
                    else if (Global_Variable.Real_RPM < 0){
                        Global_Variable.Real_RPM = 0;
                    }

                    break;
                }

            }




        }


        final byte [] HandShake_Success = new byte[]{
                (byte)0xaa,0x01,0x00,0x00,0x03,0x01,0x20,0x00,(byte)0xdb,0x55
        };

        public boolean FLAG_HandShake_Success = false;
        public float DATA_current = 0;
        public int DATA_temperature = 0;
        public float DATA_SOC = 0;//in percentage(/10)
        public float DATA_Voltage = 0;

        public void check_what_is_it(byte[] pendingMsg){

            if(Arrays.equals(pendingMsg,HandShake_Success)){
                FLAG_HandShake_Success = true;
            }else{//normal function
                if(pendingMsg[5] == 0x2e && pendingMsg[6] == 0x20){ //battery profile

                    //sign to unsign
                    for(int i = 7; i < 14;i++){
                        if(pendingMsg[i] < 0){
                            pendingMsg[i]+=128;
                            //pendingMsg[i] = pendingMsg[i] + (byte)128;
                        }
                    }




                    //data decoding for battery
                    DATA_temperature = (pendingMsg[7]|(pendingMsg[8]<<8))-300;
                    DATA_SOC = ((float)(pendingMsg[9]|((pendingMsg[10]<<8))))/10;
                    DATA_current = -((float)(((int)pendingMsg[11]|(int)pendingMsg[12]<<8)-20000)/10);



                    Global_Variable.Current = DATA_current;

                    if(DATA_current > 0.1 && DATA_current < 10){
                        Global_Variable.set0_10_counter++;
                    }
                    else if(DATA_current >= 10 && DATA_current < 20){
                        Global_Variable.set11_20_counter++;
                    }
                    else if(DATA_current >= 20 && DATA_current < 30){
                        Global_Variable.set21_30_counter++;
                    }
                    else if(DATA_current >= 30 && DATA_current < 40){
                        Global_Variable.set31_40_counter++;
                    }
                    else if(DATA_current >= 40 && DATA_current <50){
                        Global_Variable.set41_50_counter++;
                    }
                    else if(DATA_current >= 50 && DATA_current <60){
                        Global_Variable.set51_60_counter++;
                    }



                    DATA_Voltage = (float)(pendingMsg[13]|(pendingMsg[14]<<8))/10;
                    Global_Variable.Voltage = DATA_Voltage;
                    //state analyst

                }

            }

        }

        /* Call this from the main activity to send data to the remote device */
        public void write(String input) {
            byte[] bytes = input.getBytes();           //converts entered String into bytes
            try {
                mmOutStream.write(bytes);
            } catch (IOException e) { }
        }

        public void write_byte(byte[] input){
            try{
                mmOutStream.write(input);
            }catch (IOException e){}

        }

        /* Call this from the main activity to shutdown the connection */
        public void cancel() {
            try {
                mmSocket.close();
            } catch (IOException e) { }
        }
    }


}
