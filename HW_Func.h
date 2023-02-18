#ifndef MyFuncHWLibrary_H
#define MyFuncHWLibrary_H

void get_Signal_Pressure_All(ModbusMaster232 l_node_in);
void get_Signal_Pressure(float l_data_in[], int l_num_hose, ModbusMaster232 l_node_in, uint16_t l_addr_in, float l_arr_pSensor_conversionFactor[], float l_arr_pSensor_compensationFactor_bar_s[], uint16_t sf_tf);

extern uint16_t arr_addr_P_Sensor_SF[NUM_SEVEN_FOLD];
extern uint16_t arr_addr_P_Sensor_TF[NUM_TWELVE_FOLD];

extern uint16_t arr_addr_Solenoid_ON_SF[NUM_SEVEN_FOLD];
extern uint16_t arr_addr_Solenoid_OFF_SF[NUM_SEVEN_FOLD];

extern uint16_t arr_addr_Solenoid_ON_TF[NUM_TWELVE_FOLD];
extern uint16_t arr_addr_Solenoid_OFF_TF[NUM_TWELVE_FOLD];

extern uint16_t arr_addr_P_Sensor_SF_dry[NUM_SEVEN_FOLD_DRY];
extern uint16_t arr_addr_P_Sensor_TF_dry[NUM_TWELVE_FOLD_DRY];

extern uint16_t arr_addr_Solenoid_ON_SF_dry[NUM_SEVEN_FOLD_DRY];
extern uint16_t arr_addr_Solenoid_OFF_SF_dry[NUM_SEVEN_FOLD_DRY];

extern uint16_t arr_addr_Solenoid_ON_TF_dry[NUM_TWELVE_FOLD_DRY];
extern uint16_t arr_addr_Solenoid_OFF_TF_dry[NUM_TWELVE_FOLD_DRY];

#endif


