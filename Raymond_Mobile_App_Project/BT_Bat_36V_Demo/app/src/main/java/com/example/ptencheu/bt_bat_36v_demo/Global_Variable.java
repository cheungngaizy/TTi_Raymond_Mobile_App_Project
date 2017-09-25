package com.example.ptencheu.bt_bat_36v_demo;

/**
 * Created by PTENCHEU on 9/4/2017.
 */

public class Global_Variable
{


    public static float Current = 0;
    public static float Voltage = 0;
    public static int Real_RPM = 0;
    public static int Adjust_RPM = 0;
    public static final int Max_RPM = 4000;

    public static int set0_10_counter = 0;
    public static int set11_20_counter = 0;
    public static int set21_30_counter = 0;
    public static int set31_40_counter = 0;
    public static int set41_50_counter = 0;
    public static int set51_60_counter = 0;


    public static int target_speed_0_10 = 2000;
    public static int target_speed_11_20 = 2500;
    public static int target_speed_21_30 = 3000;
    public static int target_speed_31_40 = 3200;
    public static int target_speed_41_50 = 3000;
    public static int target_speed_51_60 = 2800;

    public static int Target_Speed = 2000;
    public static int Target_Ampere = 60000;
    public static boolean Target_Power_Limitation_On_Off = true;


}
