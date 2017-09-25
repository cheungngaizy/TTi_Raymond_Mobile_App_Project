package com.example.ptencheu.bt_bat_36v_demo;

import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.ColorFilter;
import android.graphics.drawable.Drawable;
import android.support.annotation.IntRange;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.view.View;

import com.github.mikephil.charting.charts.BarChart;
import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.components.Legend;
import com.github.mikephil.charting.components.XAxis;
import com.github.mikephil.charting.components.YAxis;
import com.github.mikephil.charting.data.BarData;
import com.github.mikephil.charting.data.BarDataSet;
import com.github.mikephil.charting.data.BarEntry;
import com.github.mikephil.charting.data.Entry;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;
import com.github.mikephil.charting.formatter.IndexAxisValueFormatter;
import com.github.mikephil.charting.interfaces.datasets.IBarDataSet;
import com.github.mikephil.charting.interfaces.datasets.ILineDataSet;
import com.github.mikephil.charting.utils.ColorTemplate;

import java.util.ArrayList;

/**
 * Created by PTENCHEU on 9/4/2017.
 */

public class mBarChart extends AppCompatActivity {

    private BarChart mChart;
    private BarData mChart_BarData;
    private BarDataSet mChart_DataSet;

    private long start_class_time;



    public mBarChart(View mView,int set0_10,int set11_20,int set21_30,int set31_40,int set41_50,int set51_60){

        int total = set0_10 + set11_20 + set21_30 + set31_40 + set41_50 + set51_60;

        float set0_10_f = (float)set0_10*100/total;
        float set11_20_f = (float)set11_20*100/total;
        float set21_30_f = (float)set21_30*100/total;
        float set31_40_f = (float)set31_40*100/total;
        float set41_50_f = (float)set41_50*100/total;
        float set51_60_f = (float)set51_60*100/total;

        mChart = (BarChart) mView;
        mChart_BarData = new BarData();
        start_class_time = System.currentTimeMillis();
        Setting();

        mChart_BarData = mChart.getBarData();
        mChart.setPinchZoom(false);
        ArrayList<BarEntry> yVals1 = new ArrayList<BarEntry>();
        yVals1.add(new BarEntry(0,set0_10_f));
        BarDataSet set1;
        set1 = new BarDataSet(yVals1, "0-10");
        set1.setColors(Color.GREEN);
        set1.setValueTextColor(Color.WHITE);
        set1.setValueTextSize(15);
        set1.setDrawValues(true);

        ArrayList<BarEntry> yVals2 = new ArrayList<BarEntry>();
        yVals2.add(new BarEntry(1,set11_20_f));
        BarDataSet set2;
        set2 = new BarDataSet(yVals2, "11-20");
        set2.setColors(Color.BLUE);
        set2.setValueTextColor(Color.WHITE);
        set2.setValueTextSize(15);
        set2.setDrawValues(true);

        ArrayList<BarEntry> yVals3 = new ArrayList<BarEntry>();
        yVals3.add(new BarEntry(2,set21_30_f));
        BarDataSet set3;
        set3 = new BarDataSet(yVals3, "21-30");
        set3.setColors(Color.GRAY);
        set3.setValueTextColor(Color.WHITE);
        set3.setValueTextSize(15);
        set3.setDrawValues(true);

        ArrayList<BarEntry> yVals4 = new ArrayList<BarEntry>();
        yVals4.add(new BarEntry(3,set31_40_f));
        BarDataSet set4;
        set4 = new BarDataSet(yVals4, "31-40");
        set4.setColors(Color.BLACK);
        set4.setValueTextColor(Color.WHITE);
        set4.setValueTextSize(15);
        set4.setDrawValues(true);

        ArrayList<BarEntry> yVals5 = new ArrayList<BarEntry>();
        yVals5.add(new BarEntry(4,set41_50_f));
        BarDataSet set5;
        set5 = new BarDataSet(yVals5, "41-50");
        set5.setColors(Color.CYAN);
        set5.setValueTextColor(Color.WHITE);
        set5.setValueTextSize(15);
        set5.setDrawValues(true);

        ArrayList<BarEntry> yVals6 = new ArrayList<BarEntry>();
        yVals6.add(new BarEntry(5,set51_60_f));
        BarDataSet set6;
        set6 = new BarDataSet(yVals6, "51-60");
        set6.setColors(Color.MAGENTA);
        set6.setValueTextColor(Color.WHITE);
        set6.setValueTextSize(15);
        set6.setDrawValues(true);




        ArrayList<IBarDataSet> dataSets = new ArrayList<IBarDataSet>();
        dataSets.add(set1);
        dataSets.add(set2);
        dataSets.add(set3);
        dataSets.add(set4);
        dataSets.add(set5);
        dataSets.add(set6);

        ArrayList<String> labels = new ArrayList<String>();
        labels.add("0-10");
        labels.add("11-20");
        labels.add("21-30");
        labels.add("31-40");
        labels.add("41-50");
        labels.add("51-60");



        mChart.getXAxis().setValueFormatter(new IndexAxisValueFormatter(labels));
        BarData data = new BarData(dataSets);

        mChart.setData(data);
        mChart.setFitBars(true);

        mChart.invalidate();


    }



    public void Setting(){

        mChart.setHardwareAccelerationEnabled(true);
        // enable description text
        mChart.getDescription().setEnabled(true);

        // enable touch gestures
        mChart.setTouchEnabled(true);

        // enable scaling and dragging
        mChart.setDragEnabled(true);
        mChart.setScaleEnabled(true);
        mChart.setDrawGridBackground(false);

        //mChart.setAutoScaleMinMaxEnabled(true);

        // if disabled, scaling can be done on x- and y-axis separately
        mChart.setPinchZoom(false);

        // set an alternative background color
        mChart.setBackgroundColor(Color.DKGRAY);

        mChart_BarData.setValueTextColor(Color.WHITE);

        // add empty data
        mChart.setData(mChart_BarData);

        // get the legend (only possible after setting data)
        Legend l = mChart.getLegend();
        l.setTextColor(Color.WHITE);
        // modify the legend ...
        l.setForm(Legend.LegendForm.LINE);
        //l.setTypeface(mTfLight);
        l.setTextColor(Color.WHITE);

        XAxis xl = mChart.getXAxis();
        //xl.setTypeface(mTfLight);
        xl.setTextColor(Color.WHITE);
        xl.setDrawGridLines(false);
        xl.setAvoidFirstLastClipping(true);
        xl.setPosition(XAxis.XAxisPosition.BOTTOM);
        xl.setEnabled(true);

        YAxis leftAxis = mChart.getAxisLeft();
        //
        // leftAxis.setTypeface(mTfLight);
        leftAxis.setTextColor(Color.WHITE);

        leftAxis.setDrawGridLines(true);

        YAxis rightAxis = mChart.getAxisRight();
        rightAxis.setEnabled(false);

    }







}
