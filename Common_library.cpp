#include "Nextion.h"
#include "Common_library.h"
#include <MsTimer2.h>
#include "HMI_Declaration_DRY.h"
#include "HMI_Declaration_IMM.h"
#include "HMI_Func_DISP.h"
#include "HMI_Func_Callback.h"
#include "HW_Func.h"
#include <ModbusMaster232.h> 

//Global varibles
float arr_pSensor_conversionFactor_sf[NUM_SEVEN_FOLD]  = { 63.04019, 62.72833, 62.56712, 62.78131, 62.87944, 62.86849, 62.88477 };
float arr_pSensor_conversionFactor_tf[NUM_TWELVE_FOLD] = { 62.42546, 62.52118, 62.44877, 62.42376, 62.51161, 62.84930, 62.74467, 62.64123, 63.01595, 63.01700, 62.83428, 62.82244 };

float arr_pSensor_compensationFactor_sf_bar_s[NUM_SEVEN_FOLD]  = { 0, 0, 0, 0, 0, 0, 0 };
float arr_pSensor_compensationFactor_tf_bar_s[NUM_TWELVE_FOLD] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

float thr_pressure_each_fin = THR_PRESSURE_EACH_FIN_2_5; // default thr : 2.5bar
float thr_pressure_each_ma = THR_PRESSURE_EACH_MA_2_5;   // default thr : 2.5bar
float thr_pressure_all_fin = THR_PRESSURE_ALL_FIN_2_5;   // default thr : 2.5bar

bool flag_start_IMM = 0;
bool flag_start_DRY = 0;

int gl_sum_fold = 0U;
float gl_final_avg_data = 0.0f;
float gl_avgcur_sum = 0.0f;

int flag_timeEnd = 0U;
int flag_toMain = 0U;
int flag_failCur = 0U;

int cnt_loop = 1U;
int cnt_timer = 0U;
int cnt_timeOut_reg = 0U;

unsigned long time_cur = 0;
int flag_timer_500ms = 0U;

int flag_DISP_deactivation = OFF;
int flag_HW_setting = NONE;
uint16_t val_regulatorInput = 0U;

uint16_t gl_STATE = SET_TEST_MODE;
int gl_DEBUG_MODE = WO_PLC;

int pt_start = 0U;
int pt_end = 0U;

float gl_ref_bar = 0.0f;
float gl_regOut_target = 0.0f;

int gl_percent = 0U;
int flag_serialHMI = 0U;
int flag_init = 0U;
int flag_toMain_setting = 0U;

float wait_time_stbl_ms = 900000;
float wait_time_test_cnt_500ms = 60;


// Function
void sendString(String sendData) {
  for(int i = 0; i < sendData.length(); i++) {
    Serial3.write(sendData[i]);
  }
  Serial3.write(0xff); Serial3.write(0xff); Serial3.write(0xff);
}

float calAve(float l_data_in, float cnt, float max_cnt)
{
  static float l_sum = 0.0f;
  float l_ave = 0.0f; 
  l_sum += l_data_in;

  l_ave = l_sum/cnt;

  if(cnt == max_cnt)
  {
    l_sum = 0.0f;
  }

  return l_ave;
}

float calAve_recursive(float l_data_in, float cnt, float l_ave_old)
{
  float l_ave = 0.0f;

  l_ave = (cnt-1)/cnt * l_ave_old + 1/cnt * l_data_in;
  

  return l_ave;
}


float calMovAve(float l_data_in, float l_data_in_oldest, float num, float l_movAve)
{
  l_movAve = l_movAve + (l_data_in - l_data_in_oldest)/num;

  return l_movAve;
}

void sendData2HMI(float f_data_in, String str_obj_name)
{
  String l_str_data_in = "";
  String l_str_data2hmi = "";

  l_str_data_in = String(f_data_in);

  l_str_data2hmi = str_obj_name;
  l_str_data2hmi.concat(".val=");
  l_str_data2hmi.concat(l_str_data_in[0]);
  l_str_data2hmi.concat(l_str_data_in[1]);  
  
  sendString(l_str_data2hmi);
}

void sendData2HMI_bt_off_on_all(String l_bt_name)
{
  l_bt_name.concat(".val=1");
  sendString(l_bt_name);
}
void sendData2HMI_bt_off_on(String l_bt_name, int l_idx)
{
  l_bt_name.concat(String(l_idx));
  l_bt_name.concat(".val=1");
  sendString(l_bt_name);
}
void sendData2HMI_WF(int l_int_data_in)
{
  String l_str_data_in = "";  
  String l_str_data2hmi = "";
  int len_data = 0U;
  
  l_str_data_in = String(l_int_data_in);

  while(l_int_data_in != 0)
  {
      l_int_data_in = l_int_data_in/10;
      ++len_data;
  }

  l_str_data2hmi = "add 4,0,";

  if(len_data==1U)
  {
    l_str_data2hmi.concat(l_str_data_in[0]);
  }
  else if(len_data==2U)
  {
    l_str_data2hmi.concat(l_str_data_in[0]);
    l_str_data2hmi.concat(l_str_data_in[1]);  
  }
  else
  {
    l_str_data2hmi.concat(l_str_data_in[0]);
    l_str_data2hmi.concat(l_str_data_in[1]);
    l_str_data2hmi.concat(l_str_data_in[2]);  
  }
  sendString(l_str_data2hmi);
}

void fT_sendData2HMI(float l_f_data_in, String l_str_obj_name_in)
{
  String l_str_obj_name = "";
  
  l_str_obj_name = l_str_obj_name_in;
  l_str_obj_name.concat(".txt=\"");
  Serial3.print(l_str_obj_name);
  Serial3.print(l_f_data_in, 2);
  Serial3.print("\"");
  Serial3.write(0xff);
  Serial3.write(0xff);
  Serial3.write(0xff);
}

void fT_sendData2HMI_idx(float l_f_data_in, String l_str_obj_name_in, int l_idx)
{
  String l_str_obj_name = "";
  
  l_str_obj_name = l_str_obj_name_in;
  l_str_obj_name.concat((String)l_idx);
  l_str_obj_name.concat(".txt=\"");
  Serial3.print(l_str_obj_name);
  Serial3.print(l_f_data_in, 2);
  Serial3.print("\"");
  Serial3.write(0xff);
  Serial3.write(0xff);
  Serial3.write(0xff);
}


void fT_sendData2HMI_WF(float f_data_in)
{
  String l_str_data_in = "";  
  String l_str_data2hmi = "";


  //l_str_data2hmi = "add 2,0,";
  //Serial3.print(l_str_data2hmi);
  //Serial3.print(f_data_in, 2);

  Serial3.print("add 2,0," + String(f_data_in));
  Serial3.write(0xff);
  Serial3.write(0xff);
  Serial3.write(0xff);
}



float int_cal_sum_arr()
{
  int l_sum_fold = 0U;
  
  if(flag_start_IMM == 1U)
  {
    for (int idx = 0U ; idx < NUM_SEVEN_FOLD; idx++)
    {
      l_sum_fold += stat_sf[idx];
    }
    for (int idx = 0U ; idx < NUM_TWELVE_FOLD; idx++)
    {
      l_sum_fold += stat_tf[idx];
    }
  }
  else if(flag_start_DRY == 1U)
  {
    for (int idx = 0U ; idx < NUM_SEVEN_FOLD_DRY; idx++)
    {
      l_sum_fold += stat_sf_dry[idx];
    }
    for (int idx = 0U ; idx < NUM_TWELVE_FOLD_DRY; idx++)
    {
      l_sum_fold += stat_tf_dry[idx];
    }
  }
  

  return l_sum_fold;
}


float int_cal_sum_arrFail()
{
  int l_sum_fold = 0U;
  
  if(flag_start_IMM == 1U)
  {
    for (int idx = 0U ; idx < NUM_SEVEN_FOLD; idx++)
    {
      l_sum_fold += fail_stat_sf_cur[idx];
    }
    for (int idx = 0U ; idx < NUM_TWELVE_FOLD; idx++)
    {
      l_sum_fold += fail_stat_tf_cur[idx];
    }
  }
  else if(flag_start_DRY == 1U)
  {
    for (int idx = 0U ; idx < NUM_SEVEN_FOLD_DRY; idx++)
    {
      l_sum_fold += fail_stat_sf_cur_dry[idx];
    }
    for (int idx = 0U ; idx < NUM_TWELVE_FOLD_DRY; idx++)
    {
      l_sum_fold += fail_stat_tf_cur_dry[idx];
    }
  }
  
  return l_sum_fold;
}


void sendData2HMI_del_button(String str_fold_in, int int_idx_in)
{
  String l_str_idx_in = "";  
  String l_str_data2hmi = "";

  l_str_idx_in = String(int_idx_in);

  l_str_data2hmi = "vis bt_";
  l_str_data2hmi.concat(str_fold_in);
  l_str_data2hmi.concat(l_str_idx_in);  
  l_str_data2hmi.concat(",0");
  
  sendString(l_str_data2hmi);
}

void sendData2HMI_on_failbutton(String str_fold_in, int int_idx_in)
{
  String l_str_idx_in = "";  
  String l_str_data2hmi = "";

  l_str_idx_in = String(int_idx_in);

  l_str_data2hmi = "vis p_fail_";
  l_str_data2hmi.concat(str_fold_in);
  l_str_data2hmi.concat(l_str_idx_in);  
  l_str_data2hmi.concat(",1");
  
  sendString(l_str_data2hmi);
}

void sendData2HMI_on_nobutton(String str_fold_in, int int_idx_in)
{
  String l_str_idx_in = "";  
  String l_str_data2hmi = "";

  l_str_idx_in = String(int_idx_in);

  l_str_data2hmi = "vis p_no_";
  l_str_data2hmi.concat(str_fold_in);
  l_str_data2hmi.concat(l_str_idx_in);  
  l_str_data2hmi.concat(",1");
  
  sendString(l_str_data2hmi);
}

void sendData2HMI_on_passbutton(String str_fold_in, int int_idx_in)
{
  String l_str_idx_in = "";  
  String l_str_data2hmi = "";

  l_str_idx_in = String(int_idx_in);

  l_str_data2hmi = "vis p_pass_";
  l_str_data2hmi.concat(str_fold_in);
  l_str_data2hmi.concat(l_str_idx_in);  
  l_str_data2hmi.concat(",1");
  
  sendString(l_str_data2hmi);
}


void sendData2HMI_PRGRS(int i_data_in)
{
  String l_str_data_in = "";  
  String l_str_data2hmi = "";
  int l_percent_progress = 0U;

  l_percent_progress = (int)(((float)i_data_in/(float)(wait_time_test_cnt_500ms))*100);

  if(l_percent_progress > 100)
  {
    l_percent_progress = 100;
  }

  l_str_data_in = String(l_percent_progress);

  l_str_data2hmi = "j0.val=";
  l_str_data2hmi.concat(l_str_data_in);
  
  sendString(l_str_data2hmi);
}


void sendData2HMI_PRGRS_STBL(int i_data_in)
{
  String l_str_data_in = "";  
  String l_str_data2hmi = "";
  int l_percent_progress = 0U;

  l_percent_progress = (int)(((float)i_data_in*1/(float)(wait_time_stbl_ms))*100);

  if(l_percent_progress > 95)
  {
    l_percent_progress = 100;
  }
  Serial_Mon("l_percent_progress", l_percent_progress);

  l_str_data_in = String(l_percent_progress);

  l_str_data2hmi = "j0.val=";
  l_str_data2hmi.concat(l_str_data_in);
  
  sendString(l_str_data2hmi);
}



void sendData2HMI_Cur(float l_arr_data_sf_in[], int l_arr_stat_sf_in[], float l_arr_data_tf_in[], int l_arr_stat_tf_in[])
{
  for (int idx = 0U ; idx < NUM_SEVEN_FOLD; idx++)
  {
    if(l_arr_stat_sf_in[idx] == 1)
    {
      fT_sendData2HMI(l_arr_data_sf_in[idx], "tCur");
    }
  }
  for (int idx = 0U ; idx < NUM_TWELVE_FOLD; idx++)
  {
    if(l_arr_stat_tf_in[idx] == 1)
    {
      fT_sendData2HMI(l_arr_data_tf_in[idx], "tCur");
    }
  }
}

void CheckFail(float l_thr)
{
  if(flag_start_IMM == 1U)
  {
    for (int idx = 0U ; idx < NUM_SEVEN_FOLD; idx++)
    {
      if(stat_sf[idx] == ON)
      {
        if(data_sf[idx]> l_thr)
        {
          fail_stat_sf_cur[idx] = FAIL;
        }
      }
    }
    for (int idx = 0U ; idx < NUM_TWELVE_FOLD; idx++)
    {
      if(stat_tf[idx] == ON)
      {
        if(data_tf[idx]> l_thr)
        {
          fail_stat_tf_cur[idx] = FAIL;
        }
      }
    }
  }

  else if(flag_start_DRY == 1U)
  {
    for (int idx = 0U ; idx < NUM_SEVEN_FOLD_DRY; idx++)
    {
      if(stat_sf_dry[idx] == ON)
      {
        if(data_sf_dry[idx]> l_thr)
        {
          fail_stat_sf_cur_dry[idx] = FAIL;
        }
      }
    }
    for (int idx = 0U ; idx < NUM_TWELVE_FOLD_DRY; idx++)
    {
      if(stat_tf_dry[idx] == ON)
      {
        if(data_tf_dry[idx]> l_thr)
        {
          fail_stat_tf_cur_dry[idx] = FAIL;
        }
      }
    }
  }
}

void sendData2HMI_Fail(int l_arr_stat_sf_in[], int l_arr_stat_tf_in[])
{
  for (int idx = 0U ; idx < NUM_SEVEN_FOLD; idx++)
  {
    if(l_arr_stat_sf_in[idx] == 1)
    {
      sendData2HMI_on_failbutton("sf", idx);
    }
    
  }
  for (int idx = 0U ; idx < NUM_TWELVE_FOLD; idx++)
  {
    if(l_arr_stat_tf_in[idx] == 1)
    {
      sendData2HMI_on_failbutton("tf", idx);
    }
  }
}

void sendData2HMI_Pass(int l_arr_stat_sf_in[], int l_arr_stat_tf_in[], int l_arr_stat_sf_p2_in[], int l_arr_stat_tf_p2_in[])
{
  for (int idx = 0U ; idx < NUM_SEVEN_FOLD; idx++)
  {
    if(l_arr_stat_sf_in[idx] == 1)
    {
      if(l_arr_stat_sf_p2_in[idx] == 0)
      {
        sendData2HMI_on_passbutton("sf", idx);
      }
    }
  }
  for (int idx = 0U ; idx < NUM_TWELVE_FOLD; idx++)
  {
    if(l_arr_stat_tf_in[idx] == 1)
    {
      if(l_arr_stat_tf_p2_in[idx] == 0)
      {
        sendData2HMI_on_passbutton("tf", idx);
      }
    }
  }
}
float fT_sendData2HMI_AvgCur_WF(float l_arr_data_sf_in[], int l_arr_stat_sf_in[], float l_arr_data_tf_in[], int l_arr_stat_tf_in[])
{
  int l_sum_fold = 0U;
  float l_sum_data = 0.0f;
  float l_avgcur = 0.0f;
  float l_avgcur_lin_intp = 0.0f;

  l_sum_fold = int_cal_sum_arr();

  for (int idx = 0U ; idx < NUM_SEVEN_FOLD; idx++)
  {
    if(l_arr_stat_sf_in[idx] == 1)
    {
      l_sum_data = l_sum_data + l_arr_data_sf_in[idx];
    }
  }
  for (int idx = 0U ; idx < NUM_TWELVE_FOLD; idx++)
  {
    if(l_arr_stat_tf_in[idx] == 1)
    {
      l_sum_data = l_sum_data + l_arr_data_tf_in[idx];
    }
  }

  l_avgcur = l_sum_data/(float)l_sum_fold;

  l_avgcur_lin_intp = lin_Intp(YLIM_LOW, YLIM_HIGH, 0, 255, l_avgcur);

  sendData2HMI_WF((int)l_avgcur_lin_intp);

  return l_avgcur;
}

float lin_Intp(float x0, float x1, float y0, float y1, float x_in)
{
  float rst;

  if(x_in <= 0)
  {
    x_in = 0;
  }

  rst = y0+(y1-y0)/(x1-x0)*(x_in-x0);

  if(rst > y1)
  {
    rst = y1-1;
  }

  return rst;
}

void Serial_Mon(String l_str_name_in, float l_data_in)
{
  String l_str_data_name = "";
  l_str_data_name = l_str_name_in;
  l_str_data_name.concat(" : ");
  Serial.print(l_str_data_name);
  Serial.println(l_data_in);
}

float queue_MovAve_2D(float l_data_in, int l_idx, float l_queue[][PERIOD_MA], int queue_size)
{
  float l_oldest = 0.0f;

  if(pt_start > ((int)(queue_size-1)))
  {
    pt_start = 0;
  }
  if(pt_end > ((int)(queue_size-1)))
  {
    pt_end = 0;
  }

  l_oldest = l_queue[l_idx][pt_end];

  l_queue[l_idx][pt_start] = l_data_in;

  return l_oldest;
}

void init_setup(ModbusMaster232 l_node_in)
{
  l_node_in.begin(115200);  // Modbus RTU
  Serial3.begin(9600);  // for HMI
  

  pinMode(LED_PIN, OUTPUT);

  // Button for diagnostic
  p0_bDiag.attachPush(p0_bDiag_Callback, &p0_bDiag);
  p0_bDebug.attachPush(p0_bDebug_Callback, &p0_bDebug);

  // Button for choosing pressure
  p1_bt0.attachPush(p1_bt0_Callback, &p1_bt0);
  p1_bt1.attachPush(p1_bt1_Callback, &p1_bt1);

  // Button for checking fold
  p1_bt_sf0.attachPush(p1_bt_sf0_Callback, &p1_bt_sf0);
  p1_bt_sf1.attachPush(p1_bt_sf1_Callback, &p1_bt_sf1);
  p1_bt_sf2.attachPush(p1_bt_sf2_Callback, &p1_bt_sf2);
  p1_bt_sf3.attachPush(p1_bt_sf3_Callback, &p1_bt_sf3);
  p1_bt_sf4.attachPush(p1_bt_sf4_Callback, &p1_bt_sf4);
  p1_bt_sf5.attachPush(p1_bt_sf5_Callback, &p1_bt_sf5);
  p1_bt_sf6.attachPush(p1_bt_sf6_Callback, &p1_bt_sf6);

  p1_bt_tf0.attachPush(p1_bt_tf0_Callback, &p1_bt_tf0);
  p1_bt_tf1.attachPush(p1_bt_tf1_Callback, &p1_bt_tf1);
  p1_bt_tf2.attachPush(p1_bt_tf2_Callback, &p1_bt_tf2);
  p1_bt_tf3.attachPush(p1_bt_tf3_Callback, &p1_bt_tf3);
  p1_bt_tf4.attachPush(p1_bt_tf4_Callback, &p1_bt_tf4);
  p1_bt_tf5.attachPush(p1_bt_tf5_Callback, &p1_bt_tf5);
  p1_bt_tf6.attachPush(p1_bt_tf6_Callback, &p1_bt_tf6);
  p1_bt_tf7.attachPush(p1_bt_tf7_Callback, &p1_bt_tf7);
  p1_bt_tf8.attachPush(p1_bt_tf8_Callback, &p1_bt_tf8);
  p1_bt_tf9.attachPush(p1_bt_tf9_Callback, &p1_bt_tf9);
  p1_bt_tf10.attachPush(p1_bt_tf10_Callback, &p1_bt_tf10);
  p1_bt_tf11.attachPush(p1_bt_tf11_Callback, &p1_bt_tf11);

  p1_bChk_All.attachPush(p1_bChk_All_Callback, &p1_bChk_All);  // Check all
  p1_bClr_All.attachPush(p1_bClr_All_Callback, &p1_bClr_All);  // Clear all

  p1_b0.attachPush(p1_b0_Callback, &p1_b0);

  p1_plusMinus0.attachPush(p1_plusMinus0_Callback, &p1_plusMinus0);
  p1_plusMinus1.attachPush(p1_plusMinus1_Callback, &p1_plusMinus1);
  t_p1_plusMinus0.attachPush(t_p1_plusMinus0_Callback, &t_p1_plusMinus0);
  t_p1_plusMinus1.attachPush(t_p1_plusMinus1_Callback, &t_p1_plusMinus1);
  

  p2_b0.attachPush(p2_b0_Callback, &p2_b0);

  p2_bt_sf0.attachPush(p2_bt_sf0_Callback, &p2_bt_sf0);
  p2_bt_sf1.attachPush(p2_bt_sf1_Callback, &p2_bt_sf1);
  p2_bt_sf2.attachPush(p2_bt_sf2_Callback, &p2_bt_sf2);
  p2_bt_sf3.attachPush(p2_bt_sf3_Callback, &p2_bt_sf3);
  p2_bt_sf4.attachPush(p2_bt_sf4_Callback, &p2_bt_sf4);
  p2_bt_sf5.attachPush(p2_bt_sf5_Callback, &p2_bt_sf5);
  p2_bt_sf6.attachPush(p2_bt_sf6_Callback, &p2_bt_sf6);

  p2_bt_tf0.attachPush(p2_bt_tf0_Callback, &p2_bt_tf0);
  p2_bt_tf1.attachPush(p2_bt_tf1_Callback, &p2_bt_tf1);
  p2_bt_tf2.attachPush(p2_bt_tf2_Callback, &p2_bt_tf2);
  p2_bt_tf3.attachPush(p2_bt_tf3_Callback, &p2_bt_tf3);
  p2_bt_tf4.attachPush(p2_bt_tf4_Callback, &p2_bt_tf4);
  p2_bt_tf5.attachPush(p2_bt_tf5_Callback, &p2_bt_tf5);
  p2_bt_tf6.attachPush(p2_bt_tf6_Callback, &p2_bt_tf6);
  p2_bt_tf7.attachPush(p2_bt_tf7_Callback, &p2_bt_tf7);
  p2_bt_tf8.attachPush(p2_bt_tf8_Callback, &p2_bt_tf8);
  p2_bt_tf9.attachPush(p2_bt_tf9_Callback, &p2_bt_tf9);
  p2_bt_tf10.attachPush(p2_bt_tf10_Callback, &p2_bt_tf10);
  p2_bt_tf11.attachPush(p2_bt_tf11_Callback, &p2_bt_tf11);

  // Button for choosing pressure
  p4_bt0.attachPush(p4_bt0_Callback, &p4_bt0);
  p4_bt1.attachPush(p4_bt1_Callback, &p4_bt1);

  // Button for checking fold
  p4_bt_sf0.attachPush(p4_bt_sf0_Callback, &p4_bt_sf0);
  p4_bt_sf1.attachPush(p4_bt_sf1_Callback, &p4_bt_sf1);
  p4_bt_sf2.attachPush(p4_bt_sf2_Callback, &p4_bt_sf2);
  p4_bt_sf3.attachPush(p4_bt_sf3_Callback, &p4_bt_sf3);
  p4_bt_sf4.attachPush(p4_bt_sf4_Callback, &p4_bt_sf4);

  p4_bt_tf0.attachPush(p4_bt_tf0_Callback, &p4_bt_tf0);
  p4_bt_tf1.attachPush(p4_bt_tf1_Callback, &p4_bt_tf1);
  p4_bt_tf2.attachPush(p4_bt_tf2_Callback, &p4_bt_tf2);
  p4_bt_tf3.attachPush(p4_bt_tf3_Callback, &p4_bt_tf3);
  p4_bt_tf4.attachPush(p4_bt_tf4_Callback, &p4_bt_tf4);
  p4_bt_tf5.attachPush(p4_bt_tf5_Callback, &p4_bt_tf5);
  p4_bt_tf6.attachPush(p4_bt_tf6_Callback, &p4_bt_tf6);
  p4_bt_tf7.attachPush(p4_bt_tf7_Callback, &p4_bt_tf7);
  p4_bt_tf8.attachPush(p4_bt_tf8_Callback, &p4_bt_tf8);
  p4_bt_tf9.attachPush(p4_bt_tf9_Callback, &p4_bt_tf9);
  p4_bt_tf10.attachPush(p4_bt_tf10_Callback, &p4_bt_tf10);
  p4_bt_tf11.attachPush(p4_bt_tf11_Callback, &p4_bt_tf11);

  p4_bChk_All.attachPush(p4_bChk_All_Callback, &p4_bChk_All);  // Check all
  p4_bClr_All.attachPush(p4_bClr_All_Callback, &p4_bClr_All);  // Clear all

  p4_b0.attachPush(p4_b0_Callback, &p4_b0);
  p5_b0.attachPush(p5_b0_Callback, &p5_b0);
  p3_b0.attachPush(p3_b0_Callback, &p3_b0);
  p1_b1.attachPush(p1_b1_Callback, &p1_b1);

  p4_b1.attachPush(p4_b1_Callback, &p4_b1);
  p4_plusMinus0.attachPush(p4_plusMinus0_Callback, &p4_plusMinus0);
  p4_plusMinus1.attachPush(p4_plusMinus1_Callback, &p4_plusMinus1);
  t_p4_plusMinus0.attachPush(t_p4_plusMinus0_Callback, &t_p4_plusMinus0);
  t_p4_plusMinus1.attachPush(t_p4_plusMinus1_Callback, &t_p4_plusMinus1);
  



  
  // initialization buttons for page 1  
  for (int idx = 0U ; idx < NUM_SEVEN_FOLD; idx++)
  {
    stat_sf[idx] = 0;
  }
  for (int idx = 0U ; idx < NUM_TWELVE_FOLD; idx++)
  {
    stat_tf[idx] = 0;
  }

  // initialization buttons for page 2 
  for (int idx = 0U ; idx < NUM_SEVEN_FOLD; idx++)
  {
    stat_sf_p2[idx] = 0;
  }
  for (int idx = 0U ; idx < NUM_TWELVE_FOLD; idx++)
  {
    stat_tf_p2[idx] = 0;
  }

  // initialization fail stat cur  
  for (int idx = 0U ; idx < NUM_SEVEN_FOLD; idx++)
  {
    fail_stat_sf_cur[idx] = 0;
  }
  for (int idx = 0U ; idx < NUM_TWELVE_FOLD; idx++)
  {
    fail_stat_tf_cur[idx] = 0;
  }

  // initialization fail stat fin 
  for (int idx = 0U ; idx < NUM_SEVEN_FOLD; idx++)
  {
    fail_stat_sf_fin[idx] = 0;
  }
  for (int idx = 0U ; idx < NUM_TWELVE_FOLD; idx++)
  {
    fail_stat_tf_fin[idx] = 0;
  }
  gl_STATE = INIT_HARDWARE;
  flag_DISP_deactivation = OFF;

  MsTimer2::set(PERIOD_PLC, send_Protocol_to_PLC);
}


void cal_recAve_All(int l_stat_sf_in[], float l_data_sf_in[], float l_aveRecursive_sf_in[], float l_aveRecursive_sf_old_in[], int l_stat_tf_in[], float l_data_tf_in[], float l_aveRecursive_tf_in[], float l_aveRecursive_tf_old_in[], int l_mode_in)
{
  int l_mode_idx_sf = 0U;
  int l_mode_idx_tf = 0U;

  if(l_mode_in==RUN_DRY)
  {
    l_mode_idx_sf = NUM_SEVEN_FOLD_DRY;
    l_mode_idx_tf = NUM_TWELVE_FOLD_DRY;
  }
  else
  {
    l_mode_idx_sf = NUM_SEVEN_FOLD;
    l_mode_idx_tf = NUM_TWELVE_FOLD;
  }

  for (int idx = 0U ; idx < l_mode_idx_sf; idx++)
  {
    if(l_stat_sf_in[idx] == 1)
    {
      l_aveRecursive_sf_in[idx] = calAve_recursive(l_data_sf_in[idx], cnt_loop, l_aveRecursive_sf_old_in[idx]);
      l_aveRecursive_sf_old_in[idx] = l_aveRecursive_sf_in[idx];
    }
  }
  for (int idx = 0U ; idx < l_mode_idx_tf; idx++)
  {
    if(l_stat_tf_in[idx] == 1)
    {
      l_aveRecursive_tf_in[idx] = calAve_recursive(l_data_tf_in[idx], cnt_loop, l_aveRecursive_tf_old_in[idx]);
      l_aveRecursive_tf_old_in[idx] = l_aveRecursive_tf_in[idx];
    }
  }
}


void cal_MovAve_Cur_All(float l_data_sf_in[], float l_data_sf_ma_in[], float l_data_sf_oldest_in[], float l_data_tf_in[], float l_data_tf_ma_in[], float l_data_tf_oldest_in[], int l_mode_in)
{
  int l_mode_idx_sf = 0U;
  int l_mode_idx_tf = 0U;

  if(l_mode_in==RUN_DRY)
  {
    l_mode_idx_sf = NUM_SEVEN_FOLD_DRY;
    l_mode_idx_tf = NUM_TWELVE_FOLD_DRY;
  }
  else
  {
    l_mode_idx_sf = NUM_SEVEN_FOLD;
    l_mode_idx_tf = NUM_TWELVE_FOLD;
  }

  for (int idx = 0U ; idx < l_mode_idx_sf; idx++)
  {
    l_data_sf_oldest_in[idx] = queue_MovAve_2D(l_data_sf_in[idx], idx, data_sf_queue, PERIOD_MA);
    l_data_sf_ma_in[idx] = calMovAve(l_data_sf_in[idx], l_data_sf_oldest_in[idx], PERIOD_MA, l_data_sf_ma_in[idx]);
  }
  for (int idx = 0U ; idx < l_mode_idx_tf; idx++)
  {
    l_data_tf_oldest_in[idx] = queue_MovAve_2D(l_data_tf_in[idx], idx, data_tf_queue, PERIOD_MA);
    l_data_tf_ma_in[idx] = calMovAve(l_data_tf_in[idx], l_data_tf_oldest_in[idx], PERIOD_MA, l_data_tf_ma_in[idx]);
  }
}

uint16_t get_val_resolution(uint16_t l_val_in, uint16_t l_resolution_in)
{
  l_val_in = l_val_in/l_resolution_in;
  l_val_in = l_val_in*l_resolution_in;

  return l_val_in;
}

void wait_openAI(unsigned long duration) {
  unsigned long start_time = millis();
  unsigned long time_cur = millis();
  static byte inByte = 0;     
  
  Serial3.flush();
  while (millis() - start_time < duration){
    if (duration == wait_time_stbl_ms)
    {
      while (Serial3.available()) {
        inByte = Serial3.read();        
        
        if(inByte == 8)
        { 
          Serial_Mon("flag_serialHMI", flag_serialHMI);
          flag_serialHMI = ON;
          inByte = 0;
          break;
        }
      }
      if(flag_serialHMI == ON)
      {
        break;
      }
      sendData2HMI_PRGRS_STBL(millis() - start_time);
      HMI_DISP_Time((millis() - time_cur) * 0.001, "tProgress");
    }
    
  }
}