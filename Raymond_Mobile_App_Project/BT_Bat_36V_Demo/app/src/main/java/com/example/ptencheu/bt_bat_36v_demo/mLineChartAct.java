package com.example.ptencheu.bt_bat_36v_demo;

import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.github.mikephil.charting.charts.LineChart;

public class mLineChartAct extends AppCompatActivity implements View.OnClickListener {

    mLineChart Current_Line;
    BlueTooth_Lib bt;
    Button mButton_BACK;
    Handler handler;
    TextView textView;
    Intent i;
    int hi;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_m_line_chart);

        this.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);

        mButton_BACK = (Button)findViewById(R.id.button_Back);
        mButton_BACK.setOnClickListener(this);
        Current_Line = new mLineChart(findViewById(R.id.chart));

        textView = (TextView)findViewById(R.id.textView);
        textView.setTextSize(50);

        handler = new Handler();
        handler.post(runnable);



    }


    public void onClick(View v) {

        switch (v.getId()) {

            case R.id.button_Back: {
                finish();
                break;}

        }

    }


    private Runnable runnable = new Runnable() {

        public void run(){

            Current_Line.insertData(1,Global_Variable.Current);
            float Power = Global_Variable.Voltage*Global_Variable.Current;
            textView.setText("Power(W):" + Power);
            handler.postDelayed(this,100);

        }

    };


}
