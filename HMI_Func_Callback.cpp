#include "Nextion.h"
#include "Common_library.h"
#include <MsTimer2.h>
#include "HMI_Declaration_DRY.h"
#include "HMI_Declaration_IMM.h"
#include "HMI_Func_DISP.h"

#include <ModbusMaster232.h> 

NexTouch *nex_listen_list[] = {&p0_bDiag, &p0_bDebug,
                              &p1_bt0, &p1_bt1, &p1_bt_sf0, &p1_bt_sf1, &p1_bt_sf2, &p1_bt_sf3, &p1_bt_sf4, &p1_bt_sf5, &p1_bt_sf6,
                              &p1_bt_tf0, &p1_bt_tf1, &p1_bt_tf2, &p1_bt_tf3, &p1_bt_tf4, &p1_bt_tf5, &p1_bt_tf6, &p1_bt_tf7, &p1_bt_tf8, &p1_bt_tf9, &p1_bt_tf10, &p1_bt_tf11,
                              &p1_bChk_All, &p1_bClr_All, &p1_b0, &p1_b1, &p1_plusMinus0, &p1_plusMinus1, &t_p1_plusMinus0, &t_p1_plusMinus1, &p2_b0,
                              &p2_bt_sf0, &p2_bt_sf1, &p2_bt_sf2, &p2_bt_sf3, &p2_bt_sf4, &p2_bt_sf5, &p2_bt_sf6,
                              &p2_bt_tf0, &p2_bt_tf1, &p2_bt_tf2, &p2_bt_tf3, &p2_bt_tf4, &p2_bt_tf5, &p2_bt_tf6, &p2_bt_tf7, &p2_bt_tf8, &p2_bt_tf9, &p2_bt_tf10, &p2_bt_tf11,
                              &p3_b0,
                              &p4_bt0, &p4_bt1, &p4_bt_sf0, &p4_bt_sf1, &p4_bt_sf2, &p4_bt_sf3, &p4_bt_sf4,
                              &p4_bt_tf0, &p4_bt_tf1, &p4_bt_tf2, &p4_bt_tf3, &p4_bt_tf4, &p4_bt_tf5, &p4_bt_tf6, &p4_bt_tf7, &p4_bt_tf8, &p4_bt_tf9, &p4_bt_tf10, &p4_bt_tf11,
                              &p4_bChk_All, &p4_bClr_All, &p4_b0, &p4_b1, &p4_plusMinus0, &p4_plusMinus1, &t_p4_plusMinus0, &t_p4_plusMinus1,
                              &p5_b0,
                              NULL};
                              
                              

// Start button
void p4_b0_Callback(void *ptr){  
  digitalWrite(LED_PIN, HIGH);  
  
  // Caluation sum of the number of seven/three folds
  gl_sum_fold = int_cal_sum_arr();
  //gl_sum_fold = 1;
  // if any button of folds is on and any pressure check button on, flag start = 1
  if((stat_2_5_bar_dry == 1 || stat_7_5_bar_dry == 1) && gl_sum_fold != 0)
  {
    flag_start_DRY = 1;
  }

  // if flag_start_DRY = 1, timer is on > timer start
  if(flag_start_DRY == 1)
  {
    MsTimer2::start();
  }
}




/************************ Callback function *****************************/

// Diagnostic
void p0_bDiag_Callback(void *ptr){

  for (int idx = 0U ; idx < NUM_SEVEN_FOLD; idx++)
  {
    stat_sf[idx] = 1U;
  }
  for (int idx = 0U ; idx < NUM_TWELVE_FOLD; idx++)
  {
    stat_tf[idx] = 1U;
  }
  
  flag_start_IMM = 1U;
  flag_start_DRY = 1U;


  MsTimer2::start();
  time_cur = millis();
}



// Debug
void p0_bDebug_Callback(void *ptr){
  gl_DEBUG_MODE = !gl_DEBUG_MODE;


  Serial_Mon("gl_DEBUG_MODE", gl_DEBUG_MODE);
  
}


// Pressure check button setting (2.5/7.5 Bar)
void p1_bt0_Callback(void *ptr){
  digitalWrite(LED_PIN, HIGH); 
  thr_pressure_each_fin = THR_PRESSURE_EACH_FIN_2_5; // threshold for 2.5bar
  thr_pressure_each_ma = THR_PRESSURE_EACH_MA_2_5;
  thr_pressure_all_fin = THR_PRESSURE_ALL_FIN_2_5;
  val_regulatorInput = ANALOG_INPUT_2_5;
  stat_2_5_bar = !stat_2_5_bar;
  stat_7_5_bar = 0;

  if(stat_2_5_bar == 1U)
  {
    gl_ref_bar = REF_BAR_2_5;
    gl_regOut_target = REGOUT_TAR_2_5;
  }
  
  Serial_Mon("val_regulatorInput : ", val_regulatorInput);
  Serial_Mon("gl_ref_bar : ", gl_ref_bar);
  Serial_Mon("gl_regOut_target : ", gl_regOut_target);
}

void p1_bt1_Callback(void *ptr){
  digitalWrite(LED_PIN, LOW);
  thr_pressure_each_fin = THR_PRESSURE_EACH_FIN_7_5; // threshold for 7.5bar
  thr_pressure_each_ma = THR_PRESSURE_EACH_MA_7_5;
  thr_pressure_all_fin = THR_PRESSURE_ALL_FIN_7_5;
  val_regulatorInput = ANALOG_INPUT_7_5;
  stat_7_5_bar = !stat_7_5_bar;
  stat_2_5_bar = 0;

  if(stat_7_5_bar == 1U)
  {
    gl_ref_bar = REF_BAR_7_5;
    gl_regOut_target = REGOUT_TAR_7_5;
  }

  Serial_Mon("val_regulatorInput : ", val_regulatorInput);
  Serial_Mon("gl_ref_bar : ", gl_ref_bar);
  Serial_Mon("gl_regOut_target : ", gl_regOut_target);
}


// Seven/three fold button setting(check each) 
void p1_bt_sf0_Callback(void *ptr){
  stat_sf[0] = !stat_sf[0];
}
void p1_bt_sf1_Callback(void *ptr){
  stat_sf[1] = !stat_sf[1];
}
void p1_bt_sf2_Callback(void *ptr){
  stat_sf[2] = !stat_sf[2];
}
void p1_bt_sf3_Callback(void *ptr){
  stat_sf[3] = !stat_sf[3];
}
void p1_bt_sf4_Callback(void *ptr){
  stat_sf[4] = !stat_sf[4];
}
void p1_bt_sf5_Callback(void *ptr){
  stat_sf[5] = !stat_sf[5];
}
void p1_bt_sf6_Callback(void *ptr){
  stat_sf[6] = !stat_sf[6];
}


void p1_bt_tf0_Callback(void *ptr){
  stat_tf[0] = !stat_tf[0];
}
void p1_bt_tf1_Callback(void *ptr){
  stat_tf[1] = !stat_tf[1];
}
void p1_bt_tf2_Callback(void *ptr){
  stat_tf[2] = !stat_tf[2];
}
void p1_bt_tf3_Callback(void *ptr){
  stat_tf[3] = !stat_tf[3];
}
void p1_bt_tf4_Callback(void *ptr){
  stat_tf[4] = !stat_tf[4];
}
void p1_bt_tf5_Callback(void *ptr){
  stat_tf[5] = !stat_tf[5];
}
void p1_bt_tf6_Callback(void *ptr){
  stat_tf[6] = !stat_tf[6];
}
void p1_bt_tf7_Callback(void *ptr){
  stat_tf[7] = !stat_tf[7];
}
void p1_bt_tf8_Callback(void *ptr){
  stat_tf[8] = !stat_tf[8];
}
void p1_bt_tf9_Callback(void *ptr){
  stat_tf[9] = !stat_tf[9];
}
void p1_bt_tf10_Callback(void *ptr){
  stat_tf[10] = !stat_tf[10];
}
void p1_bt_tf11_Callback(void *ptr){
  stat_tf[11] = !stat_tf[11];
}

// Seven/three fold button setting(check all) 
void p1_bChk_All_Callback(void *ptr){
  for (int idx = 0U ; idx < NUM_SEVEN_FOLD; idx++)
  {
    stat_sf[idx] = 1;
  }
  for (int idx = 0U ; idx < NUM_TWELVE_FOLD; idx++)
  {
    stat_tf[idx] = 1;
  }
}

// Seven/three fold button setting(clear all) 
void p1_bClr_All_Callback(void *ptr){
  for (int idx = 0U ; idx < NUM_SEVEN_FOLD; idx++)
  {
    stat_sf[idx] = 0;
  }
  for (int idx = 0U ; idx < NUM_TWELVE_FOLD; idx++)
  {
    stat_tf[idx] = 0;
  }
}


// Start button
void p1_b0_Callback(void *ptr){  
  digitalWrite(LED_PIN, HIGH);  
  // Caluation sum of the number of seven/three folds
  gl_sum_fold = int_cal_sum_arr();
  //gl_sum_fold=1;
  Serial_Mon("gl_sum_fold :", gl_sum_fold);

  // if any button of folds is on and any pressure check button on, flag start = 1
  if((stat_2_5_bar == 1 || stat_7_5_bar == 1) && gl_sum_fold != 0)
  {
    flag_start_IMM = 1;
  }

  // if flag_start_IMM = 1, timer is on > timer start
  if(flag_start_IMM == 1)
  {
    for (int idx = 0U ; idx < NUM_SEVEN_FOLD; idx++)
    {
      if(stat_sf[idx] == 0)
      {
        sendData2HMI_del_button("sf", idx);
        sendData2HMI_on_nobutton("sf", idx);
      }      
    }
    for (int idx = 0U ; idx < NUM_TWELVE_FOLD; idx++)
    {
      if(stat_tf[idx] == 0)
      {
        sendData2HMI_del_button("tf", idx);
        sendData2HMI_on_nobutton("tf", idx);
      }
    }
    flag_serialHMI = OFF;
    MsTimer2::start();
  }
}
// Button(Back to main)
void p2_b0_Callback(void *ptr){
  
  digitalWrite(LED_PIN, LOW);

  flag_toMain = 1U;
}


void p2_bt_sf0_Callback(void *ptr){
  for (int idx = 0U ; idx < NUM_SEVEN_FOLD; idx++)
  {
    if(idx == 0U)
    {
      stat_sf_p2[idx] = 1;
    }
    else
    {
      stat_sf_p2[idx] = 0;
    }
  }

  for (int idx = 0U ; idx < NUM_TWELVE_FOLD; idx++)
  {
    stat_tf_p2[idx] = 0;
  }
}

void p2_bt_sf1_Callback(void *ptr){
  for (int idx = 0U ; idx < NUM_SEVEN_FOLD; idx++)
  {
    if(idx == 1U)
    {
      stat_sf_p2[idx] = 1;
    }
    else
    {
      stat_sf_p2[idx] = 0;
    }
  }
  for (int idx = 0U ; idx < NUM_TWELVE_FOLD; idx++)
  {
    stat_tf_p2[idx] = 0;
  }
}
void p2_bt_sf2_Callback(void *ptr){
  for (int idx = 0U ; idx < NUM_SEVEN_FOLD; idx++)
  {
    if(idx == 2U)
    {
      stat_sf_p2[idx] = 1;
    }
    else
    {
      stat_sf_p2[idx] = 0;
    }
  }
  for (int idx = 0U ; idx < NUM_TWELVE_FOLD; idx++)
  {
    stat_tf_p2[idx] = 0;
  }
}
void p2_bt_sf3_Callback(void *ptr){
  for (int idx = 0U ; idx < NUM_SEVEN_FOLD; idx++)
  {
    if(idx == 3U)
    {
      stat_sf_p2[idx] = 1;
    }
    else
    {
      stat_sf_p2[idx] = 0;
    }
  }
  for (int idx = 0U ; idx < NUM_TWELVE_FOLD; idx++)
  {
    stat_tf_p2[idx] = 0;
  }
}
void p2_bt_sf4_Callback(void *ptr){
  for (int idx = 0U ; idx < NUM_SEVEN_FOLD; idx++)
  {
    if(idx == 4U)
    {
      stat_sf_p2[idx] = 1;
    }
    else
    {
      stat_sf_p2[idx] = 0;
    }
  }
  for (int idx = 0U ; idx < NUM_TWELVE_FOLD; idx++)
  {
    stat_tf_p2[idx] = 0;
  }
}
void p2_bt_sf5_Callback(void *ptr){
  for (int idx = 0U ; idx < NUM_SEVEN_FOLD; idx++)
  {
    if(idx == 5U)
    {
      stat_sf_p2[idx] = 1;
    }
    else
    {
      stat_sf_p2[idx] = 0;
    }
  }
  for (int idx = 0U ; idx < NUM_TWELVE_FOLD; idx++)
  {
    stat_tf_p2[idx] = 0;
  }
}
void p2_bt_sf6_Callback(void *ptr){
  for (int idx = 0U ; idx < NUM_SEVEN_FOLD; idx++)
  {
    if(idx == 6U)
    {
      stat_sf_p2[idx] = 1;
    }
    else
    {
      stat_sf_p2[idx] = 0;
    }
  }
  for (int idx = 0U ; idx < NUM_TWELVE_FOLD; idx++)
  {
    stat_tf_p2[idx] = 0;
  }
}



void p2_bt_tf0_Callback(void *ptr){
  for (int idx = 0U ; idx < NUM_TWELVE_FOLD; idx++)
  {
    if(idx == 0U)
    {
      stat_tf_p2[idx] = 1;
    }
    else
    {
      stat_tf_p2[idx] = 0;
    }
  }
  for (int idx = 0U ; idx < NUM_SEVEN_FOLD; idx++)
  {
    stat_sf_p2[idx] = 0;
  }
}

void p2_bt_tf1_Callback(void *ptr){
  for (int idx = 0U ; idx < NUM_TWELVE_FOLD; idx++)
  {
    if(idx == 1U)
    {
      stat_tf_p2[idx] = 1;
    }
    else
    {
      stat_tf_p2[idx] = 0;
    }
  }
  for (int idx = 0U ; idx < NUM_SEVEN_FOLD; idx++)
  {
    stat_sf_p2[idx] = 0;
  }
}
void p2_bt_tf2_Callback(void *ptr){
  for (int idx = 0U ; idx < NUM_TWELVE_FOLD; idx++)
  {
    if(idx == 2U)
    {
      stat_tf_p2[idx] = 1;
    }
    else
    {
      stat_tf_p2[idx] = 0;
    }
  }
  for (int idx = 0U ; idx < NUM_SEVEN_FOLD; idx++)
  {
    stat_sf_p2[idx] = 0;
  }
}
void p2_bt_tf3_Callback(void *ptr){
  for (int idx = 0U ; idx < NUM_TWELVE_FOLD; idx++)
  {
    if(idx == 3U)
    {
      stat_tf_p2[idx] = 1;
    }
    else
    {
      stat_tf_p2[idx] = 0;
    }
  }
  for (int idx = 0U ; idx < NUM_SEVEN_FOLD; idx++)
  {
    stat_sf_p2[idx] = 0;
  }
}
void p2_bt_tf4_Callback(void *ptr){
  for (int idx = 0U ; idx < NUM_TWELVE_FOLD; idx++)
  {
    if(idx == 4U)
    {
      stat_tf_p2[idx] = 1;
    }
    else
    {
      stat_tf_p2[idx] = 0;
    }
  }
  for (int idx = 0U ; idx < NUM_SEVEN_FOLD; idx++)
  {
    stat_sf_p2[idx] = 0;
  }
}
void p2_bt_tf5_Callback(void *ptr){
  for (int idx = 0U ; idx < NUM_TWELVE_FOLD; idx++)
  {
    if(idx == 5U)
    {
      stat_tf_p2[idx] = 1;
    }
    else
    {
      stat_tf_p2[idx] = 0;
    }
  }
  for (int idx = 0U ; idx < NUM_SEVEN_FOLD; idx++)
  {
    stat_sf_p2[idx] = 0;
  }
}
void p2_bt_tf6_Callback(void *ptr){
  for (int idx = 0U ; idx < NUM_TWELVE_FOLD; idx++)
  {
    if(idx == 6U)
    {
      stat_tf_p2[idx] = 1;
    }
    else
    {
      stat_tf_p2[idx] = 0;
    }
  }
  for (int idx = 0U ; idx < NUM_SEVEN_FOLD; idx++)
  {
    stat_sf_p2[idx] = 0;
  }
}
void p2_bt_tf7_Callback(void *ptr){
  for (int idx = 0U ; idx < NUM_TWELVE_FOLD; idx++)
  {
    if(idx == 7U)
    {
      stat_tf_p2[idx] = 1;
    }
    else
    {
      stat_tf_p2[idx] = 0;
    }
  }
  for (int idx = 0U ; idx < NUM_SEVEN_FOLD; idx++)
  {
    stat_sf_p2[idx] = 0;
  }
}
void p2_bt_tf8_Callback(void *ptr){
  for (int idx = 0U ; idx < NUM_TWELVE_FOLD; idx++)
  {
    if(idx == 8U)
    {
      stat_tf_p2[idx] = 1;
    }
    else
    {
      stat_tf_p2[idx] = 0;
    }
  }
  for (int idx = 0U ; idx < NUM_SEVEN_FOLD; idx++)
  {
    stat_sf_p2[idx] = 0;
  }
}
void p2_bt_tf9_Callback(void *ptr){
  for (int idx = 0U ; idx < NUM_TWELVE_FOLD; idx++)
  {
    if(idx == 9U)
    {
      stat_tf_p2[idx] = 1;
    }
    else
    {
      stat_tf_p2[idx] = 0;
    }
  }
  for (int idx = 0U ; idx < NUM_SEVEN_FOLD; idx++)
  {
    stat_sf_p2[idx] = 0;
  }
}
void p2_bt_tf10_Callback(void *ptr){
  for (int idx = 0U ; idx < NUM_TWELVE_FOLD; idx++)
  {
    if(idx == 10U)
    {
      stat_tf_p2[idx] = 1;
    }
    else
    {
      stat_tf_p2[idx] = 0;
    }
  }
}


void p2_bt_tf11_Callback(void *ptr){
  for (int idx = 0U ; idx < NUM_TWELVE_FOLD; idx++)
  {
    if(idx == 11U)
    {
      stat_tf_p2[idx] = 1;
    }
    else
    {
      stat_tf_p2[idx] = 0;
    }
  }
}

//page3 callback

// Pressure check button setting (2.5/7.5 Bar)
void p4_bt0_Callback(void *ptr){
  digitalWrite(LED_PIN, HIGH); 
  thr_pressure_each_fin = THR_PRESSURE_EACH_FIN_2_5; // threshold for 2.5bar
  thr_pressure_each_ma = THR_PRESSURE_EACH_MA_2_5;
  thr_pressure_all_fin = THR_PRESSURE_ALL_FIN_2_5;
  val_regulatorInput = ANALOG_INPUT_2_5;
  stat_2_5_bar_dry = !stat_2_5_bar_dry;
  stat_7_5_bar_dry = 0;

  Serial_Mon("thr_pressure_each_fin", thr_pressure_each_fin);
  Serial_Mon("thr_pressure_each_ma", thr_pressure_each_ma);
  Serial_Mon("thr_pressure_all_fin", thr_pressure_all_fin);

  if(stat_2_5_bar_dry == 1U)
  {
    gl_ref_bar = REF_BAR_2_5;
    gl_regOut_target = REGOUT_TAR_2_5;
  }
  
  Serial_Mon("val_regulatorInput : ", val_regulatorInput);
  Serial_Mon("gl_ref_bar : ", gl_ref_bar);
  Serial_Mon("gl_regOut_target : ", gl_regOut_target);
  
}

void p4_bt1_Callback(void *ptr){
  digitalWrite(LED_PIN, LOW);
  thr_pressure_each_fin = THR_PRESSURE_EACH_FIN_7_5; // threshold for 7.5bar
  thr_pressure_each_ma = THR_PRESSURE_EACH_MA_7_5;
  thr_pressure_all_fin = THR_PRESSURE_ALL_FIN_7_5;
  val_regulatorInput = ANALOG_INPUT_7_5;
  stat_7_5_bar_dry = !stat_7_5_bar_dry;
  stat_2_5_bar_dry = 0;

  Serial_Mon("thr_pressure_each_fin", thr_pressure_each_fin);
  Serial_Mon("thr_pressure_each_ma", thr_pressure_each_ma);
  Serial_Mon("thr_pressure_all_fin", thr_pressure_all_fin);

  if(stat_7_5_bar_dry == 1U)
  {
    gl_ref_bar = REF_BAR_7_5;
    gl_regOut_target = REGOUT_TAR_7_5;
  }

  Serial_Mon("val_regulatorInput : ", val_regulatorInput);
  Serial_Mon("gl_ref_bar : ", gl_ref_bar);
  Serial_Mon("gl_regOut_target : ", gl_regOut_target);
}


// Seven/three fold button setting(check each) 
void p4_bt_sf0_Callback(void *ptr){
  stat_sf_dry[0] = !stat_sf_dry[0];
}
void p4_bt_sf1_Callback(void *ptr){
  stat_sf_dry[1] = !stat_sf_dry[1];
}
void p4_bt_sf2_Callback(void *ptr){
  stat_sf_dry[2] = !stat_sf_dry[2];
}
void p4_bt_sf3_Callback(void *ptr){
  stat_sf_dry[3] = !stat_sf_dry[3];
}
void p4_bt_sf4_Callback(void *ptr){
  stat_sf_dry[4] = !stat_sf_dry[4];
}


void p4_bt_tf0_Callback(void *ptr){
  stat_tf_dry[0] = !stat_tf_dry[0];
}
void p4_bt_tf1_Callback(void *ptr){
  stat_tf_dry[1] = !stat_tf_dry[1];
}
void p4_bt_tf2_Callback(void *ptr){
  stat_tf_dry[2] = !stat_tf_dry[2];
}
void p4_bt_tf3_Callback(void *ptr){
  stat_tf_dry[3] = !stat_tf_dry[3];
}
void p4_bt_tf4_Callback(void *ptr){
  stat_tf_dry[4] = !stat_tf_dry[4];
}
void p4_bt_tf5_Callback(void *ptr){
  stat_tf_dry[5] = !stat_tf_dry[5];
}
void p4_bt_tf6_Callback(void *ptr){
  stat_tf_dry[6] = !stat_tf_dry[6];
}
void p4_bt_tf7_Callback(void *ptr){
  stat_tf_dry[7] = !stat_tf_dry[7];
}
void p4_bt_tf8_Callback(void *ptr){
  stat_tf_dry[8] = !stat_tf_dry[8];
}
void p4_bt_tf9_Callback(void *ptr){
  stat_tf_dry[9] = !stat_tf_dry[9];
}
void p4_bt_tf10_Callback(void *ptr){
  stat_tf_dry[10] = !stat_tf_dry[10];
}
void p4_bt_tf11_Callback(void *ptr){
  stat_tf_dry[11] = !stat_tf_dry[11];
}

// Seven/three fold button setting(check all) 
void p4_bChk_All_Callback(void *ptr){
  for (int idx = 0U ; idx < NUM_SEVEN_FOLD_DRY; idx++)
  {
    stat_sf_dry[idx] = 1;
  }
  for (int idx = 0U ; idx < NUM_TWELVE_FOLD_DRY; idx++)
  {
    stat_tf_dry[idx] = 1;
  }
}

// Seven/three fold button setting(clear all) 
void p4_bClr_All_Callback(void *ptr){
  for (int idx = 0U ; idx < NUM_SEVEN_FOLD_DRY; idx++)
  {
    stat_sf_dry[idx] = 0;
  }
  for (int idx = 0U ; idx < NUM_TWELVE_FOLD_DRY; idx++)
  {
    stat_tf_dry[idx] = 0;
  }
}

// Button(Back to main)
void p5_b0_Callback(void *ptr){
  
  digitalWrite(LED_PIN, LOW);

  flag_toMain = 1U;
  Serial_Mon("flag_toMain", flag_toMain);
}

void p3_b0_Callback(void *ptr){
  digitalWrite(LED_PIN, LOW);

  flag_toMain = 1U;
  Serial_Mon("flag_toMain", flag_toMain);
}

// Back to main (page1)
void p1_b1_Callback(void *ptr){
  digitalWrite(LED_PIN, LOW);

  flag_toMain_setting = 1U;
  Serial_Mon("flag_toMain_setting", flag_toMain_setting);
}

// stbl time -5 (page1)
void p1_plusMinus0_Callback(void *ptr){
  wait_time_stbl_ms = wait_time_stbl_ms - 5000;
  if(wait_time_stbl_ms < 10000)
  {
    wait_time_stbl_ms = 10000;
  }
  Serial_Mon("wait_time_stbl_ms", wait_time_stbl_ms);
}

// stbl time +5 (page1)
void p1_plusMinus1_Callback(void *ptr){
  wait_time_stbl_ms = wait_time_stbl_ms + 5000;
  if(wait_time_stbl_ms > 900000)
  {
    wait_time_stbl_ms = 900000;
  }
  Serial_Mon("wait_time_stbl_ms", wait_time_stbl_ms);
}

// test time -10 (page1)
void t_p1_plusMinus0_Callback(void *ptr){
  wait_time_test_cnt_500ms = wait_time_test_cnt_500ms - 20;
  if(wait_time_test_cnt_500ms < 20)
  {
    wait_time_test_cnt_500ms = 20;
  }
  Serial_Mon("wait_time_test_cnt_500ms", wait_time_test_cnt_500ms);
}

// test time +10 (page1)
void t_p1_plusMinus1_Callback(void *ptr){
  wait_time_test_cnt_500ms = wait_time_test_cnt_500ms + 20;
  if(wait_time_test_cnt_500ms > 240)
  {
    wait_time_test_cnt_500ms = 240;
  }
  Serial_Mon("wait_time_test_cnt_500ms", wait_time_test_cnt_500ms);
}





// Back to main (page4, DRY)
void p4_b1_Callback(void *ptr){
  digitalWrite(LED_PIN, LOW);

  flag_toMain_setting = 1U;
  Serial_Mon("flag_toMain_setting", flag_toMain_setting);
}

// stbl time -5 (page4, DRY)
void p4_plusMinus0_Callback(void *ptr){
  wait_time_stbl_ms = wait_time_stbl_ms - 5000;
  if(wait_time_stbl_ms < 10000)
  {
    wait_time_stbl_ms = 10000;
  }
  Serial_Mon("wait_time_stbl_ms", wait_time_stbl_ms);
}

// stbl time +5 (page4, DRY)
void p4_plusMinus1_Callback(void *ptr){
  wait_time_stbl_ms = wait_time_stbl_ms + 5000;
  if(wait_time_stbl_ms > 30000)
  {
    wait_time_stbl_ms = 30000;
  }
  Serial_Mon("wait_time_stbl_ms", wait_time_stbl_ms);
}

// test time -10 (page4, DRY)
void t_p4_plusMinus0_Callback(void *ptr){
  wait_time_test_cnt_500ms = wait_time_test_cnt_500ms - 20;
  if(wait_time_test_cnt_500ms < 20)
  {
    wait_time_test_cnt_500ms = 20;
  }
  Serial_Mon("wait_time_test_cnt_500ms", wait_time_test_cnt_500ms);
}

// test time +10 (page4, DRY)
void t_p4_plusMinus1_Callback(void *ptr){
  wait_time_test_cnt_500ms = wait_time_test_cnt_500ms + 20;
  if(wait_time_test_cnt_500ms > 240)
  {
    wait_time_test_cnt_500ms = 240;
  }
  Serial_Mon("wait_time_test_cnt_500ms", wait_time_test_cnt_500ms);
}