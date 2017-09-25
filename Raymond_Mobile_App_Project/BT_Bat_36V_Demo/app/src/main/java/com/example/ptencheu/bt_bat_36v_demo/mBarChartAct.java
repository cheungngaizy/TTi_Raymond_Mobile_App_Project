package com.example.ptencheu.bt_bat_36v_demo;

import android.content.pm.ActivityInfo;
import android.graphics.Color;
import android.os.Handler;
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

        Analyst = new mBarChart(findViewById(R.id.BarChart),
                Global_Variable.set0_10_counter,
                Global_Variable.set11_20_counter,
                Global_Variable.set21_30_counter,
                Global_Variable.set31_40_counter,
                Global_Variable.set41_50_counter,
                Global_Variable.set51_60_counter);

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

            //Analyst.insertData(1,Global_Variable.Current);
            float total = Global_Variable.set0_10_counter+Global_Variable.set11_20_counter+Global_Variable.set21_30_counter+Global_Variable.set31_40_counter+Global_Variable.set41_50_counter+Global_Variable.set51_60_counter;
            float percentage_Set_0_10 = Global_Variable.set0_10_counter/total*100;
            float percentage_Set_11_20 = Global_Variable.set11_20_counter/total*100;
            float percentage_Set_21_30 = Global_Variable.set21_30_counter/total*100;
            float percentage_Set_31_40 = Global_Variable.set31_40_counter/total*100;
            float percentage_Set_41_50 = Global_Variable.set41_50_counter/total*100;
            float percentage_Set_51_60 = Global_Variable.set51_60_counter/total*100;


//            text_percentage.setText(Html.fromHtml("0-10A:"+ percentage_Set_0_10+"%"+ "<br>"+
//                    "11_20A:"+ percentage_Set_11_20+"%"+ "<br>"+
//                    "21_30A:"+ percentage_Set_21_30+"%"+ "<br>"+
//                    "31_40A:"+ percentage_Set_31_40+"%"+ "<br>"+
//                    "41_50A:"+ percentage_Set_41_50+"%"+ "<br>"+
//                    "51_60A:"+ percentage_Set_51_60+"%"+ "<br>"));

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

            text_target_speed.setText(   "RPM: "+  hi1
                                        +"      M      "+Global_Variable.target_speed_11_20
                                        +"      M      "+Global_Variable.target_speed_21_30
                                        +"      M      "+Global_Variable.target_speed_31_40
                                        +"      M      "+Global_Variable.target_speed_41_50
                                        +"      M      "+Global_Variable.target_speed_51_60);

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

    @Override
    public void onStartTrackingTouch(SeekBar seekBar) {

    }

    @Override
    public void onStopTrackingTouch(SeekBar seekBar) {

    }
}
