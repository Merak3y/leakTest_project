#include "Nextion.h"
#include "Common_library.h"
#include <MsTimer2.h>
#include "HMI_Declaration_DRY.h"
#include "HMI_Declaration_IMM.h"
#include "HMI_Func_DISP.h"
#include "HW_Func.h"


void get_Signal_Pressure_All(ModbusMaster232 l_node_in)
{
  if(flag_start_IMM == 1U)
  {
    for (int idx = 0U ; idx < NUM_SEVEN_FOLD; idx++)
    {
      if(stat_sf[idx] == 1)
      {
        get_Signal_Pressure(data_sf, idx, l_node_in, arr_addr_P_Sensor_SF[idx], arr_pSensor_conversionFactor_sf, arr_pSensor_compensationFactor_sf_bar_s, 1);
      }
    }
    for (int idx = 0U ; idx < NUM_TWELVE_FOLD; idx++)
    {
      if(stat_tf[idx] == 1)
      {
        get_Signal_Pressure(data_tf, idx, l_node_in, arr_addr_P_Sensor_TF[idx], arr_pSensor_conversionFactor_tf, arr_pSensor_compensationFactor_tf_bar_s, 2);
      }
    }
  }
  else if(flag_start_DRY == 1U)
  {
    for (int idx = 0U ; idx < NUM_SEVEN_FOLD_DRY; idx++)
    {
      if(stat_sf_dry[idx] == 1)
      {
        get_Signal_Pressure(data_sf_dry, idx, l_node_in, arr_addr_P_Sensor_SF_dry[idx], arr_pSensor_conversionFactor_sf_dry, arr_pSensor_compensationFactor_sf_bar_s, 1);
      }
    }
    for (int idx = 0U ; idx < NUM_TWELVE_FOLD_DRY; idx++)
    {
      if(stat_tf_dry[idx] == 1)
      {
        get_Signal_Pressure(data_tf_dry, idx, l_node_in, arr_addr_P_Sensor_TF_dry[idx], arr_pSensor_conversionFactor_tf_dry, arr_pSensor_compensationFactor_tf_bar_s, 2);
      }
    }
  }
}


void get_Signal_Pressure(float l_data_in[], int l_num_hose, ModbusMaster232 l_node_in, uint16_t l_addr_in, float l_arr_pSensor_conversionFactor[], float l_arr_pSensor_compensationFactor_bar_s[], uint16_t sf_tf) {
  l_node_in.readInputRegisters(l_addr_in, 1);

  l_data_in[l_num_hose] = get_val_resolution(l_node_in.getResponseBuffer(0), RESOLUTION_SENSOR);

  if (l_data_in[l_num_hose] > 65000) {
    l_data_in[l_num_hose] = 0.0f;
  }
  
  l_data_in[l_num_hose] = l_data_in[l_num_hose] * l_arr_pSensor_conversionFactor[l_num_hose] * 0.00001;

  
  if(flag_start_IMM == 1U)
  {
    if(sf_tf == 1U)
    {
      l_data_in[l_num_hose] = refPresure_sf[l_num_hose] - l_data_in[l_num_hose] + l_arr_pSensor_compensationFactor_bar_s[l_num_hose]; 
    }
    else if (sf_tf == 2U)
    {
      l_data_in[l_num_hose] = refPresure_tf[l_num_hose] - l_data_in[l_num_hose] + l_arr_pSensor_compensationFactor_bar_s[l_num_hose]; 
    }
  }
  
  else if(flag_start_DRY == 1U)
  {
    if(sf_tf == 1U)
    {
      l_data_in[l_num_hose] = refPresure_sf_dry[l_num_hose] - l_data_in[l_num_hose] + l_arr_pSensor_compensationFactor_bar_s[l_num_hose]; 
    }
    else if (sf_tf == 2U)
    {
      l_data_in[l_num_hose] = refPresure_tf_dry[l_num_hose] - l_data_in[l_num_hose] + l_arr_pSensor_compensationFactor_bar_s[l_num_hose]; 
    }
  }
  
  
  // if (l_data_in[l_num_hose] <= 0)
  // {
  //   l_data_in[l_num_hose] = 0;
  // }
  

  l_node_in.clearResponseBuffer();
}
