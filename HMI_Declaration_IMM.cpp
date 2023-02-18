#include "Nextion.h"
#include "Common_library.h"


float data_sf[NUM_SEVEN_FOLD] = {};
float data_tf[NUM_TWELVE_FOLD] = {};

float refPresure_sf[NUM_SEVEN_FOLD] = {};
float refPresure_tf[NUM_TWELVE_FOLD] = {};

float aveRecursive_sf[NUM_SEVEN_FOLD] = {};
float aveRecursive_sf_old[NUM_SEVEN_FOLD] = {};
float aveRecursive_tf[NUM_TWELVE_FOLD] = {};
float aveRecursive_tf_old[NUM_TWELVE_FOLD] = {};

int stat_sf[NUM_SEVEN_FOLD] = {};
int stat_tf[NUM_TWELVE_FOLD] = {};
int stat_sf_p2[NUM_SEVEN_FOLD] = {};
int stat_tf_p2[NUM_TWELVE_FOLD] = {};

int fail_stat_sf_cur[NUM_SEVEN_FOLD] = {};
int fail_stat_tf_cur[NUM_TWELVE_FOLD] = {};

int fail_stat_sf_fin[NUM_SEVEN_FOLD] = {};
int fail_stat_tf_fin[NUM_TWELVE_FOLD] = {};

float data_sf_queue[NUM_SEVEN_FOLD][PERIOD_MA] = {};
float data_sf_ma[NUM_SEVEN_FOLD] = {};
float data_sf_oldest[NUM_SEVEN_FOLD] = {};

float data_tf_queue[NUM_TWELVE_FOLD][PERIOD_MA] = {};
float data_tf_ma[NUM_TWELVE_FOLD] = {};
float data_tf_oldest[NUM_TWELVE_FOLD] = {};


bool stat_2_5_bar = 0;

bool stat_7_5_bar = 0;

NexText tStatus = NexText(2, 67, "tStatus");
NexText tProgress = NexText(2, 65, "tProgress");

NexText tResult = NexText(2, 61, "tResult");


/***** NexButton(0, 1, "b0") : (page, id, name) *****/
// page0
NexButton p0_bDiag = NexButton(0, 6, "p0_bDiag");  // button for DIAG (page0)
NexButton p0_bDebug = NexButton(0, 9, "p0_bDebug");  // button for Debug (page0)

// page1
NexButton p1_bt0 = NexButton(1, 2, "bt0");  // button for 2.5bar (page1)
NexButton p1_bt1 = NexButton(1, 3, "bt1");  // button for 7.5bar (page1)

NexButton p1_bt_sf0 = NexButton(1, 15, "bt_sf0");  // button for sevenfold_1 (page1)
NexButton p1_bt_sf1 = NexButton(1, 14, "bt_sf1");  // button for sevenfold_2 (page1)
NexButton p1_bt_sf2 = NexButton(1, 13, "bt_sf2");  // button for sevenfold_3 (page1)
NexButton p1_bt_sf3 = NexButton(1, 9, "bt_sf3");  // button for sevenfold_4 (page1)
NexButton p1_bt_sf4 = NexButton(1, 12, "bt_sf4");  // button for sevenfold_5 (page1)
NexButton p1_bt_sf5 = NexButton(1, 10, "bt_sf5");  // button for sevenfold_6 (page1)
NexButton p1_bt_sf6 = NexButton(1, 11, "bt_sf6");  // button for sevenfold_7 (page1)


NexButton p1_bt_tf0 = NexButton(1, 16, "bt_tf0");  // button for twelvefold_1 (page1)
NexButton p1_bt_tf1 = NexButton(1, 21, "bt_tf1");  // button for twelvefold_2 (page1)
NexButton p1_bt_tf2 = NexButton(1, 17, "bt_tf2");  // button for twelvefold_2 (page1)
NexButton p1_bt_tf3 = NexButton(1, 22, "bt_tf3");  // button for twelvefold_2 (page1)
NexButton p1_bt_tf4 = NexButton(1, 18, "bt_tf4");  // button for twelvefold_2 (page1)
NexButton p1_bt_tf5 = NexButton(1, 19, "bt_tf5");  // button for twelvefold_2 (page1)
NexButton p1_bt_tf6 = NexButton(1, 24, "bt_tf6");  // button for twelvefold_2 (page1)
NexButton p1_bt_tf7 = NexButton(1, 20, "bt_tf7");  // button for twelvefold_2 (page1)
NexButton p1_bt_tf8 = NexButton(1, 25, "bt_tf8");  // button for twelvefold_2 (page1)
NexButton p1_bt_tf9 = NexButton(1, 23, "bt_tf9");  // button for twelvefold_2 (page1)
NexButton p1_bt_tf10 = NexButton(1, 27, "bt_tf10");  // button for twelvefold_2 (page1)
NexButton p1_bt_tf11 = NexButton(1, 26, "bt_tf11");  // button for twelvefold_2 (page1)

NexButton p1_bChk_All = NexButton(1, 6, "bChk_All");  // button for Check all (page1)
NexButton p1_bClr_All = NexButton(1, 7, "bClr_All");  // button for Clear all (page1)

NexButton p1_b0 = NexButton(1, 4, "b0");  // button for START (page1)
NexButton p1_plusMinus0 = NexButton(1, 36, "p1_plusMinus0");  // button for -5 Stbl (page1)
NexButton p1_plusMinus1 = NexButton(1, 37, "p1_plusMinus1");  // button for +5 Stbl (page1)
NexButton t_p1_plusMinus0 = NexButton(1, 40, "t_p1_plusMinus0");  // button for -5 test time (page1)
NexButton t_p1_plusMinus1 = NexButton(1, 41, "t_p1_plusMinus1");  // button for +5 test time (page1)



// page2
NexButton p2_b0 = NexButton(2, 58, "b0");  // button for Back to main (page2)


// fold button(sf)
NexButton p2_bt_sf0 = NexButton(2, 32, "bt_sf0");  // button for sevenfold_1 (page2)
NexButton p2_bt_sf1 = NexButton(2, 31, "bt_sf1");  // button for sevenfold_2 (page2)
NexButton p2_bt_sf2 = NexButton(2, 20, "bt_sf2");  // button for sevenfold_3 (page2)
NexButton p2_bt_sf3 = NexButton(2, 16, "bt_sf3");  // button for sevenfold_4 (page2)
NexButton p2_bt_sf4 = NexButton(2, 19, "bt_sf4");  // button for sevenfold_5 (page2)
NexButton p2_bt_sf5 = NexButton(2, 17, "bt_sf5");  // button for sevenfold_6 (page2)
NexButton p2_bt_sf6 = NexButton(2, 18, "bt_sf6");  // button for sevenfold_7 (page2)

// fold button(tf)
NexButton p2_bt_tf0 = NexButton(2, 33, "bt_tf0");  // button for twelvefold_1 (page2)
NexButton p2_bt_tf1 = NexButton(2, 24, "bt_tf1");  // button for twelvefold_2 (page2)
NexButton p2_bt_tf2 = NexButton(2, 34, "bt_tf2");  // button for twelvefold_3 (page2)
NexButton p2_bt_tf3 = NexButton(2, 25, "bt_tf3");  // button for twelvefold_4 (page2)
NexButton p2_bt_tf4 = NexButton(2, 21, "bt_tf4");  // button for twelvefold_5 (page2)
NexButton p2_bt_tf5 = NexButton(2, 22, "bt_tf5");  // button for twelvefold_6 (page2)
NexButton p2_bt_tf6 = NexButton(2, 27, "bt_tf6");  // button for twelvefold_7 (page2)
NexButton p2_bt_tf7 = NexButton(2, 23, "bt_tf7");  // button for twelvefold_8 (page2)
NexButton p2_bt_tf8 = NexButton(2, 28, "bt_tf8");  // button for twelvefold_9 (page2)
NexButton p2_bt_tf9 = NexButton(2, 26, "bt_tf9");  // button for twelvefold_10 (page2)
NexButton p2_bt_tf10 = NexButton(2, 30, "bt_tf10");  // button for twelvefold_11 (page2)
NexButton p2_bt_tf11 = NexButton(2, 29, "bt_tf11");  // button for twelvefold_12 (page2)

NexButton p1_b1 = NexButton(1, 33, "b0");  // button for back to main (page1)



