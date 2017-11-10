package com.example.ptencheu.bt_bat_36v_demo;

import android.os.Environment;
import android.util.Log;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Calendar;

/**
 * Created by PTENCHEU on 10/9/2017.
 */

public class Raw_Data_Write {

    boolean first_time_to_write = true;

    public int total_data_type = 0;

    //class for better code structure data
    private class Data_type{

        public final String DataName;
        public final String UnitName;
        public String _Data;

        public Data_type(String Data_Name, String Unit_Name, String Data){
            DataName = Data_Name;
            UnitName = Unit_Name;
            _Data = Data;
            total_data_type ++;
        }
    }


    public void write_full_title(){
        if(first_time_to_write == true){
            write_title();
            write_Unit();
            first_time_to_write = false;
        }
    }


    public Data_type time;
    public Data_type real_current;
    public Data_type real_voltage;
    public Data_type target_RPM;
    public Data_type real_RPM;

    public Raw_Data_Write(){//constructor

        time = new Data_type("Time_Stamp","(millisecond)","null"); //0
        real_current = new Data_type("Real_Current","(Ampere)","null"); //1
        real_voltage = new Data_type("Real_Voltage","(Volt)","null"); //2
        real_RPM = new Data_type("Real_RPM","(RPM)","null");//3
        target_RPM = new Data_type("target_RPM","(RPM)","null");//4


        //write_one_set_data(1,1,1,1,1);
    }

    private int tab_spacing = 13;

    public void write_one_set_data(long time_go, float current_go, float voltage_go, int real_RPM_go, int target_RPM_go){

        for(int i = 0; i < total_data_type; i++){
            switch(i){
                case 0: {
                    time._Data = String.valueOf(time_go);
                    write_a_tab_with_Data(time);
                    write_dividing_line();
                    break;
                }
                case 1: {
                    real_current._Data = Float.toString(current_go);
                    write_a_tab_with_Data(real_current);
                    write_dividing_line();
                    break;
                }
                case 2: {
                    real_voltage._Data = Float.toString(voltage_go);
                    write_a_tab_with_Data(real_voltage);
                    write_dividing_line();
                    break;
                }
                case 3: {
                    real_RPM._Data = Integer.toString(real_RPM_go);
                    write_a_tab_with_Data(real_RPM);
                    write_dividing_line();
                    break;
                }
                case 4: {
                    target_RPM._Data = Integer.toString(target_RPM_go);
                    write_a_tab_with_Data(target_RPM);
                    write_dividing_line();
                    break;
                }
            }
        }
        write_enter(1);
    }



    public void write_a_tab_with_DataName(Data_type type){

        int front_spacing;
        int end_spacing;
        front_spacing = (tab_spacing - type.DataName.length())/2;
        end_spacing = tab_spacing - front_spacing - type.DataName.length();
        write_space(front_spacing);
        writeOneString(type.DataName);
        write_space(end_spacing);

    }

    public void write_a_tab_with_DataUnit(Data_type type){

        int front_spacing;
        int end_spacing;
        front_spacing = (tab_spacing - type.UnitName.length())/2;
        end_spacing = tab_spacing - front_spacing - type.UnitName.length();
        write_space(front_spacing);
        writeOneString(type.UnitName);
        write_space(end_spacing);

    }

    public void write_a_tab_with_Data(Data_type type){

        int front_spacing;
        int end_spacing;
        front_spacing = (tab_spacing - type._Data.length())/2;
        end_spacing = tab_spacing - front_spacing - type._Data.length();
        write_space(front_spacing);
        writeOneString(type._Data);
        write_space(end_spacing);

    }


    public void write_title(){

        for(int i = 0; i < total_data_type; i++){

            switch(i){
                case 0: {
                    write_a_tab_with_DataName(time);
                    write_dividing_line();
                    break;
                }
                case 1: {

                    write_a_tab_with_DataName(real_current);
                    write_dividing_line();
                    break;
                }
                case 2: {
                    write_a_tab_with_DataName(real_voltage);
                    write_dividing_line();
                    break;
                }
                case 3: {
                    write_a_tab_with_DataName(real_RPM);
                    write_dividing_line();
                    break;
                }
                case 4: {
                    write_a_tab_with_DataName(target_RPM);
                    write_dividing_line();
                    break;
                }
            }
        }
        write_enter(1);
    }
    public void write_Unit(){

        for(int i = 0; i < total_data_type; i++){

            switch(i){
                case 0: {
                    write_a_tab_with_DataUnit(time);
                    write_dividing_line();
                    break;
                }
                case 1: {

                    write_a_tab_with_DataUnit(real_current);
                    write_dividing_line();
                    break;
                }
                case 2: {
                    write_a_tab_with_DataUnit(real_voltage);
                    write_dividing_line();
                    break;
                }
                case 3: {
                    write_a_tab_with_DataUnit(real_RPM);
                    write_dividing_line();
                    break;
                }
                case 4: {
                    write_a_tab_with_DataUnit(target_RPM);
                    write_dividing_line();
                    break;
                }
            }
        }
        write_enter(1);
    }

    public void write_dividing_line(){
        writeOneString(",");
    }

    public void write_enter(int how_many){
        for(int i = 0; i < how_many; i ++){
            writeOneString("\n");
        }
    }

    public void write_space(int how_many){
        for(int i = 0; i < how_many; i ++){
            writeOneString(" ");
        }
    }


    boolean FLAG_selected_correct_file_name = false;
    File phoneMemoryTextMainDirectory = null;

    private void writeOneString(String data) {
        try {
            //device basic path(phone memory)
            //File path = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DCIM);



            File path = new File((Environment.getExternalStorageDirectory())+"/TTi_Mower_Test_Result"+Global_Variable.bt_name);


            if(path.exists() == false){
                path.mkdir();
            }

            int number = 0;
            String fileName;

            String mydate = java.text.DateFormat.getDateTimeInstance().format(Calendar.getInstance().getTime());

            while(FLAG_selected_correct_file_name == false) {
                fileName = "Mower_Test"+ "_"+mydate +"_"+ number + ".txt";
                phoneMemoryTextMainDirectory = new File(path, fileName);
                if (phoneMemoryTextMainDirectory.exists() == true) {
                    number++;
                }
                else FLAG_selected_correct_file_name = true;
            }
            
            FileOutputStream stream = new FileOutputStream(phoneMemoryTextMainDirectory, true);

            stream.write(data.getBytes());
            stream.flush();
            stream.close();

            //Log.e("Success", "writed");

        }

        catch (IOException e) {
            //Log.e("Exception", "File write failed: " + e.toString());
        }
    }



    private void writeException(Exception e) {
        try {
            //device basic path(phone memory)
            //File path = new File((Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DCIM))+"/TTi_Mower_Test_Result");
            File path = new File((Environment.getExternalStorageDirectory())+"/TTi_Mower_Test_Result"+Global_Variable.bt_name);


            if(path.exists() == false){
                path.mkdir();
            }


            int number = 0;
            String fileName;

            while(FLAG_selected_correct_file_name == false) {
                fileName = "Mower_Test" + number + ".txt";
                phoneMemoryTextMainDirectory = new File(path, fileName);
                if (phoneMemoryTextMainDirectory.exists() == true) {
                    number++;
                }
                else FLAG_selected_correct_file_name = true;
            }

            FileOutputStream stream = new FileOutputStream(phoneMemoryTextMainDirectory, true);

            stream.write(e.getMessage().toString().getBytes());
            stream.flush();
            stream.close();

            Log.e("Success", "writed");

        }

        catch (IOException haha) {
            Log.e("Exception", "File write failed: " + haha.toString());
        }
    }


}
