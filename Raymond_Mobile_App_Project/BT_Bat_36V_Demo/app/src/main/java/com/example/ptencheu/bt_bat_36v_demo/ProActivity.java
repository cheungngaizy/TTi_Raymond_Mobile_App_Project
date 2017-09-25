package com.example.ptencheu.bt_bat_36v_demo;

import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

public class ProActivity extends AppCompatActivity {

    //chart
    mLineChart mChart_Current;

    Handler handler;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_pro);
        mChart_Current = new mLineChart(findViewById(R.id.chart_Current));

        Handler handler = new Handler();
        handler.post(runnable);
    }


    private Runnable runnable = new Runnable() {
        @Override
        public void run() {

//            bt.bt_sent(bt.Req_Bat_Profile_Command);

//            mChart_Current.insertData(1, bt.Get_Data_Current());

            //mText.setText("size:x"+getWindowManager().getDefaultDisplay().getWidth()+"y"+getWindowManager().getDefaultDisplay().getHeight());
            handler.postDelayed(this, 100);
        }
    };





}
