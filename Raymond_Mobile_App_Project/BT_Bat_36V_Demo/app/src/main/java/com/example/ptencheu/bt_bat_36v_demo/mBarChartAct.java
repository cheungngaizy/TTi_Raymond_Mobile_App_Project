package com.example.ptencheu.bt_bat_36v_demo;

import android.content.pm.ActivityInfo;
import android.graphics.Color;
import android.graphics.Typeface;
import android.icu.text.DecimalFormat;
import android.os.Handler;
import android.provider.Settings;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.Html;
import android.view.View;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.TextView;

import org.w3c.dom.Text;


public class mBarChartAct extends AppCompatActivity implements View.OnClickListener, SeekBar.OnSeekBarChangeListener {

    mBarChart Analyst;
    Handler handler;
    Button button_Back;

    TextView text_percentage;
    TextView text_target_speed;
    TextView text_Ah;
    TextView text_Cutting_Time;



    SeekBar mSeekBar_0_10;
    SeekBar mSeekBar_11_20;
    SeekBar mSeekBar_21_30;
    SeekBar mSeekBar_31_40;
    SeekBar mSeekBar_41_50;
    SeekBar mSeekBar_51_60;

    final int max_speed = 4000;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_m_bar_chart);
        this.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);



        text_percentage = (TextView)findViewById(R.id.textView_Percentage);
        text_target_speed = (TextView)findViewById(R.id.textView_TargetSpeed);

        Typeface type = Typeface.createFromAsset(getAssets(),"fonts/SFDigitalReadout-Medium.ttf");


        text_Ah = (TextView)findViewById(R.id.mRight_Ah);
        text_Ah.setTypeface(type);
        text_Ah.setTextSize(30);
        text_Ah.setTextColor(Color.WHITE);
        text_Ah.setText("Used Capacity:"+String.format("%.2f",Global_Variable.Ampere_Hour)+ " Ah");

        text_Cutting_Time = (TextView)findViewById(R.id.mLeft_cutting_tim);
        text_Cutting_Time.setTypeface(type);
        text_Cutting_Time.setTextSize(30);
        text_Cutting_Time.setTextColor(Color.WHITE);

        //if(Global_Variable.FLAG_first_start_time == true) {
            text_Cutting_Time.setText("Run Time:" + Global_Variable.cutting_time / 60000 + " Mins");
        //}

        Analyst = new mBarChart(findViewById(R.id.BarChart));

        button_Back = (Button)findViewById(R.id.button_Bar);
        button_Back.setOnClickListener(this);

        mSeekBar_0_10 = (SeekBar)findViewById(R.id.seekBar_0_10);
        mSeekBar_0_10.setProgress(Global_Variable.target_speed_0_10*100/max_speed);
        mSeekBar_0_10.setOnSeekBarChangeListener(this);

        mSeekBar_11_20 = (SeekBar)findViewById(R.id.seekBar_11_20);
        mSeekBar_11_20.setProgress(Global_Variable.target_speed_11_20*100/max_speed);
        mSeekBar_11_20.setOnSeekBarChangeListener(this);

        mSeekBar_21_30 = (SeekBar)findViewById(R.id.seekBar_21_30);
        mSeekBar_21_30.setProgress(Global_Variable.target_speed_21_30*100/max_speed);
        mSeekBar_21_30.setOnSeekBarChangeListener(this);

        mSeekBar_31_40 = (SeekBar)findViewById(R.id.seekBar_31_40);
        mSeekBar_31_40.setProgress(Global_Variable.target_speed_31_40*100/max_speed);
        mSeekBar_31_40.setOnSeekBarChangeListener(this);

        mSeekBar_41_50 = (SeekBar)findViewById(R.id.seekBar_41_50);
        mSeekBar_41_50.setProgress(Global_Variable.target_speed_41_50*100/max_speed);
        mSeekBar_41_50.setOnSeekBarChangeListener(this);

        mSeekBar_51_60 = (SeekBar)findViewById(R.id.seekBar_51_60);
        mSeekBar_51_60.setProgress(Global_Variable.target_speed_51_60*100/max_speed);
        mSeekBar_51_60.setOnSeekBarChangeListener(this);

        handler = new Handler();
        handler.post(runnable);
    }


    private Runnable runnable = new Runnable() {

        public void run(){


            text_target_speed.setTextColor(Color.WHITE);

            String space1 = "  ";
            String space2 = "    ";
            String space3 = "      ";

            String hi1 = "";

            if(Global_Variable.target_speed_0_10 < 1000 && Global_Variable.target_speed_0_10 > 100){
                hi1 = space1 + Global_Variable.target_speed_0_10;
            }
            else if(Global_Variable.target_speed_0_10 < 100 && Global_Variable.target_speed_0_10 > 10){
                hi1 = space2 + Global_Variable.target_speed_0_10;
            }
            else if(Global_Variable.target_speed_0_10 < 10){
                hi1 = space3 + Global_Variable.target_speed_0_10;
            }
            else if (Global_Variable.target_speed_0_10 >=1000){
                hi1 = hi1 +Global_Variable.target_speed_0_10;
            }

            if(FLAG_show_speed_Text == true) {
                text_target_speed.setText("%    RPM: " + hi1
                        + "      M      " + Global_Variable.target_speed_11_20
                        + "      M      " + Global_Variable.target_speed_21_30
                        + "      M      " + Global_Variable.target_speed_31_40
                        + "      M      " + Global_Variable.target_speed_41_50
                        + "      M      " + Global_Variable.target_speed_51_60);
            }
            else if (FLAG_show_speed_Text == false){
                //text_target_speed.setText("%");
            }

            handler.postDelayed(this,100);

        }

    };


    @Override
    public void onClick(View view) {

        switch(view.getId()){
            case R.id.button_Bar:{
                finish();
                break;
            }

        }
    }

    @Override
    public void onProgressChanged(SeekBar seekBar, int i, boolean b) {


        int get_value_from_i = i*max_speed/100;

        switch (seekBar.getId()){

            case R.id.seekBar_0_10: {
                Global_Variable.target_speed_0_10 = get_value_from_i;
                break;
            }
            case R.id.seekBar_11_20: {
                Global_Variable.target_speed_11_20 = get_value_from_i;
                break;
            }
            case R.id.seekBar_21_30: {
                Global_Variable.target_speed_21_30 = get_value_from_i;
                break;
            }
            case R.id.seekBar_31_40: {
                Global_Variable.target_speed_31_40 = get_value_from_i;
                break;
            }
            case R.id.seekBar_41_50: {
                Global_Variable.target_speed_41_50 = get_value_from_i;
                break;
            }
            case R.id.seekBar_51_60: {
                Global_Variable.target_speed_51_60 = get_value_from_i;
                break;
            }

        }

    }

    boolean FLAG_show_speed_Text = false;

    @Override
    public void onStartTrackingTouch(SeekBar seekBar) {
        FLAG_show_speed_Text = true;

    }

    @Override
    public void onStopTrackingTouch(SeekBar seekBar) {
        FLAG_show_speed_Text = false;
    }
}
