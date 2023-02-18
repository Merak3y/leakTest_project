#ifndef myHMILibraryIMM_H
#define myMHILibraryIMM_H



extern float data_sf[NUM_SEVEN_FOLD];
extern float data_tf[NUM_TWELVE_FOLD];

extern float aveRecursive_sf[NUM_SEVEN_FOLD];
extern float aveRecursive_sf_old[NUM_SEVEN_FOLD];
extern float aveRecursive_tf[NUM_TWELVE_FOLD];
extern float aveRecursive_tf_old[NUM_TWELVE_FOLD];

extern int stat_sf[NUM_SEVEN_FOLD];
extern int stat_tf[NUM_TWELVE_FOLD];
extern int stat_sf_p2[NUM_SEVEN_FOLD];
extern int stat_tf_p2[NUM_TWELVE_FOLD];

extern int fail_stat_sf_cur[NUM_SEVEN_FOLD];
extern int fail_stat_tf_cur[NUM_TWELVE_FOLD];

extern int fail_stat_sf_fin[NUM_SEVEN_FOLD];
extern int fail_stat_tf_fin[NUM_TWELVE_FOLD];

extern float data_sf_queue[NUM_SEVEN_FOLD][PERIOD_MA];
extern float data_sf_ma[NUM_SEVEN_FOLD];
extern float data_sf_oldest[NUM_SEVEN_FOLD];

extern float data_tf_queue[NUM_TWELVE_FOLD][PERIOD_MA];
extern float data_tf_ma[NUM_TWELVE_FOLD];
extern float data_tf_oldest[NUM_TWELVE_FOLD];


extern float refPresure_sf[NUM_SEVEN_FOLD];
extern float refPresure_tf[NUM_TWELVE_FOLD];


extern bool stat_2_5_bar;
extern bool stat_7_5_bar;


extern NexText tStatus;
extern NexText tProgress;

extern NexText tResult;


// NexButton(0, 1, "b0") : (page, id, name)

// page0
extern NexButton p0_bDiag;  // button for DIAG (page0)
extern NexButton p0_bDebug;

// page1
extern NexButton p1_bt0;  // button for 2.5bar (page1)
extern NexButton p1_bt1;  // button for 7.5bar (page1)

extern NexButton p1_bt_sf0;  // button for sevenfold_1 (page1)
extern NexButton p1_bt_sf1;  // button for sevenfold_2 (page1)
extern NexButton p1_bt_sf2;  // button for sevenfold_3 (page1)
extern NexButton p1_bt_sf3;  // button for sevenfold_4 (page1)
extern NexButton p1_bt_sf4;  // button for sevenfold_5 (page1)
extern NexButton p1_bt_sf5;  // button for sevenfold_6 (page1)
extern NexButton p1_bt_sf6;  // button for sevenfold_7 (page1)

extern NexButton p1_bt_tf0;  // button for twelvefold_1 (page1)
extern NexButton p1_bt_tf1;  // button for twelvefold_2 (page1)
extern NexButton p1_bt_tf2;  // button for twelvefold_2 (page1)
extern NexButton p1_bt_tf3;  // button for twelvefold_2 (page1)
extern NexButton p1_bt_tf4;  // button for twelvefold_2 (page1)
extern NexButton p1_bt_tf5;  // button for twelvefold_2 (page1)
extern NexButton p1_bt_tf6;  // button for twelvefold_2 (page1)
extern NexButton p1_bt_tf7;  // button for twelvefold_2 (page1)
extern NexButton p1_bt_tf8;  // button for twelvefold_2 (page1)
extern NexButton p1_bt_tf9;  // button for twelvefold_2 (page1)
extern NexButton p1_bt_tf10;  // button for twelvefold_2 (page1)
extern NexButton p1_bt_tf11;  // button for twelvefold_2 (page1)

extern NexButton p1_bChk_All;  // button for Check all (page1)
extern NexButton p1_bClr_All;  // button for Clear all (page1)

extern NexButton p1_b0;  // button for START (page1)
extern NexButton p1_plusMinus0;
extern NexButton p1_plusMinus1;
extern NexButton t_p1_plusMinus0;
extern NexButton t_p1_plusMinus1;



// page2
extern NexButton p2_b0;  // button for Back to main (page2)


// fold button(sf)
extern NexButton p2_bt_sf0;  // button for sevenfold_1 (page2)
extern NexButton p2_bt_sf1;  // button for sevenfold_2 (page2)
extern NexButton p2_bt_sf2;  // button for sevenfold_3 (page2)
extern NexButton p2_bt_sf3;  // button for sevenfold_4 (page2)
extern NexButton p2_bt_sf4;  // button for sevenfold_5 (page2)
extern NexButton p2_bt_sf5;  // button for sevenfold_6 (page2)
extern NexButton p2_bt_sf6;  // button for sevenfold_7 (page2)

// fold button(tf)
extern NexButton p2_bt_tf0;  // button for twelvefold_1 (page2)
extern NexButton p2_bt_tf1;  // button for twelvefold_2 (page2)
extern NexButton p2_bt_tf2;  // button for twelvefold_3 (page2)
extern NexButton p2_bt_tf3;  // button for twelvefold_4 (page2)
extern NexButton p2_bt_tf4;  // button for twelvefold_5 (page2)
extern NexButton p2_bt_tf5;  // button for twelvefold_6 (page2)
extern NexButton p2_bt_tf6;  // button for twelvefold_7 (page2)
extern NexButton p2_bt_tf7;  // button for twelvefold_8 (page2)
extern NexButton p2_bt_tf8;  // button for twelvefold_9 (page2)
extern NexButton p2_bt_tf9;  // button for twelvefold_10 (page2)
extern NexButton p2_bt_tf10;  // button for twelvefold_11 (page2)
extern NexButton p2_bt_tf11;  // button for twelvefold_12 (page2)
extern NexButton p1_b1;  // button for back to main (page1)

#endif