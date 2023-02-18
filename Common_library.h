#ifndef MyLibrary_H
#define MyLibrary_H

#include <ModbusMaster232.h> 

/********************************************************* Define *********************************************************/
#define LED_PIN    13

#define PERIOD_ALL 50 // Period of main loop
#define PERIOD_MA  5

#define PERIOD_PLC 500 // ms Period of PLC
// Total period(ms) = PERIOD_PLC * wait_time_test_cnt_500ms

#define WAIT_TIME_HARDWARE 1000-1
#define WAIT_TIME_HARDWARE_BENT 3000
#define WAIT_TIME_HARDWARE_REGULATION 500


#define WAIT_TIME_STABILIZATION_BF_CLOSE_SOL 10000-1//12000

#define WAIT_TIME_STABILIZATION 3001//30000+1//180000//ms


#define TIMEOUT_STABILIZATION 20 // PERIOD_PLC(500ms) * TIMEOUT_STABILIZATION = time(ms), 10(s)

//
#define SET_TEST_MODE 0U
#define RUN_IMM            1U
#define RUN_DIAGNOSTIC     3U
#define RUN_DRY            2U
#define INIT_HARDWARE      100U

#define ACTIVATION    0U
#define DE_ACTIVATION 1U

#define PASS    0U
#define FAIL    1U

#define OFF    0U
#define ON     1U

#define W_PLC 0U
#define WO_PLC 1U

#define MAIN_VALVE_ON  0U
#define MAIN_VALVE_OFF 1U

#define SOLENOID_ON  1U
#define SOLENOID_OFF 0U

// Threshold

#define THR_PRESSURE_ALL_FIN_2_5 0.02f //100%
#define THR_PRESSURE_ALL_FIN_7_5 0.05f

#define THR_PRESSURE_EACH_FIN_2_5 THR_PRESSURE_ALL_FIN_2_5 * 1.5 // 150%
#define THR_PRESSURE_EACH_FIN_7_5 THR_PRESSURE_ALL_FIN_7_5 * 1.5

#define THR_PRESSURE_EACH_MA_2_5 50000//THR_PRESSURE_ALL_FIN_2_5 * 2.0
#define THR_PRESSURE_EACH_MA_7_5 60000//THR_PRESSURE_ALL_FIN_7_5 * 2.0//200%




#define ANALOG_INPUT_2_5 4433 
#define ANALOG_INPUT_7_5 13300

#define REF_BAR_2_5 2.5//3950
#define REF_BAR_7_5 7.5//12000



// The number of each fold
#define NUM_SEVEN_FOLD  7
#define NUM_TWELVE_FOLD 12

#define NUM_SEVEN_FOLD_DRY 5
#define NUM_TWELVE_FOLD_DRY 12

#define YLIM_LOW  0
#define YLIM_HIGH 0.1

#define NONE 0
#define DONE 1

#define RESOLUTION_REGULATION 20
#define RESOLUTION_SENSOR 10

#define REGOUT_TAR_2_5 0//4433
#define REGOUT_TAR_7_5 0//12000

#define ID_WF_IMM 60
#define ID_WF_DRY 22



/********************************************************* Global varibles *********************************************************/
extern float arr_pSensor_conversionFactor_sf[NUM_SEVEN_FOLD];
extern float arr_pSensor_conversionFactor_tf[NUM_TWELVE_FOLD];

extern float arr_pSensor_compensationFactor_sf_bar_s[NUM_SEVEN_FOLD];
extern float arr_pSensor_compensationFactor_tf_bar_s[NUM_TWELVE_FOLD];


extern float thr_pressure_each_fin; // default thr : 2.5bar
extern float thr_pressure_each_ma;   // default thr : 2.5bar
extern float thr_pressure_all_fin;   // default thr : 2.5bar


extern bool flag_start_IMM;
extern bool flag_start_DRY;

extern int gl_sum_fold;
extern float gl_final_avg_data;
extern float gl_avgcur_sum;

extern int flag_timeEnd;
extern int flag_toMain;
extern int flag_failCur;

extern int cnt_loop;
extern int cnt_timer;
extern int cnt_timeOut_reg;

extern unsigned long time_cur;
extern int flag_timer_500ms;

extern int flag_DISP_deactivation;
extern int flag_HW_setting;
extern uint16_t analog_input_value;

extern uint16_t gl_STATE;
extern int gl_DEBUG_MODE;

extern int pt_start;
extern int pt_end;

extern float gl_ref_bar;
extern float gl_regOut_target;
extern uint16_t val_regulatorInput;
extern int flag_serialHMI;
extern int flag_init;
extern int flag_toMain_setting;
extern int gl_percent;

extern float wait_time_stbl_ms;
extern float wait_time_test_cnt_500ms;

/********************************************************* Function *********************************************************/
float calAve(float l_data_in, float cnt, float max_cnt);
float calAve_recursive(float l_data_in, float cnt, float l_ave_old);
float queue_MovAve(float l_data_in, float l_queue[], int queue_size, int pt_start, int pt_end);
float calMovAve(float l_data_in, float l_data_in_oldest, float num, float l_movAve);
float int_cal_sum_arr();
float int_cal_sum_arrFail();
float lin_Intp(float x0, float x1, float y0, float y1, float x_in);

void CheckFail(float l_thr);

void sendString(String sendData);

void sendData2HMI(float f_data_in, String str_obj_name);
void sendData2HMI_WF(int f_data_in);
void sendData2HMI_del_button(String str_fold_in, int int_idx_in);
void sendData2HMI_on_failbutton(String str_fold_in, int int_idx_in);
void sendData2HMI_on_nobutton(String str_fold_in, int int_idx_in);
void sendData2HMI_on_passbutton(String str_fold_in, int int_idx_in);
void sendData2HMI_Cur(float l_arr_data_sf_in[], int l_arr_stat_sf_in[], float l_arr_data_tf_in[], int l_arr_stat_tf_in[]);
void sendData2HMI_PRGRS(int i_data_in);
void sendData2HMI_PRGRS_STBL(int i_data_in);
void sendData2HMI_Fail(int l_arr_stat_sf_in[], int l_arr_stat_tf_in[]);
void sendData2HMI_Pass(int l_arr_stat_sf_in[], int l_arr_stat_tf_in[], int l_arr_stat_sf_p2_in[], int l_arr_stat_tf_p2_in[]);
void sendData2HMI_bt_off_on_all(String l_bt_name);
void sendData2HMI_bt_off_on(String l_bt_name, int l_idx);
void fT_sendData2HMI_idx(float l_f_data_in, String l_str_obj_name_in, int l_idx);

void fT_sendData2HMI(float l_f_data_in, String l_str_obj_name_in);
void fT_sendData2HMI_WF(float f_data_in);
float fT_sendData2HMI_AvgCur_WF(float l_arr_data_sf_in[], int l_arr_stat_sf_in[], float l_arr_data_tf_in[], int l_arr_stat_tf_in[]);

void send_Protocol_to_PLC();

void Serial_Mon(String l_str_name_in, float l_data_in);

float queue_MovAve_2D(float l_data_in, int l_idx, float l_queue[][PERIOD_MA], int queue_size);

void init_setup(ModbusMaster232 l_node_in);

void cal_recAve_All(int l_stat_sf_in[], float l_data_sf_in[], float l_aveRecursive_sf_in[], float l_aveRecursive_sf_old_in[], int l_stat_tf_in[], float l_data_tf_in[], float l_aveRecursive_tf_in[], float l_aveRecursive_tf_old_in[], int l_mode_in);
void cal_MovAve_Cur_All(float l_data_sf_in[], float l_data_sf_ma_in[], float l_data_sf_oldest_in[], float l_data_tf_in[], float l_data_tf_ma_in[], float l_data_tf_oldest_in[], int l_mode_in);



uint16_t get_val_resolution(uint16_t l_val_in, uint16_t l_resolution_in);

extern void wait_openAI(unsigned long duration);
#endif