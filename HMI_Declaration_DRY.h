#ifndef myHMILibrary_H
#define myMHILibrary_H

extern uint16_t stat_2_5_bar_dry;
extern uint16_t stat_7_5_bar_dry;

extern uint16_t stat_sf_dry[NUM_SEVEN_FOLD_DRY];
extern uint16_t stat_tf_dry[NUM_TWELVE_FOLD_DRY];


// dry
extern float data_sf_dry[NUM_SEVEN_FOLD_DRY];
extern float data_tf_dry[NUM_TWELVE_FOLD_DRY];

extern float aveRecursive_sf_dry[NUM_SEVEN_FOLD_DRY];
extern float aveRecursive_sf_old_dry[NUM_SEVEN_FOLD_DRY];
extern float aveRecursive_tf_dry[NUM_TWELVE_FOLD_DRY];
extern float aveRecursive_tf_old_dry[NUM_TWELVE_FOLD_DRY];

extern uint16_t stat_sf_p2_dry[NUM_SEVEN_FOLD_DRY];
extern uint16_t stat_tf_p2_dry[NUM_TWELVE_FOLD_DRY];

extern uint16_t fail_stat_sf_cur_dry[NUM_SEVEN_FOLD_DRY];
extern uint16_t fail_stat_tf_cur_dry[NUM_TWELVE_FOLD_DRY];

extern uint16_t fail_stat_sf_fin_dry[NUM_SEVEN_FOLD_DRY];
extern uint16_t fail_stat_tf_fin_dry[NUM_TWELVE_FOLD_DRY];


extern float data_sf_queue_dry[NUM_SEVEN_FOLD_DRY][PERIOD_MA];
extern float data_sf_ma_dry[NUM_SEVEN_FOLD_DRY];
extern float data_sf_oldest_dry[NUM_SEVEN_FOLD_DRY];

extern float data_tf_queue_dry[NUM_TWELVE_FOLD_DRY][PERIOD_MA];
extern float data_tf_ma_dry[NUM_TWELVE_FOLD_DRY];
extern float data_tf_oldest_dry[NUM_TWELVE_FOLD_DRY];


extern float refPresure_sf_dry[NUM_SEVEN_FOLD_DRY];
extern float refPresure_tf_dry[NUM_TWELVE_FOLD_DRY];



extern NexButton p4_bt0;
extern NexButton p4_bt1;

extern NexButton p4_bt_sf0;
extern NexButton p4_bt_sf1;
extern NexButton p4_bt_sf2;
extern NexButton p4_bt_sf3;
extern NexButton p4_bt_sf4;

extern NexButton p4_bt_tf0;
extern NexButton p4_bt_tf1;
extern NexButton p4_bt_tf2;
extern NexButton p4_bt_tf3;
extern NexButton p4_bt_tf4;
extern NexButton p4_bt_tf5;
extern NexButton p4_bt_tf6;
extern NexButton p4_bt_tf7;
extern NexButton p4_bt_tf8;
extern NexButton p4_bt_tf9;
extern NexButton p4_bt_tf10;
extern NexButton p4_bt_tf11;

extern NexButton p4_bChk_All;
extern NexButton p4_bClr_All;

extern NexButton p4_b0;
extern NexButton p5_b0;

extern NexButton p3_b0;

extern NexButton p4_b1;
extern NexButton p4_plusMinus0;
extern NexButton p4_plusMinus1;
extern NexButton t_p4_plusMinus0;
extern NexButton t_p4_plusMinus1;

#endif