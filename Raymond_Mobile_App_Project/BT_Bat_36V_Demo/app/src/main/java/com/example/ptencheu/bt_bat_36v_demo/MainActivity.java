package com.example.ptencheu.bt_bat_36v_demo;

import android.Manifest;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.LightingColorFilter;
import android.graphics.Paint;
import android.graphics.PorterDuff;
import android.graphics.Typeface;
import android.media.Image;
import android.os.Handler;
import android.provider.Settings;
import android.support.v4.app.ActivityCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;
import android.view.animation.AlphaAnimation;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;

import java.util.Random;
import static com.example.ptencheu.bt_bat_36v_demo.R.*;

public class MainActivity extends AppCompatActivity implements View.OnClickListener, View.OnTouchListener , SeekBar.OnSeekBarChangeListener {

    TextView mDebugText;
    TextView mTextView_RPM;
    Handler handler;
    Handler handler_draw;
    Handler handler_battery_update;
    Raw_Data_Write txt_write;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(layout.activity_main);
        this.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);

        initialization();

        handler = new Handler();
        handler.post(runnable);

        handler_draw = new Handler();
        handler_draw.post(runnable_draw);

        handler_battery_update = new Handler();
        handler_battery_update.post(runnable_for_slow);

        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);


        ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE}, 1);
        ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.READ_EXTERNAL_STORAGE}, 1);
        txt_write = new Raw_Data_Write();
    }

    Button mButton_PRO;
    Button mButton_BT;
    Button mButton_Result;
    Button mButton_0;
    Button mButton_1;
    Button mButton_2;
    Button mButton_3;
    Button mButton_4;
    Button mButton_5;
    Button mButton_0000;
    Button mButton_1000;
    Button mButton_2000;
    Button mButton_3000;
    Button mButton_4000;
    Button mButton_500;
    Button mButton_1500;
    Button mButton_2500;
    Button mButton_3500;

    Button mButton_Mode_Power;
    Button mButton_Mode_Eco;


    Button mButton_Real_Pro;

    Intent mIntentPRO;
    Intent mIntentResult;
    Intent mIntentRealPRO;

    mDrawableLib mDrawing;
    View mView_Drawable;
    //View mView_Drawable_adjust;
    Canvas canvas;
    //Canvas canvas_adjust;

    ImageView Current_IMAGE;

    SeekBar mSeekBar_MAX_A;

    //init UI function
    public void initialization() {

        //TextView
        mDebugText = (TextView)findViewById(id.mText_Debug);
        mDebugText.setText("begin");
        mTextView_RPM = (TextView)findViewById(id.textView_RPM);
        Typeface type = Typeface.createFromAsset(getAssets(),"fonts/SFDigitalReadout-Medium.ttf");
        mTextView_RPM.setTypeface(type);
        mTextView_RPM.setTextSize(50);
        mTextView_RPM.setTextColor(Color.WHITE);

        //intent
        //mIntentPRO = new Intent(this,mLineChartAct.class);
        mIntentPRO = new Intent(MainActivity.this,mLineChartAct.class);
        mIntentResult = new Intent(MainActivity.this,mBarChartAct.class);
        mIntentRealPRO = new Intent(MainActivity.this,mLineChartAct.class);

        //button
//        mButton_BT = (Button) findViewById(id.button_BT);
//        mButton_BT.setOnClickListener(this);
//        mButton_BT.setOnTouchListener(this);

        mButton_PRO = (Button) findViewById(id.button_PRO);
        mButton_PRO.setOnClickListener(this);
        mButton_PRO.setOnTouchListener(this);

        mButton_Result = (Button) findViewById(id.button_Result);
        mButton_Result.setOnClickListener(this);
        mButton_Result.setOnTouchListener(this);

//        mButton_Real_Pro = (Button) findViewById(id.button_Real_PRO);
//        mButton_Real_Pro.setOnClickListener(this);

        mButton_Mode_Power = (Button) findViewById(id.button_power);
        mButton_Mode_Power.setOnClickListener(this);
        mButton_Mode_Power.setVisibility(View.INVISIBLE);
        mButton_Mode_Eco = (Button) findViewById(id.button_eco);
        mButton_Mode_Eco.setOnClickListener(this);
        mButton_Mode_Eco.setVisibility(View.INVISIBLE);

        mButton_0 = (Button)findViewById(id.button_0_10);
        mButton_0.setOnClickListener(this);
        mButton_1 = (Button)findViewById(id.button_11_20);
        mButton_1.setOnClickListener(this);
        mButton_2 = (Button)findViewById(id.button_21_30);
        mButton_2.setOnClickListener(this);
        mButton_3 = (Button)findViewById(id.button_31_40);
        mButton_3.setOnClickListener(this);
        mButton_4 = (Button)findViewById(id.button_41_50);
        mButton_4.setOnClickListener(this);
        mButton_5 = (Button)findViewById(id.button_51_60);
        mButton_5.setOnClickListener(this);

        mButton_0000 = (Button)findViewById(R.id.button_0000);
        mButton_0000.setOnClickListener(this);
        mButton_1000 = (Button)findViewById(R.id.button_1000);
        mButton_1000.setOnClickListener(this);
        mButton_2000 = (Button)findViewById(R.id.button_2000);
        mButton_2000.setOnClickListener(this);
        mButton_3000 = (Button)findViewById(R.id.button_3000);
        mButton_3000.setOnClickListener(this);
        mButton_4000 = (Button)findViewById(R.id.button_4000);
        mButton_4000.setOnClickListener(this);
        mButton_500 = (Button)findViewById(R.id.button_500);
        mButton_500.setOnClickListener(this);
        mButton_1500 = (Button)findViewById(R.id.button_1500);
        mButton_1500.setOnClickListener(this);
        mButton_2500 = (Button)findViewById(R.id.button_2500);
        mButton_2500.setOnClickListener(this);
        mButton_3500 = (Button)findViewById(R.id.button_3500);
        mButton_3500.setOnClickListener(this);

        //seekbar
        mSeekBar_MAX_A = (SeekBar) findViewById(id.seekBar_MAXA);
        mSeekBar_MAX_A.setOnSeekBarChangeListener(this);
        mSeekBar_MAX_A.setProgress(Global_Variable.Target_Ampere/1000);

        //bt
        bt = new BlueTooth_Lib(this);

        //Drawable

        mView_Drawable =  new mDrawableLib(getApplicationContext());

        Bitmap bitmap = Bitmap.createBitmap(2392,1440, Bitmap.Config.ARGB_8888);

        ImageView iv = (ImageView) findViewById(id.iv);
        iv.setImageBitmap(bitmap);

        canvas = new Canvas(bitmap);


//        mView_Drawable_adjust = new mDrawableLib(getApplicationContext());
//        Bitmap bitmap1 = Bitmap.createBitmap(2392,1440, Bitmap.Config.ARGB_8888);
//
//        ImageView iv1 = (ImageView) findViewById(id.imageView);
//        iv1.setImageBitmap(bitmap);
//
//        canvas_adjust = new Canvas(bitmap1);
//        mView_Drawable_adjust.draw(canvas_adjust);

        mView_Drawable.draw(canvas);

        color_init();

        //Current_image
        Current_IMAGE = (ImageView) findViewById(id.Current_Image);
        Battery_IMAGE = (ImageView) findViewById(id.Battery_Level_Image);
    }

    ImageView Battery_IMAGE;

    Paint Gauge_Paint;
    Paint Adjust_RPM_Paint;

    void color_init(){
        Gauge_Paint = new Paint();
        Gauge_Paint.setColor(getResources().getColor(color.color_Gauge_indicator));
        Gauge_Paint.setStyle(Paint.Style.STROKE);
        Gauge_Paint.setStrokeWidth(15);

        Adjust_RPM_Paint = new Paint();
        Adjust_RPM_Paint.setColor(getResources().getColor(color.color_Adjust_RPM));
        Adjust_RPM_Paint.setStyle(Paint.Style.STROKE);
        Adjust_RPM_Paint.setStrokeWidth(15);

    }

    private AlphaAnimation buttonClick = new AlphaAnimation(1F,0.1F);
    boolean FLAG_run_set = false; //false is running, true is setting
    int FLAG_RPM_Change_which = 10;

    public void onClick(View v) {

        //v.startAnimation(buttonClick);
        switch (v.getId()) {

//            case id.button_BT: {
//                //boolean wtf = bt.get_connection_status();
//                //
//                // Toast.makeText(this,"hi:" + wtf,Toast.LENGTH_SHORT);
//                break;
//            }

            case id.button_PRO: {
                mIntentPRO.putExtra("F", 10);
                //startActivity(mIntentPRO);

                if (FLAG_run_set == false) {
                    mButton_PRO.setBackgroundResource(drawable.run_);
                    FLAG_run_set = true;
                    Toast.makeText(this, "select a Speed", Toast.LENGTH_SHORT).show();
                    mButton_Mode_Eco.setVisibility(View.VISIBLE);
                    mButton_Mode_Power.setVisibility(View.VISIBLE);
                } else if (FLAG_run_set == true) {
                    mButton_PRO.setBackgroundResource(drawable.set);
                    //RPM_selection_button_Trans(FLAG_RPM_Change_which);
                    FLAG_run_set = false;
                    FLAG_RPM_Change_which = 10;
                    mButton_Mode_Power.setVisibility(View.INVISIBLE);
                    mButton_Mode_Eco.setVisibility(View.INVISIBLE);
                }

                break;
            }

            case id.button_eco:{
                Global_Variable.Adjust_RPM = 2500;
                Global_Variable.target_speed_0_10 = 2500;
                Global_Variable.Adjust_RPM = 2800;
                Global_Variable.target_speed_11_20 = 2800;
                Global_Variable.target_speed_21_30 = 2800;
                Global_Variable.target_speed_31_40 = 2800;
                Global_Variable.target_speed_41_50 = 2800;
                Global_Variable.target_speed_51_60 = 2800;
                Toast.makeText(this,"Set to Eco Mode",Toast.LENGTH_SHORT).show();

                break;
            }

            case id.button_power:{

                Global_Variable.Adjust_RPM = 3200;
                Global_Variable.target_speed_0_10 = 3200;
                Global_Variable.target_speed_11_20 = 3200;
                Global_Variable.target_speed_21_30 = 3200;
                Global_Variable.target_speed_31_40 = 3200;
                Global_Variable.target_speed_41_50 = 3200;
                Global_Variable.target_speed_51_60 = 3200;
                Toast.makeText(this,"Set to Power Mode",Toast.LENGTH_SHORT).show();
                break;
            }

            case id.button_Result: {
                if(Global_Variable.FLAG_first_start_time == true) {
                    Global_Variable.cutting_time = System.currentTimeMillis() - Global_Variable.starting_time;
                }
                else Global_Variable.cutting_time = 0;
                startActivity(mIntentResult);
                break;
            }

//            case id.button_Real_PRO:{
//                startActivity(mIntentRealPRO);
//                break;
//            }
        }


        if(FLAG_run_set == true) {
            switch (v.getId()) {

                case id.button_0000:{Global_Variable.Adjust_RPM = 0;break;}
                case id.button_1000:{Global_Variable.Adjust_RPM = 1000;break;}
                case id.button_2000:{Global_Variable.Adjust_RPM = 2000;break;}
                case id.button_3000:{Global_Variable.Adjust_RPM = 3000;break;}
                case id.button_4000:{Global_Variable.Adjust_RPM = 4000;break;}
                case id.button_500:{Global_Variable.Adjust_RPM = 500;break;}
                case id.button_1500:{Global_Variable.Adjust_RPM = 1500;break;}
                case id.button_2500:{Global_Variable.Adjust_RPM = 2500;break;}
                case id.button_3500:{Global_Variable.Adjust_RPM = 3500;break;}


                case id.button_0_10: {
                    //RPM_selection_button_Trans(FLAG_RPM_Change_which);
                    FLAG_RPM_Change_which = 0;
                    Global_Variable.Adjust_RPM = Global_Variable.target_speed_0_10;
                    //RPM_selection_button_NONTrans(FLAG_RPM_Change_which);
                    break;
                }

                case id.button_11_20: {
                    //RPM_selection_button_Trans(FLAG_RPM_Change_which);
                    FLAG_RPM_Change_which = 1;
                    //RPM_selection_button_NONTrans(FLAG_RPM_Change_which);
                    Global_Variable.Adjust_RPM = Global_Variable.target_speed_11_20;
                    break;
                }
                case id.button_21_30: {
                    //RPM_selection_button_Trans(FLAG_RPM_Change_which);
                    FLAG_RPM_Change_which = 2;
                    //RPM_selection_button_NONTrans(FLAG_RPM_Change_which);
                    Global_Variable.Adjust_RPM = Global_Variable.target_speed_21_30;

                    break;
                }
                case id.button_31_40: {
                    //RPM_selection_button_Trans(FLAG_RPM_Change_which);
                    FLAG_RPM_Change_which = 3;
                    //RPM_selection_button_NONTrans(FLAG_RPM_Change_which);
                    Global_Variable.Adjust_RPM = Global_Variable.target_speed_31_40;

                    break;
                }
                case id.button_41_50: {
                    //RPM_selection_button_Trans(FLAG_RPM_Change_which);
                    FLAG_RPM_Change_which = 4;
                    //RPM_selection_button_NONTrans(FLAG_RPM_Change_which);
                    Global_Variable.Adjust_RPM = Global_Variable.target_speed_41_50;
                    break;
                }
                case id.button_51_60: {
                    //RPM_selection_button_Trans(FLAG_RPM_Change_which);
                    FLAG_RPM_Change_which = 5;
                    //RPM_selection_button_NONTrans(FLAG_RPM_Change_which);
                    Global_Variable.Adjust_RPM = Global_Variable.target_speed_51_60;
                    break;
                }
            }
        }

    }

    private int x_prev = 0;
    private int x_diff = 0;
    @Override
    public boolean onTouchEvent(MotionEvent event) {

        int x = (int)event.getX();
        switch (event.getAction()) {
            case MotionEvent.ACTION_DOWN:{

                if(x > getWindowManager().getDefaultDisplay().getWidth()/2){
                    Global_Variable.Adjust_RPM += 100;
                }
                else {
                    Global_Variable.Adjust_RPM -= 100;
                }
                if(Global_Variable.Adjust_RPM > 4000){Global_Variable.Adjust_RPM = 4000;}
                else if(Global_Variable.Adjust_RPM < 0){Global_Variable.Adjust_RPM = 0;}

                break;
            }
            case MotionEvent.ACTION_MOVE:{

//                x_diff = (x - x_prev)/20 * 100;
//
//                Global_Variable.Adjust_RPM += x_diff;
//                if(Global_Variable.Adjust_RPM > 4000){Global_Variable.Adjust_RPM = 4000;}
//                else if(Global_Variable.Adjust_RPM < 0){Global_Variable.Adjust_RPM = 0;}
//                x_prev = x;

                break;
            }

        }
        return false;
    }

    @Override
    public boolean onTouch(View view, MotionEvent motionEvent) {

        if(motionEvent.getAction() == MotionEvent.ACTION_DOWN || motionEvent.getAction() == motionEvent.ACTION_MOVE){
            view.getBackground().setColorFilter(new LightingColorFilter(0xffffffff,0xffaa0000));
        }

        if(motionEvent.getAction() == MotionEvent.ACTION_UP || motionEvent.getAction() == motionEvent.ACTION_CANCEL){
            view.getBackground().setColorFilter(new LightingColorFilter(0xffffffff,0x00000000));
        }
        return false;
    }

    public void drawLine_for_Gauge(int X_origin, int Y_origin, double length,double percentage, double percentage_Adjust){

        try {
            canvas.drawColor(0, PorterDuff.Mode.CLEAR);

            if (FLAG_run_set == false) {

                double theta = 0;
                double starting_theta = 270;
                double ending_theta = 90;

                if (percentage <= 50) {
                    theta = 270 + (90 - (starting_theta - 270)) * percentage / 50;
                } else if (percentage > 50) {
                    theta = 0 + ending_theta * (percentage - 50) / 50;
                }

                int X_line = 0;
                int Y_line = 0;

                X_line = (int) (length * (Math.sin(Math.toRadians(theta))));
                Y_line = (int) (length * (Math.cos(Math.toRadians(theta))));

                float X_end_point = X_line + X_origin;
                float Y_end_point = -Y_line + Y_origin;

                canvas.drawLine(X_origin, Y_origin, X_end_point, Y_end_point, Gauge_Paint);
            }


            if (FLAG_run_set == true) {

                double theta_Adjust = 0;
                double starting_theta_Adjust = 270;
                double ending_theta_Adjust = 90;

                if (percentage_Adjust <= 50) {
                    theta_Adjust = 270 + (90 - (starting_theta_Adjust - 270)) * percentage_Adjust / 50;
                } else if (percentage_Adjust > 50) {
                    theta_Adjust = 0 + ending_theta_Adjust * (percentage_Adjust - 50) / 50;
                }

                int X_line_Adjust = 0;
                int Y_line_Adjust = 0;

                X_line_Adjust = (int) (length * (Math.sin(Math.toRadians(theta_Adjust))));
                Y_line_Adjust = (int) (length * (Math.cos(Math.toRadians(theta_Adjust))));

                float X_end_point_Adjust = X_line_Adjust + X_origin;
                float Y_end_point_Adjust = -Y_line_Adjust + Y_origin;

                canvas.drawLine(X_origin, Y_origin, X_end_point_Adjust, Y_end_point_Adjust, Adjust_RPM_Paint);

            }

            mView_Drawable.invalidate();
        }
        catch(Exception e){}
    }


    private Runnable runnable_for_slow = new Runnable() {
        @Override
        public void run() {

            if(Global_Variable.Voltage > 40){
                Battery_IMAGE.setImageResource(drawable.battery_04);
            }
            else if (Global_Variable.Voltage <=40 && Global_Variable.Voltage > 38){
                Battery_IMAGE.setImageResource(drawable.battery_03);
            }
            else if (Global_Variable.Voltage <=38 && Global_Variable.Voltage > 36){
                Battery_IMAGE.setImageResource(drawable.battery_02);
            }
            else if (Global_Variable.Voltage <=36){
                Battery_IMAGE.setImageResource(drawable.battery_01);
            }

            handler_battery_update.postDelayed(this,2000);
        }
    };

    void select_current_Image(int fake_current){

        //control the current bar according to what i get from bt
        if(fake_current >=0 && fake_current < 2) {
            Current_IMAGE.setImageResource(drawable.current_01);
        }
        else if(fake_current >= 2 && fake_current < 4){
            Current_IMAGE.setImageResource(drawable.current_02);
        }
        else if(fake_current >= 4 && fake_current < 6){
            Current_IMAGE.setImageResource(drawable.current_03);
        }
        else if(fake_current >= 6 && fake_current < 8){
            Current_IMAGE.setImageResource(drawable.current_04);
        }
        else if(fake_current >= 8 && fake_current < 10){
            Current_IMAGE.setImageResource(drawable.current_05);
        }
        else if(fake_current >= 10 && fake_current < 12){
            Current_IMAGE.setImageResource(drawable.current_06);
        }
        else if(fake_current >= 12 && fake_current < 14){
            Current_IMAGE.setImageResource(drawable.current_07);
        }
        else if(fake_current >= 14 && fake_current < 16){
            Current_IMAGE.setImageResource(drawable.current_08);
        }
        else if(fake_current >= 16 && fake_current < 18){
            Current_IMAGE.setImageResource(drawable.current_09);
        }
        else if(fake_current >= 18 && fake_current < 20){
            Current_IMAGE.setImageResource(drawable.current_10);
        }
        else if(fake_current >= 20 && fake_current < 22){
            Current_IMAGE.setImageResource(drawable.current_11);
        }
        else if(fake_current >= 22 && fake_current < 24){
            Current_IMAGE.setImageResource(drawable.current_12);
        }
        else if(fake_current >= 24 && fake_current < 26){
            Current_IMAGE.setImageResource(drawable.current_13);
        }
        else if(fake_current >= 26 && fake_current < 28){
            Current_IMAGE.setImageResource(drawable.current_14);
        }
        else if(fake_current >= 28 && fake_current < 30){
            Current_IMAGE.setImageResource(drawable.current_15);
        }
        else if(fake_current >= 30 && fake_current < 32){
            Current_IMAGE.setImageResource(drawable.current_16);
        }
        else if(fake_current >= 32 && fake_current < 34){
            Current_IMAGE.setImageResource(drawable.current_17);
        }
        else if(fake_current >= 34 && fake_current < 36){
            Current_IMAGE.setImageResource(drawable.current_18);
        }
        else if(fake_current >= 36 && fake_current < 38){
            Current_IMAGE.setImageResource(drawable.current_19);
        }
        else if(fake_current >= 38 && fake_current < 40){
            Current_IMAGE.setImageResource(drawable.current_20);
        }
        else if(fake_current >= 40 && fake_current < 42){
            Current_IMAGE.setImageResource(drawable.current_21);
        }
        else if(fake_current >= 42 && fake_current < 44){
            Current_IMAGE.setImageResource(drawable.current_22);
        }
        else if(fake_current >= 44 && fake_current < 46){
            Current_IMAGE.setImageResource(drawable.current_23);
        }
        else if(fake_current >= 46 && fake_current < 48){
            Current_IMAGE.setImageResource(drawable.current_24);
        }
        else if(fake_current >= 48 && fake_current < 50){
            Current_IMAGE.setImageResource(drawable.current_25);
        }
        else if(fake_current >= 50 && fake_current < 52){
            Current_IMAGE.setImageResource(drawable.current_26);
        }
        else if(fake_current >= 52 && fake_current < 54){
            Current_IMAGE.setImageResource(drawable.current_27);
        }
        else if(fake_current >= 54 && fake_current < 56){
            Current_IMAGE.setImageResource(drawable.current_28);
        }
        else if(fake_current >= 56 && fake_current < 58){
            Current_IMAGE.setImageResource(drawable.current_29);
        }
        else if(fake_current >= 58){
            Current_IMAGE.setImageResource(drawable.current_30);
        }
    }

    BlueTooth_Lib bt;
    boolean auto_send_request = false;

    private Runnable runnable = new Runnable() {
        @Override
        public void run() {

            //bluetooth automatic sending command for getting battery status
            bt.driver_send_all();
            if( Global_Variable.FLAG_first_start_time == true) {
                txt_write.write_full_title();
                txt_write.write_one_set_data(System.currentTimeMillis() - Global_Variable.starting_time, Global_Variable.Current, Global_Variable.Voltage, Global_Variable.Real_RPM, Global_Variable.Target_Speed);
            }
            handler.postDelayed(this, 100);
        }
    };

    void RPM_selection_button_Trans (int which){

        switch (which){
            case 0:{
                mButton_0.setBackgroundResource(drawable.circle_test_trans);
                break;}
            case 1:{
                mButton_1.setBackgroundResource(drawable.circle_test_trans);
                break;}
            case 2:{
                mButton_2.setBackgroundResource(drawable.circle_test_trans);
                break;}
            case 3:{
                mButton_3.setBackgroundResource(drawable.circle_test_trans);
                break;}
            case 4:{
                mButton_4.setBackgroundResource(drawable.circle_test_trans);
                break;}
            case 5:{
                mButton_5.setBackgroundResource(drawable.circle_test_trans);
                break;}
        }
    }

    void RPM_selection_button_NONTrans (int which){

        switch (which){
            case 0:{
                mButton_0.setBackgroundResource(drawable.circle_test);
                break;}
            case 1:{
                mButton_1.setBackgroundResource(drawable.circle_test);
                break;}
            case 2:{
                mButton_2.setBackgroundResource(drawable.circle_test);
                break;}
            case 3:{
                mButton_3.setBackgroundResource(drawable.circle_test);
                break;}
            case 4:{
                mButton_4.setBackgroundResource(drawable.circle_test);
                break;}
            case 5:{
                mButton_5.setBackgroundResource(drawable.circle_test);
                break;}

        }
    }

    private Runnable runnable_draw = new Runnable() {

            public void run(){

                double RPM_percentage = Global_Variable.Real_RPM * 100 / Global_Variable.Max_RPM;

                double Adjust_RPM_percentage = 0;

                if(FLAG_RPM_Change_which != 10) {
                    Adjust_RPM_percentage = Global_Variable.Adjust_RPM*100/Global_Variable.Max_RPM;
                    switch (FLAG_RPM_Change_which) {
                        case 0: {
                            Global_Variable.target_speed_0_10 = Global_Variable.Adjust_RPM;
                            break;
                        }
                        case 1: {
                            Global_Variable.target_speed_11_20 = Global_Variable.Adjust_RPM;
                            break;
                        }
                        case 2: {
                            Global_Variable.target_speed_21_30 = Global_Variable.Adjust_RPM;
                            break;
                        }
                        case 3: {
                            Global_Variable.target_speed_31_40 = Global_Variable.Adjust_RPM;
                            break;
                        }
                        case 4: {
                            Global_Variable.target_speed_41_50 = Global_Variable.Adjust_RPM;
                            break;
                        }
                        case 5: {
                            Global_Variable.target_speed_51_60 = Global_Variable.Adjust_RPM;
                            break;
                        }
                    }
                }

                drawLine_for_Gauge(getWindowManager().getDefaultDisplay().getWidth()/2,getWindowManager().getDefaultDisplay().getHeight()/2+200,600,
                                    RPM_percentage,//Real RPM
                                    Adjust_RPM_percentage);//The Adjust RPM

                if(FLAG_run_set == false) {

                    select_current_Image((int) Global_Variable.Current);//0-60
                    mTextView_RPM.setText((Global_Variable.Real_RPM) + "\n" + "RPM");

                }
                else if (FLAG_run_set == true){

                    int selected_current = (FLAG_RPM_Change_which+1)*10;
                    select_current_Image( selected_current );
                    mTextView_RPM.setText(Global_Variable.Adjust_RPM+ "\n" + "RPM");

                }

                mDebugText.setText("Max Current:"+Global_Variable.Target_Ampere/1000+"A");
                handler_draw.postDelayed(this,250);

            }

    };


    //helper functions
    public int random_gen(int min, int max) {
        Random r = new Random();
        return r.nextInt(max - min + 1) + min;
    }

//SeekBar Related stuff
    @Override
    public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
        switch (seekBar.getId()){
            case(id.seekBar_MAXA):{
                Global_Variable.Target_Ampere = i * 5000;
                break;
            }
        }
    }

    @Override
    public void onStartTrackingTouch(SeekBar seekBar) {

    }

    @Override
    public void onStopTrackingTouch(SeekBar seekBar) {

    }

}
