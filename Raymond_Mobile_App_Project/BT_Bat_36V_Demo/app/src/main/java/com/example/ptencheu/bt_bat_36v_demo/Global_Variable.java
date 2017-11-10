package com.example.ptencheu.bt_bat_36v_demo;

/**
 * Created by PTENCHEU on 9/4/2017.
 */

public class Global_Variable
{


    public static float Current = 0;
    public static float Voltage = 36f;
    public static int Real_RPM = 0;
    public static int Adjust_RPM = 0;
    public static final int Max_RPM = 4000;

    public static int set0_10_counter = 20;
    public static int set11_20_counter = 50;
    public static int set21_30_counter = 10;
    public static int set31_40_counter = 10;
    public static int set41_50_counter = 10;
    public static int set51_60_counter = 10;


    public static int target_speed_0_10 = 2000;
    public static int target_speed_11_20 = 2500;
    public static int target_speed_21_30 = 3000;
    public static int target_speed_31_40 = 3200;
    public static int target_speed_41_50 = 3000;
    public static int target_speed_51_60 = 2800;


    public static long counter_0_5 = 0;
    public static long counter_5_10 = 0;
    public static long counter_10_15 = 0;
    public static long counter_15_20 = 0;
    public static long counter_20_25 = 0;
    public static long counter_25_30 = 0;
    public static long counter_30_35 = 0;
    public static long counter_35_40 = 0;
    public static long counter_40_45 = 0;
    public static long counter_45_50 = 0;
    public static long counter_50_55 = 0;
    public static long counter_55_60 = 0;


    public static int Target_Speed = 2000;
    public static int Target_Ampere = 60000;
    public static boolean Target_Power_Limitation_On_Off = true;



    public static float Ampere_Hour = 0.0f;
    public static long cutting_time = 0;//in millisecond

    public static long starting_time = 0;
    public static boolean FLAG_first_start_time = false;

    public static String bt_name;


}
