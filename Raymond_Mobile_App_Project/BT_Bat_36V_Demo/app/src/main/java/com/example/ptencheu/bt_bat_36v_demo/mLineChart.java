package com.example.ptencheu.bt_bat_36v_demo;

import android.content.Context;
import android.graphics.Color;
import android.graphics.drawable.Drawable;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;

import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.components.Legend;
import com.github.mikephil.charting.components.XAxis;
import com.github.mikephil.charting.components.YAxis;
import com.github.mikephil.charting.data.Entry;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;
import com.github.mikephil.charting.interfaces.datasets.ILineDataSet;
import com.github.mikephil.charting.utils.ColorTemplate;

/**
 * Created by PTENCHEU on 8/27/2017.
 */

public class mLineChart extends AppCompatActivity {

    private LineChart mChart;
    private LineData mChart_Linedata;

    public float xAxisData;
    public float yAxisData;

    private long start_class_time;

    public mLineChart(View mView){
        mChart = (LineChart) mView;
        mChart_Linedata = new LineData();
        Setting();
        start_class_time = System.currentTimeMillis();
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
        mChart.setBackgroundColor(Color.LTGRAY);

        //Drawable backgoundImage = getResources().getDrawable(R.drawable.capture);
        //mChart.setBackground(backgoundImage);

        mChart_Linedata.setValueTextColor(Color.WHITE);

        // add empty data
        mChart.setData(mChart_Linedata);

        // get the legend (only possible after setting data)
        Legend l = mChart.getLegend();

        // modify the legend ...
        l.setForm(Legend.LegendForm.LINE);
        //l.setTypeface(mTfLight);
        l.setTextColor(Color.WHITE);

        XAxis xl = mChart.getXAxis();
        //xl.setTypeface(mTfLight);
        xl.setTextColor(Color.BLACK);
        xl.setDrawGridLines(false);
        xl.setAvoidFirstLastClipping(true);
        xl.setPosition(XAxis.XAxisPosition.BOTTOM);
        xl.setEnabled(true);

        YAxis leftAxis = mChart.getAxisLeft();
        //
        // leftAxis.setTypeface(mTfLight);
        leftAxis.setTextColor(Color.BLACK);
        leftAxis.setAxisMaximum(60f);
        leftAxis.setAxisMinimum(-20f);
        leftAxis.setDrawGridLines(true);

        YAxis rightAxis = mChart.getAxisRight();
        rightAxis.setEnabled(false);

    }

    public void insertData(float x, float y){

        LineData data = mChart.getData();

        if (data != null) {

            ILineDataSet set = data.getDataSetByIndex(0);
            // set.addEntry(...); // can be called as well

            if (set == null) {
                set = createSet(2);
                data.addDataSet(set);
            }

            long interval = System.currentTimeMillis() - start_class_time;
        //    Log.d("mLineChart", String.valueOf(interval));

//            if(y>0){
//                set = createSet(2);
//                data.addDataSet(set);
//            }
//            else if(y<=0){
//                set = createSet(1);
//
//               data.addDataSet(set);
//            }




            data.addEntry(new Entry((interval), y), 0);


            data.notifyDataChanged();

            // let the chart know it's data has changed
            mChart.notifyDataSetChanged();
            // limit the number of visible entries
            mChart.setVisibleXRangeMaximum(2000);

            // move to the latest entry
            mChart.moveViewToX(data.getXMax());


            // mChart.setVisibleYRange(30, AxisDependency.LEFT);
            // this automatically refreshes the chart (calls invalidate())
            // mChart.moveViewTo(data.getXValCount()-7, 55f,
            // AxisDependency.LEFT);
        }

    }


    private LineDataSet createSet(int mode) {

        LineDataSet set = new LineDataSet(null, "Current(A)");
        if(mode == 1){
            set.setAxisDependency(YAxis.AxisDependency.LEFT);
            set.setColor(ColorTemplate.getHoloBlue());
            set.setCircleColor(Color.WHITE);
            set.setLineWidth(2f);
            set.setCircleRadius(1f);
            set.setFillAlpha(65);
            set.setFillColor(ColorTemplate.getHoloBlue());
            set.setHighLightColor(Color.rgb(244, 117, 117));
            set.setValueTextColor(Color.WHITE);
            set.setValueTextSize(9f);
            set.setDrawValues(false);
            set.setDrawFilled(true);
            set.setCubicIntensity(0.05f);
            set.setMode(LineDataSet.Mode.CUBIC_BEZIER);
        }
        else if(mode == 2){

            set.setAxisDependency(YAxis.AxisDependency.LEFT);
            set.setColor(Color.CYAN);
            set.setCircleColor(Color.CYAN);
            set.setLineWidth(2f);
            set.setCircleRadius(1f);
            set.setFillAlpha(65);
            set.setFillColor(ColorTemplate.getHoloBlue());
            set.setHighLightColor(Color.rgb(24, 11, 117));
            set.setValueTextColor(Color.CYAN);
            set.setValueTextSize(9f);
            set.setDrawValues(false);
            set.setDrawFilled(true);
            set.setCubicIntensity(0.05f);
            set.setMode(LineDataSet.Mode.CUBIC_BEZIER);


        }
        return set;
    }


    void input_one_data(){


    }




}
