#include "Nextion.h"
#include "Common_library.h"


uint16_t stat_sf_dry[NUM_SEVEN_FOLD_DRY] = {};
uint16_t stat_tf_dry[NUM_TWELVE_FOLD_DRY] = {};


uint16_t stat_2_5_bar_dry = 0U;
uint16_t stat_7_5_bar_dry = 0U;

// dry
float data_sf_dry[NUM_SEVEN_FOLD_DRY] = {};
float data_tf_dry[NUM_TWELVE_FOLD_DRY] = {};

float refPresure_sf_dry[NUM_SEVEN_FOLD_DRY] = {};
float refPresure_tf_dry[NUM_TWELVE_FOLD_DRY] = {};

float aveRecursive_sf_dry[NUM_SEVEN_FOLD_DRY] = {};
float aveRecursive_sf_old_dry[NUM_SEVEN_FOLD_DRY] = {};
float aveRecursive_tf_dry[NUM_TWELVE_FOLD_DRY] = {};
float aveRecursive_tf_old_dry[NUM_TWELVE_FOLD_DRY] = {};

uint16_t stat_sf_p2_dry[NUM_SEVEN_FOLD_DRY] = {};
uint16_t stat_tf_p2_dry[NUM_TWELVE_FOLD_DRY] = {};

uint16_t fail_stat_sf_cur_dry[NUM_SEVEN_FOLD_DRY] = {};
uint16_t fail_stat_tf_cur_dry[NUM_TWELVE_FOLD_DRY] = {};

uint16_t fail_stat_sf_fin_dry[NUM_SEVEN_FOLD_DRY] = {};
uint16_t fail_stat_tf_fin_dry[NUM_TWELVE_FOLD_DRY] = {};


float data_sf_queue_dry[NUM_SEVEN_FOLD_DRY][PERIOD_MA] = {};
float data_sf_ma_dry[NUM_SEVEN_FOLD_DRY] = {};
float data_sf_oldest_dry[NUM_SEVEN_FOLD_DRY] = {};

float data_tf_queue_dry[NUM_TWELVE_FOLD_DRY][PERIOD_MA] = {};
float data_tf_ma_dry[NUM_TWELVE_FOLD_DRY] = {};
float data_tf_oldest_dry[NUM_TWELVE_FOLD_DRY] = {};



NexButton p4_bt0 = NexButton(4, 24, "bt0");  // button for 2.5bar (page4)
NexButton p4_bt1 = NexButton(4, 25, "bt1");  // button for 7.5bar (page4)

NexButton p4_bt_sf0 = NexButton(4, 15, "bt_sf0");  // button for sevenfold_13 (page4)
NexButton p4_bt_sf1 = NexButton(4, 16, "bt_sf1");  // button for sevenfold_14 (page4)
NexButton p4_bt_sf2 = NexButton(4, 17, "bt_sf2");  // button for sevenfold_15 (page4)
NexButton p4_bt_sf3 = NexButton(4, 18, "bt_sf3");  // button for sevenfold_16 (page4)
NexButton p4_bt_sf4 = NexButton(4, 19, "bt_sf4");  // button for sevenfold_17 (page4)


NexButton p4_bt_tf0 = NexButton(4, 3, "bt_tf0");  // button for twelvefold_1 (page4)
NexButton p4_bt_tf1 = NexButton(4, 4, "bt_tf1");  // button for twelvefold_2 (page4)
NexButton p4_bt_tf2 = NexButton(4, 5, "bt_tf2");  // button for twelvefold_2 (page4)
NexButton p4_bt_tf3 = NexButton(4, 6, "bt_tf3");  // button for twelvefold_2 (page4)
NexButton p4_bt_tf4 = NexButton(4, 7, "bt_tf4");  // button for twelvefold_2 (page4)
NexButton p4_bt_tf5 = NexButton(4, 8, "bt_tf5");  // button for twelvefold_2 (page4)
NexButton p4_bt_tf6 = NexButton(4, 9, "bt_tf6");  // button for twelvefold_2 (page4)
NexButton p4_bt_tf7 = NexButton(4, 10, "bt_tf7");  // button for twelvefold_2 (page4)
NexButton p4_bt_tf8 = NexButton(4, 11, "bt_tf8");  // button for twelvefold_2 (page4)
NexButton p4_bt_tf9 = NexButton(4, 12, "bt_tf9");  // button for twelvefold_2 (page4)
NexButton p4_bt_tf10 = NexButton(4, 13, "bt_tf10");  // button for twelvefold_2 (page4)
NexButton p4_bt_tf11 = NexButton(4, 14, "bt_tf11");  // button for twelvefold_2 (page4)

NexButton p4_bChk_All = NexButton(4, 28, "bChk_All");  // button for Check all (page4)
NexButton p4_bClr_All = NexButton(4, 29, "bClr_All");  // button for Clear all (page4)

NexButton p4_b0 = NexButton(4, 27, "b0");  // button for START (page4)

NexButton p5_b0 = NexButton(5, 21, "b0");  // button for back to main (page5)

NexButton p3_b0 = NexButton(3, 2, "b0");  // button for back to main (page3)

NexButton p4_b1 = NexButton(4, 39, "b1");  // button for back to main (page4)
NexButton p4_plusMinus0 = NexButton(4, 33, "p4_plusMinus0");  // button for -5 Stbl (page4)
NexButton p4_plusMinus1 = NexButton(4, 34, "p4_plusMinus1");  // button for +5 Stbl (page4)
NexButton t_p4_plusMinus0 = NexButton(4, 37, "t_p4_plusMinus0");  // button for -10 test time (page4)
NexButton t_p4_plusMinus1 = NexButton(4, 38, "t_p4_plusMinus1");  // button for +10 test time (page4)

