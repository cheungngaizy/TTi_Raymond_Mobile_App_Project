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
import com.github.mikephil.charting.formatter.PercentFormatter;
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



    public mBarChart(View mView){

//        int total = set0_10 + set11_20 + set21_30 + set31_40 + set41_50 + set51_60;
//
//        float set0_10_f = (float)set0_10*100/total;
//        float set11_20_f = (float)set11_20*100/total;
//        float set21_30_f = (float)set21_30*100/total;
//        float set31_40_f = (float)set31_40*100/total;
//        float set41_50_f = (float)set41_50*100/total;
//        float set51_60_f = (float)set51_60*100/total;

        double total_new = Global_Variable.counter_0_5 + Global_Variable.counter_5_10
                +Global_Variable.counter_10_15 + Global_Variable.counter_15_20
                +Global_Variable.counter_20_25 + Global_Variable.counter_25_30
                +Global_Variable.counter_30_35 + Global_Variable.counter_35_40
                +Global_Variable.counter_40_45 + Global_Variable.counter_45_50
                +Global_Variable.counter_50_55 + Global_Variable.counter_55_60;

        double p0_5 = Global_Variable.counter_0_5*100/total_new;
        double p5_10 = Global_Variable.counter_5_10*100/total_new;
        double p10_15 = Global_Variable.counter_10_15*100/total_new;
        double p15_20 = Global_Variable.counter_15_20*100/total_new;
        double p20_25 = Global_Variable.counter_20_25*100/total_new;
        double p25_30 = Global_Variable.counter_25_30*100/total_new;
        double p30_35 = Global_Variable.counter_30_35*100/total_new;
        double p35_40 = Global_Variable.counter_35_40*100/total_new;
        double p40_45 = Global_Variable.counter_40_45*100/total_new;
        double p45_50 = Global_Variable.counter_45_50*100/total_new;
        double p50_55 = Global_Variable.counter_50_55*100/total_new;
        double p55_60 = Global_Variable.counter_55_60*100/total_new;



        mChart = (BarChart) mView;
        mChart_BarData = new BarData();
        start_class_time = System.currentTimeMillis();

        Setting();

        mChart_BarData = mChart.getBarData();
        mChart.setPinchZoom(false);
        ArrayList<BarEntry> yVals1 = new ArrayList<BarEntry>();
        yVals1.add(new BarEntry(0, (float) p0_5));
        BarDataSet set1;
        set1 = new BarDataSet(yVals1, "");
        set1.setColors(Color.rgb(0x39,0xc0,0x39));
        set1.setValueTextColor(Color.WHITE);
        set1.setValueTextSize(15);
        set1.setValueFormatter(new PercentFormatter());
        set1.setDrawValues(true);



        ArrayList<BarEntry> yVals2 = new ArrayList<BarEntry>();
        yVals2.add(new BarEntry(1,(float)p5_10));
        BarDataSet set2;
        set2 = new BarDataSet(yVals2, "");
        set2.setColors(Color.rgb(0x4b,0xc0,0x39));
        set2.setValueTextColor(Color.WHITE);
        set2.setValueTextSize(15);
        set2.setValueFormatter(new PercentFormatter());
        set2.setDrawValues(true);


        ArrayList<BarEntry> yVals3 = new ArrayList<BarEntry>();
        yVals3.add(new BarEntry(2,(float)p10_15));
        BarDataSet set3;
        set3 = new BarDataSet(yVals3, "");
        set3.setColors(Color.rgb(0x66,0xc0,0x39));
        set3.setValueTextColor(Color.WHITE);
        set3.setValueTextSize(15);
        set3.setValueFormatter(new PercentFormatter());
        set3.setDrawValues(true);

        ArrayList<BarEntry> yVals4 = new ArrayList<BarEntry>();
        yVals4.add(new BarEntry(3,(float)p15_20));
        BarDataSet set4;
        set4 = new BarDataSet(yVals4, "");
        set4.setColors(Color.rgb(0x78,0xc0,0x39));
        set4.setValueTextColor(Color.WHITE);
        set4.setValueTextSize(15);
        set4.setValueFormatter(new PercentFormatter());
        set4.setDrawValues(true);

        ArrayList<BarEntry> yVals5 = new ArrayList<BarEntry>();
        yVals5.add(new BarEntry(4,(float)p20_25));
        BarDataSet set5;
        set5 = new BarDataSet(yVals5, "");
        set5.setColors(Color.rgb(0x8f,0xc0,0x39));
        set5.setValueTextColor(Color.WHITE);
        set5.setValueTextSize(15);
        set5.setValueFormatter(new PercentFormatter());
        set5.setDrawValues(true);

        ArrayList<BarEntry> yVals6 = new ArrayList<BarEntry>();
        yVals6.add(new BarEntry(5,(float)p25_30));
        BarDataSet set6;
        set6 = new BarDataSet(yVals6, "");
        set6.setColors(Color.rgb(0xa5,0xc0,0x39));
        set6.setValueTextColor(Color.WHITE);
        set6.setValueTextSize(15);
        set6.setValueFormatter(new PercentFormatter());
        set6.setDrawValues(true);

        ArrayList<BarEntry> yVals7 = new ArrayList<BarEntry>();
        yVals6.add(new BarEntry(6,(float)p30_35));
        BarDataSet set7;
        set7 = new BarDataSet(yVals7, "");
        set7.setColors(Color.rgb(0xbc,0xc0,0x39));
        set7.setValueTextColor(Color.WHITE);
        set7.setValueTextSize(15);
        set7.setValueFormatter(new PercentFormatter());
        set7.setDrawValues(true);

        ArrayList<BarEntry> yVals8 = new ArrayList<BarEntry>();
        yVals8.add(new BarEntry(7,(float)p35_40));
        BarDataSet set8;
        set8 = new BarDataSet(yVals8, "");
        set8.setColors(Color.rgb(0xc0,0xaa,0x39));
        set8.setValueTextColor(Color.WHITE);
        set8.setValueTextSize(15);
        set8.setValueFormatter(new PercentFormatter());
        set8.setDrawValues(true);

        ArrayList<BarEntry> yVals9 = new ArrayList<BarEntry>();
        yVals9.add(new BarEntry(8,(float)p40_45));
        BarDataSet set9;
        set9 = new BarDataSet(yVals9, "");
        set9.setColors(Color.rgb(0xc0,0x95,0x39));
        set9.setValueTextColor(Color.WHITE);
        set9.setValueTextSize(15);
        set9.setValueFormatter(new PercentFormatter());
        set9.setDrawValues(true);

        ArrayList<BarEntry> yVals10 = new ArrayList<BarEntry>();
        yVals10.add(new BarEntry(9,(float)p45_50));
        BarDataSet set10;
        set10 = new BarDataSet(yVals10, "");
        set10.setColors(Color.rgb(0xc0,0x7f,0x39));
        set10.setValueTextColor(Color.WHITE);
        set10.setValueTextSize(15);
        set10.setValueFormatter(new PercentFormatter());
        set10.setDrawValues(true);

        ArrayList<BarEntry> yVals11 = new ArrayList<BarEntry>();
        yVals11.add(new BarEntry(10,(float)p50_55));
        BarDataSet set11;
        set11 = new BarDataSet(yVals11, "");
        set11.setColors(Color.rgb(0xc0,0x68,0x39));
        set11.setValueTextColor(Color.WHITE);
        set11.setValueTextSize(15);
        set11.setValueFormatter(new PercentFormatter());
        set11.setDrawValues(true);

        ArrayList<BarEntry> yVals12 = new ArrayList<BarEntry>();
        yVals12.add(new BarEntry(11,(float)p55_60));
        BarDataSet set12;
        set12 = new BarDataSet(yVals12, "");
        set12.setColors(Color.rgb(0xc0,0x49,0x39));
        set12.setValueTextColor(Color.WHITE);
        set12.setValueTextSize(15);
        set12.setValueFormatter(new PercentFormatter());
        set12.setDrawValues(true);

        ArrayList<IBarDataSet> dataSets = new ArrayList<IBarDataSet>();
        dataSets.add(set1);
        dataSets.add(set2);
        dataSets.add(set3);
        dataSets.add(set4);
        dataSets.add(set5);
        dataSets.add(set6);
        dataSets.add(set7);
        dataSets.add(set8);
        dataSets.add(set9);
        dataSets.add(set10);
        dataSets.add(set11);
        dataSets.add(set12);

//        ArrayList<String> labels = new ArrayList<String>();
//        labels.add("0-10");
//        labels.add("11-20");
//        labels.add("21-30");
//        labels.add("31-40");
//        labels.add("41-50");
//        labels.add("51-60");
//


        //mChart.getXAxis().setValueFormatter(new IndexAxisValueFormatter(labels));
        BarData data = new BarData(dataSets);

        mChart.setData(data);
        mChart.setFitBars(true);
        mChart.getXAxis().setEnabled(false);

        mChart.getXAxis().setDrawLabels(false);
        mChart.getLegend().setEnabled(false);

        mChart.getAxisLeft().setDrawLabels(false);

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
        mChart.setBackgroundColor(Color.rgb(0x40,0x40,0x40));

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
        l.setTextSize(13);

        XAxis xl = mChart.getXAxis();
        //xl.setTypeface(mTfLight);
        xl.setTextColor(Color.WHITE);
        xl.setDrawGridLines(false);
        xl.setAvoidFirstLastClipping(true);
        xl.setPosition(XAxis.XAxisPosition.BOTTOM);
        xl.setEnabled(true);
        xl.setTextSize(13);

        YAxis leftAxis = mChart.getAxisLeft();
        //
        // leftAxis.setTypeface(mTfLight);
        leftAxis.setTextColor(Color.WHITE);
        leftAxis.setTextSize(13);
        leftAxis.setDrawGridLines(true);
        leftAxis.setSpaceBottom(0);
        leftAxis.setAxisMaximum(100);
        leftAxis.setAxisMinimum(0);


        YAxis rightAxis = mChart.getAxisRight();
        rightAxis.setEnabled(false);

        mChart.getDescription().setText("Current Set(A)");
        mChart.getDescription().setTextColor(Color.WHITE);
        mChart.getDescription().setTextSize(13);
        mChart.getDescription().setPosition(1700,1410);

    }







}
