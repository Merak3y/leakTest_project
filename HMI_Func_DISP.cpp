#include "Nextion.h"
#include "Common_library.h"
#include "HMI_Declaration_IMM.h"
#include "HMI_Declaration_DRY.h"


// inact btN_tf0.val=1
void HMI_DISP_deactivation_BUTTON(String str_hose_name_in, int l_int_hose_idx_in, int l_activation_flag_in)
{
  String l_str_hose_idx_in = "";  
  String l_str_data2hmi = "";
  String l_str_activation_flag_in = "";

  l_str_hose_idx_in = String(l_int_hose_idx_in);
  l_str_activation_flag_in = String(l_activation_flag_in);

  l_str_data2hmi.concat(str_hose_name_in);
  l_str_data2hmi.concat(l_str_hose_idx_in);  
  l_str_data2hmi.concat(".val=");
  l_str_data2hmi.concat(l_str_activation_flag_in);

  sendString(l_str_data2hmi);
}

// fail bt_tf1.val=1
void HMI_DISP_fail_BUTTON(String str_hose_name_in, int l_int_hose_idx_in, int l_fail_flag_in)
{
  String l_str_hose_idx_in = "";  
  String l_str_data2hmi = "";
  String l_str_fail_flag_in = "";

  l_str_hose_idx_in = String(l_int_hose_idx_in);
  l_str_fail_flag_in = String(l_fail_flag_in);

  l_str_data2hmi.concat(str_hose_name_in);
  l_str_data2hmi.concat(l_str_hose_idx_in);  
  l_str_data2hmi.concat(".val=");
  l_str_data2hmi.concat(l_str_fail_flag_in);

  sendString(l_str_data2hmi);
}


void HMI_DISP_fail_BUTTON_ALL(String l_str_hose_name_in)
{
  int l_mode_idx_sf = 0U;
  int l_mode_idx_tf = 0U;

  String l_str_hose_name_in_sf = "";
  String l_str_hose_name_in_tf = "";

  l_str_hose_name_in_sf = l_str_hose_name_in;
  l_str_hose_name_in_sf.concat("_sf");

  l_str_hose_name_in_tf = l_str_hose_name_in;
  l_str_hose_name_in_tf.concat("_tf");
  
  if(flag_start_IMM == 1U)
  {
    for (int idx = 0U ; idx < NUM_SEVEN_FOLD; idx++)
    {
      if(fail_stat_sf_cur[idx] == ON)
      {
        HMI_DISP_fail_BUTTON(l_str_hose_name_in_sf, idx, FAIL);
      }
    }
    for (int idx = 0U ; idx < NUM_TWELVE_FOLD; idx++)
    {
      if(fail_stat_tf_cur[idx] == ON)
      {
        HMI_DISP_fail_BUTTON(l_str_hose_name_in_tf, idx, FAIL);
      }
    }
  }

  else if(flag_start_DRY == 1U)
  {
    for (int idx = 0U ; idx < NUM_SEVEN_FOLD_DRY; idx++)
    {
      if(fail_stat_sf_cur_dry[idx] == ON)
      {
        HMI_DISP_fail_BUTTON(l_str_hose_name_in_sf, idx, FAIL);
      }
    }
    for (int idx = 0U ; idx < NUM_TWELVE_FOLD_DRY; idx++)
    {
      if(fail_stat_tf_cur_dry[idx] == ON)
      {
        HMI_DISP_fail_BUTTON(l_str_hose_name_in_tf, idx, FAIL);
      }
    }
  }
}


// vis btN_tf1,0 no disp
void HMI_DISP_vis_BUTTON(String str_hose_name_in, int l_int_hose_idx_in, int l_vis_flag_in)
{
  String l_str_hose_idx_in = "";  
  String l_str_data2hmi = "";
  String l_str_vis_flag = "";

  l_str_hose_idx_in = String(l_int_hose_idx_in);
  l_str_vis_flag    = String(l_vis_flag_in);

  l_str_data2hmi = "vis ";
  l_str_data2hmi.concat(str_hose_name_in);
  l_str_data2hmi.concat(l_str_hose_idx_in);  
  l_str_data2hmi.concat(",");
  l_str_data2hmi.concat(l_str_vis_flag);
  
  sendString(l_str_data2hmi);
}


void HMI_DISP_onText(String l_str_obj_name_in, int l_hose_idx, float l_f_data_in)
{
  String l_str_obj_name = "";
  String l_str_hose_idx = "";
  
  l_str_hose_idx = String(l_hose_idx);

  l_str_obj_name = l_str_obj_name_in;
  l_str_obj_name.concat(l_str_hose_idx);
  l_str_obj_name.concat(".txt=\"");
  Serial3.print(l_str_obj_name);
  Serial3.print(l_f_data_in, 2);
  Serial3.print("\"");
  Serial3.write(0xff);
  Serial3.write(0xff);
  Serial3.write(0xff);
}

void HMI_DISP_Cur_PRS_ALL()
{  
  if(flag_start_IMM == 1U)
  {
    for (int idx = 0U ; idx < NUM_SEVEN_FOLD; idx++)
    {
      if(stat_sf[idx] == 1)
      {
        HMI_DISP_onText("t_sf", idx, data_sf[idx]);
      }
    }
    for (int idx = 0U ; idx < NUM_TWELVE_FOLD; idx++)
    {
      if(stat_tf[idx] == 1)
      {
        HMI_DISP_onText("t_tf", idx, data_tf[idx]);
      }
    }
  }
  else if(flag_start_DRY == 1U)
  {
    for (int idx = 0U ; idx < NUM_SEVEN_FOLD_DRY; idx++)
    {
      if(stat_sf_dry[idx] == 1)
      {
        HMI_DISP_onText("t_sf", idx, data_sf_dry[idx]);
      }
    }
    for (int idx = 0U ; idx < NUM_TWELVE_FOLD_DRY; idx++)
    {
      if(stat_tf_dry[idx] == 1)
      {
        HMI_DISP_onText("t_tf", idx, data_tf_dry[idx]);
      }
    }
  }
}
void HMI_DISP_vis_BUTTON_ALL()
{  
  if(flag_start_IMM == 1U)
  {
    for (int idx = 0U ; idx < NUM_SEVEN_FOLD; idx++)
    {
      if(stat_sf[idx] == FAIL)
      {
        HMI_DISP_vis_BUTTON("btN_sf", idx, OFF);
      }
    }
    for (int idx = 0U ; idx < NUM_TWELVE_FOLD; idx++)
    {
      if(stat_tf[idx] == FAIL)
      {
        HMI_DISP_vis_BUTTON("btN_tf", idx, OFF);
      }
    }
  }

  else if(flag_start_DRY == 1U)
  {
    for (int idx = 0U ; idx < NUM_SEVEN_FOLD_DRY; idx++)
    {
      if(stat_sf_dry[idx] == FAIL)
      {
        HMI_DISP_vis_BUTTON("btN_sf", idx, OFF);
      }
    }
    for (int idx = 0U ; idx < NUM_TWELVE_FOLD_DRY; idx++)
    {
      if(stat_tf_dry[idx] == FAIL)
      {
        HMI_DISP_vis_BUTTON("btN_tf", idx, OFF);
      }
    }
  }
}

void HMI_DISP_WF(int l_int_data_in, int l_wf_id_in)
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
  l_str_data2hmi = "add ";
  l_str_data2hmi.concat(String(l_wf_id_in));
  l_str_data2hmi.concat(",0,");

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

float HMI_DISP_AvgCur_WF()
{
  int l_wf_id_in = 0U;
  int l_sum_fold = 0U;
  float l_sum_data = 0.0f;
  float l_avgcur = 0.0f;
  float l_avgcur_lin_intp = 0.0f;

  int l_mode_idx_sf = 0U;
  int l_mode_idx_tf = 0U;


  if(flag_start_DRY == 1U)
  { 
    l_wf_id_in = ID_WF_DRY;
    l_sum_fold = int_cal_sum_arr();

    for (int idx = 0U ; idx < NUM_SEVEN_FOLD_DRY; idx++)
    {
      if(stat_sf_dry[idx] == 1)
      {
        l_sum_data = l_sum_data + data_sf_dry[idx];
      }
    }
    for (int idx = 0U ; idx < NUM_TWELVE_FOLD_DRY; idx++)
    {
      if(stat_tf_dry[idx] == 1)
      {
        l_sum_data = l_sum_data + data_tf_dry[idx];
      }
    }
  }
  else if(flag_start_IMM == 1U)
  {
    
    l_wf_id_in = ID_WF_IMM;
    l_sum_fold = int_cal_sum_arr();
    for (int idx = 0U ; idx < NUM_SEVEN_FOLD; idx++)
    {
      if(stat_sf[idx] == 1)
      {
        l_sum_data = l_sum_data + data_sf[idx];
      }
    }
    for (int idx = 0U ; idx < NUM_TWELVE_FOLD; idx++)
    {
      if(stat_tf[idx] == 1)
      {
        l_sum_data = l_sum_data + data_tf[idx];
      }
    }
  }
  Serial_Mon("l_sum_data", l_sum_data);

  l_avgcur = l_sum_data/(float)l_sum_fold;
  Serial_Mon("l_avgcur", l_avgcur);
  l_avgcur_lin_intp = lin_Intp(YLIM_LOW-0.001, YLIM_HIGH, 0, 255, l_avgcur);
  Serial_Mon("l_avgcur_lin_intp", l_avgcur_lin_intp);
  HMI_DISP_WF((int)l_avgcur_lin_intp, l_wf_id_in);

  return l_avgcur;
}

// inact btN_tf0.val=1
void HMI_DISP_deactivation_BUTTON_ALL(String str_hose_name_in)
{
  int l_mode_idx_sf = 0U;
  int l_mode_idx_tf = 0U;

  String l_str_hose_name = "";
  String l_str_hose_name_sf = "";
  String l_str_hose_name_tf = "";


  l_str_hose_name = str_hose_name_in;

  l_str_hose_name_sf = l_str_hose_name;
  l_str_hose_name_sf.concat("_sf");
  
  l_str_hose_name_tf = l_str_hose_name;
  l_str_hose_name_tf.concat("_tf");


  if(flag_start_IMM == 1U)
  {
    for (int idx = 0U ; idx < NUM_SEVEN_FOLD; idx++)
    {
      if(stat_sf[idx] == 0)
      {
        HMI_DISP_deactivation_BUTTON(l_str_hose_name_sf, idx, DE_ACTIVATION);
      }
    }
    for (int idx = 0U ; idx < NUM_TWELVE_FOLD; idx++)
    {
      if(stat_tf[idx] == 0)
      {
        HMI_DISP_deactivation_BUTTON(l_str_hose_name_tf, idx, DE_ACTIVATION);
      }
    }
  }
  else if(flag_start_DRY == 1U)
  {
    for (int idx = 0U ; idx < NUM_SEVEN_FOLD_DRY; idx++)
    {
      if(stat_sf_dry[idx] == 0)
      {
        HMI_DISP_deactivation_BUTTON(l_str_hose_name_sf, idx, DE_ACTIVATION);
      }
    }
    for (int idx = 0U ; idx < NUM_TWELVE_FOLD_DRY; idx++)
    {
      if(stat_tf_dry[idx] == 0)
      {
        HMI_DISP_deactivation_BUTTON(l_str_hose_name_tf, idx, DE_ACTIVATION);
      }
    }    
  }
}

void HMI_DISP_Time(float l_f_data_in, String l_str_obj_name_in)
{
  String l_str_obj_name = "";
  uint16_t l_min = 0U;
  uint16_t l_sec = 0U;

  l_sec = (uint16_t)l_f_data_in%60;
  l_min = ((uint16_t)l_f_data_in/60)%60;

  l_str_obj_name = l_str_obj_name_in;
  l_str_obj_name.concat(".txt=\"");
  l_str_obj_name.concat(l_min);
  l_str_obj_name.concat(" min ");
  l_str_obj_name.concat(l_sec);
  l_str_obj_name.concat(" sec\"");
  sendString(l_str_obj_name);
  Serial3.write(0xff);
  Serial3.write(0xff);
  Serial3.write(0xff);
  

}

  
  

