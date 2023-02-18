
#include "Nextion.h"
#include <MsTimer2.h>
#include "Common_library.h"
#include "HMI_Declaration_DRY.h"
#include "HMI_Declaration_IMM.h"
#include "HMI_Func_DISP.h"
#include "HMI_Func_Callback.h"
#include "HW_Func.h"

#include <ModbusMaster232.h>
ModbusMaster232 node(1);

/********************************* Global variables ********************************/

// Set address for Main valve
uint16_t addr_mainValve_ON = 0x0056;
uint16_t addr_mainValve_OFF = 0x0057;

// Set address for Elec-regulator
uint16_t addr_elecRegulatorInput = 0x40000;
uint16_t addr_elecRegulatorOutput = 0x0BB8;


// Set address for IMM
// SF1   , SF2   , SF3   , SF4   , SF5   , SF6   , SF7
uint16_t arr_addr_P_Sensor_SF[NUM_SEVEN_FOLD] = { 0x012C, 0x00C8, 0x0640, 0x06A4, 0x05DC, 0x04B0, 0x0960 };
// TF1   , TF2   , TF3   , TF4   , TF5   , TF6   , TF7   , TF8   , TF9   , TF10  , TF11  , TF12
uint16_t arr_addr_P_Sensor_TF[NUM_TWELVE_FOLD] = { 0x0190, 0x0708, 0x01F4, 0x076C, 0x0258, 0x0898, 0x0320, 0x08FC, 0x0384, 0x02BC, 0x0578, 0x0514 };

// SF1   , SF2   , SF3   , SF4   , SF5   , SF6   , SF7
uint16_t arr_addr_Solenoid_ON_SF[NUM_SEVEN_FOLD] = { 0x0032, 0x0030, 0x0048, 0x004A, 0x0046, 0x0040, 0x0054 };
uint16_t arr_addr_Solenoid_OFF_SF[NUM_SEVEN_FOLD] = { 0x0033, 0x0031, 0x0049, 0x004B, 0x0047, 0x0041, 0x0055 };

// TF1   , TF2   , TF3   , TF4   , TF5   , TF6   , TF7   , TF8   , TF9   , TF10  , TF11  , TF12
uint16_t arr_addr_Solenoid_ON_TF[NUM_TWELVE_FOLD] = { 0x0034, 0x004C, 0x0036, 0x004E, 0x0038, 0x0050, 0x003C, 0x0052, 0x003E, 0x003A, 0x0044, 0x0042 };
uint16_t arr_addr_Solenoid_OFF_TF[NUM_TWELVE_FOLD] = { 0x0035, 0x004D, 0x0037, 0x004F, 0x0039, 0x0051, 0x003D, 0x0053, 0x003F, 0x003B, 0x0045, 0x0043 };




// Set address for DRY
// SF13  , SF14  , SF15  , SF16  , SF17
uint16_t arr_addr_P_Sensor_SF_dry[NUM_SEVEN_FOLD_DRY] = { 0x012C, 0x00C8, 0x0640, 0x06A4, 0x05DC };
// TF1   , TF2   , TF3   , TF4   , TF5   , TF6   , TF7   , TF8   , TF9   , TF10  , TF11  , TF12
uint16_t arr_addr_P_Sensor_TF_dry[NUM_TWELVE_FOLD_DRY] = { 0x0190, 0x0708, 0x01F4, 0x076C, 0x0258, 0x0898, 0x0320, 0x08FC, 0x0384, 0x04B0, 0x0514, 0x0578 };


//  SF13  , SF14  , SF15  , SF16  , SF17
uint16_t arr_addr_Solenoid_ON_SF_dry[NUM_SEVEN_FOLD_DRY] = { 0x0032, 0x0030, 0x0048, 0x004A, 0x0046 };
uint16_t arr_addr_Solenoid_OFF_SF_dry[NUM_SEVEN_FOLD_DRY] = { 0x0033, 0x0031, 0x0049, 0x004B, 0x0047 };

// TF1   , TF2   , TF3   , TF4   , TF5   , TF6   , TF7   , TF8   , TF9  , TF10  , TF11  , TF12
uint16_t arr_addr_Solenoid_ON_TF_dry[NUM_TWELVE_FOLD_DRY] = { 0x0034, 0x004C, 0x0036, 0x004E, 0x0038, 0x0050, 0x003C, 0x0052, 0x003E, 0x0040, 0x0042, 0x0044 };
uint16_t arr_addr_Solenoid_OFF_TF_dry[NUM_TWELVE_FOLD_DRY] = { 0x0035, 0x004D, 0x0037, 0x004F, 0x0039, 0x0051, 0x003D, 0x0053, 0x003F, 0x0041, 0x0043, 0x0045 };


// P-Sensor Conversion factor

/********************************* Functions ********************************/
void send_Protocol_to_PLC();
float queue_MovAve_2D(float l_data_in, int l_idx, float l_queue[][PERIOD_MA], int queue_size);
void get_Signal_Pressure(float l_data_in[], int l_num_hose, ModbusMaster232 l_node_in, uint16_t l_addr_in);
void set_elecRegulator(ModbusMaster232 l_node_in, uint16_t l_addr_elec_reg_w_in, uint16_t l_ui_data_in, uint16_t l_addr_elec_reg_r_in);
void set_Solenoid(ModbusMaster232 l_node_in);
void init_Solenoid(ModbusMaster232 l_node_in);
void close_Solenoid(ModbusMaster232 l_node_in);
void off_Solenoid(ModbusMaster232 l_node_in);
void open_Solenoids(ModbusMaster232 l_node_in);
void init_MainValve(ModbusMaster232 l_node_in);
void set_MainValve(ModbusMaster232 l_node_in, uint16_t l_arr_addr_mainvalve_on, uint16_t l_arr_addr_mainvalve_off);
void init_Elec_Regulator(ModbusMaster232 l_node_in, uint16_t l_addr_elec_reg_w_in);
void off_Elec_Regulator(ModbusMaster232 l_node_in, uint16_t l_addr_elec_reg_w_in);
void Param_init();
void stabilization();
void init_hardWare();
void set_leakTestMode();
void set_hardWare();
void init_HMI_Disp();
void set_HMI_Disp();
void set_HMI_Disp_Done();
void get_refPresure();

/*********************************** Initialization **********************************/
void setup() {
  nexInit();
  init_setup(node);
  gl_DEBUG_MODE = W_PLC;
}

/******************** Main ********************/
void loop() {
  nexLoop(nex_listen_list);
  if(gl_DEBUG_MODE == 1)
  {
    init_setup(node);
    init_hardWare();
    gl_DEBUG_MODE = 0;
  }

  // Initialization
  if (flag_timeEnd == 1U || flag_toMain == 1U || flag_failCur == 1U || flag_toMain_setting == 1U) {
    if(flag_init == OFF)
    {
      init_hardWare();
    }

    if (flag_timeEnd == 1U) {
      if (gl_STATE == RUN_IMM || gl_STATE == RUN_DRY) {
        /*********** [CHECK FAIL] final all average *************/
        if (gl_final_avg_data > thr_pressure_all_fin) {
          tResult.setText("FAIL");
        } else {
          tResult.setText("PASS");
        }
      } else {
        HMI_DISP_fail_BUTTON("bt_all", 0, FAIL);
      }
      tProgress.setText("COMPLETE LEAK TEST");
      Serial.println("COMPLETE LEAK TEST.");
    }
    else if (flag_failCur == 1U)
    {
      tProgress.setText("FAIL LEAK TEST");
      Serial.println("FAIL LEAK TEST");
    }
    Param_init();
  }
  
  if (flag_timer_500ms == 1U && (flag_start_IMM == 1 || flag_start_DRY == 1)) {
    flag_timer_500ms = 0U;
    
    if (gl_STATE == INIT_HARDWARE)
    {
      init_hardWare();
      gl_STATE = SET_TEST_MODE;
    }
    else if (gl_STATE == SET_TEST_MODE)
    {
      set_leakTestMode();
    }
    else if (gl_STATE == RUN_IMM || gl_STATE == RUN_DRY ) {
      if (flag_HW_setting == NONE) {
        set_hardWare();
        flag_HW_setting = DONE;
        init_HMI_Disp();
        get_Signal_Pressure_All(node);
        get_refPresure();
        tResult.setText("Leak testing...");
      }
      
      
      // After done setting HW
      else if(flag_HW_setting == DONE && flag_serialHMI == OFF) {
        /************  1. [Get data]   **************/
        get_Signal_Pressure_All(node);
        set_HMI_Disp();
        /*********** 2 [CHECK FAIL]  *************/
        CheckFail(thr_pressure_each_ma);
        /*********** 3 [DISPLAY HMI]  *************/
        set_HMI_Disp_Done();
        cnt_loop++;
        pt_start++;
        pt_end++;

        if (flag_failCur == 0U) {
          if (cnt_timer > (int)wait_time_test_cnt_500ms) { cnt_timer = (int)wait_time_test_cnt_500ms; }
          sendData2HMI_PRGRS(cnt_timer);
          HMI_DISP_Time((millis() - time_cur) * 0.001, "tProgress");
          gl_percent = (int)(((float)cnt_timer / (float)wait_time_test_cnt_500ms) * 100);
          if(gl_percent > 100)
          {
            gl_percent = 100;
          }
          HMI_DISP_onText("tPRGRS_per", 0, gl_percent);
        }
      }
      else
      {
        Serial.println("Back to Main");
        flag_toMain = 1U;
      }
    }
  }
}

void send_Protocol_to_PLC() {
  // Request PLC to start measure
  flag_timer_500ms = 1U;

  if (flag_HW_setting == DONE) {
    cnt_timer++;
  }
}

void off_Elec_Regulator(ModbusMaster232 l_node_in, uint16_t l_addr_elec_reg_w_in) {
  tProgress.setText("Turning off Regulator...");
  Serial.println("Turning off Regulator...");

  l_node_in.writeSingleRegister(l_addr_elec_reg_w_in, 0);
}


void set_Solenoid(ModbusMaster232 l_node_in) {
  int l_mode_idx_sf = 0U;
  int l_mode_idx_tf = 0U;


  tProgress.setText("Setting Solenoids...");
  Serial.println("Setting Solenoids...");

  if(flag_start_IMM == 1U)
  {
    // SOL OPEN(RED)
    for (int idx = 0U; idx < NUM_SEVEN_FOLD; idx++)
    {
      if (stat_sf[idx] == 1U) {
        l_node_in.writeSingleCoil(arr_addr_Solenoid_ON_SF[idx], SOLENOID_OFF);
        l_node_in.writeSingleCoil(arr_addr_Solenoid_OFF_SF[idx], SOLENOID_ON);
      } else {
        l_node_in.writeSingleCoil(arr_addr_Solenoid_OFF_SF[idx], SOLENOID_OFF);
        l_node_in.writeSingleCoil(arr_addr_Solenoid_ON_SF[idx], SOLENOID_ON);
      }
    }
    for (int idx = 0U; idx < NUM_TWELVE_FOLD; idx++) {
      if (stat_tf[idx] == 1U) {
        l_node_in.writeSingleCoil(arr_addr_Solenoid_ON_TF[idx], SOLENOID_OFF);
        l_node_in.writeSingleCoil(arr_addr_Solenoid_OFF_TF[idx], SOLENOID_ON);
      } else {
        l_node_in.writeSingleCoil(arr_addr_Solenoid_OFF_TF[idx], SOLENOID_OFF);
        l_node_in.writeSingleCoil(arr_addr_Solenoid_ON_TF[idx], SOLENOID_ON);
      }
    }
  }
  else if(flag_start_DRY == 1U)
  {
    // SOL OPEN(RED)
    for (int idx = 0U; idx < NUM_SEVEN_FOLD_DRY; idx++)
    {
      if (stat_sf_dry[idx] == 1U) {
        l_node_in.writeSingleCoil(arr_addr_Solenoid_ON_SF_dry[idx], SOLENOID_OFF);
        l_node_in.writeSingleCoil(arr_addr_Solenoid_OFF_SF_dry[idx], SOLENOID_ON);
      } else {
        l_node_in.writeSingleCoil(arr_addr_Solenoid_OFF_SF_dry[idx], SOLENOID_OFF);
        l_node_in.writeSingleCoil(arr_addr_Solenoid_ON_SF_dry[idx], SOLENOID_ON);
      }
    }
    for (int idx = 0U; idx < NUM_TWELVE_FOLD_DRY; idx++) {
      if (stat_tf_dry[idx] == 1U) {
        l_node_in.writeSingleCoil(arr_addr_Solenoid_ON_TF_dry[idx], SOLENOID_OFF);
        l_node_in.writeSingleCoil(arr_addr_Solenoid_OFF_TF_dry[idx], SOLENOID_ON);
      } else {
        l_node_in.writeSingleCoil(arr_addr_Solenoid_OFF_TF_dry[idx], SOLENOID_OFF);
        l_node_in.writeSingleCoil(arr_addr_Solenoid_ON_TF_dry[idx], SOLENOID_ON);
      }
    }
  }

  l_node_in.clearResponseBuffer();
}


void set_elecRegulator(ModbusMaster232 l_node_in, uint16_t l_addr_elec_reg_w_in, uint16_t l_ui_data_in, uint16_t l_addr_elec_reg_r_in) {
  uint8_t output_regul = 0U;

  static int flag_start_setRegul = 0U;

  if (flag_start_setRegul == 0U) {
    tProgress.setText("Setting Regulator...");
    Serial.println("Setting Regulator...");
    wait_openAI(WAIT_TIME_HARDWARE);

    flag_start_setRegul = 1U;

    l_node_in.writeSingleRegister(l_addr_elec_reg_w_in, l_ui_data_in);
  }
  if(flag_start_setRegul == 1U)
  {
    cnt_timeOut_reg++;
    output_regul = get_val_resolution(l_node_in.readInputRegisters(l_addr_elec_reg_r_in, 1), RESOLUTION_REGULATION);

    if (output_regul > 65000) {
      output_regul = 0U;
    }
    l_node_in.clearResponseBuffer();

    Serial_Mon("output_regul : ", output_regul);
    Serial_Mon("cnt_timeOut_reg : ", cnt_timeOut_reg);

    if (output_regul >= gl_regOut_target) {
      tProgress.setText("Stabilizing before closing solenoids...");
      Serial.println("Stabilizing before closing solenoids...");
      wait_openAI(WAIT_TIME_STABILIZATION_BF_CLOSE_SOL);
      Serial.println("Done to stabilization");

      flag_start_setRegul = 0U;

    } else if (cnt_timeOut_reg > TIMEOUT_STABILIZATION) {
      cnt_timeOut_reg = 0U;
      flag_start_setRegul = 0U;
      flag_failCur = 1U;

      Serial.println("TimeOut");
      tProgress.setText("Time out for setting Regulator.");
    }
  }

  //l_node_in.clearResponseBuffer();
}

void close_Solenoid(ModbusMaster232 l_node_in) {

  tProgress.setText("Closing Solenoids...");
  Serial.println("Closing Solenoids...");

  // SOL CLOSE(GREEN)
  if(flag_start_IMM == 1U)
  {
    for (int idx = 0U; idx < NUM_SEVEN_FOLD; idx++)
    {
      l_node_in.writeSingleCoil(arr_addr_Solenoid_OFF_SF[idx], SOLENOID_OFF);
      l_node_in.writeSingleCoil(arr_addr_Solenoid_ON_SF[idx], SOLENOID_ON);
    }
    for (int idx = 0U; idx < NUM_TWELVE_FOLD; idx++) {
      l_node_in.writeSingleCoil(arr_addr_Solenoid_OFF_TF[idx], SOLENOID_OFF);
      l_node_in.writeSingleCoil(arr_addr_Solenoid_ON_TF[idx], SOLENOID_ON);
    }
  }
  else if(flag_start_DRY == 1U)
  {
    for (int idx = 0U; idx < NUM_SEVEN_FOLD_DRY; idx++)
    {
      l_node_in.writeSingleCoil(arr_addr_Solenoid_OFF_SF_dry[idx], SOLENOID_OFF);
      l_node_in.writeSingleCoil(arr_addr_Solenoid_ON_SF_dry[idx], SOLENOID_ON);
    }
    for (int idx = 0U; idx < NUM_TWELVE_FOLD_DRY; idx++) {
      l_node_in.writeSingleCoil(arr_addr_Solenoid_OFF_TF_dry[idx], SOLENOID_OFF);
      l_node_in.writeSingleCoil(arr_addr_Solenoid_ON_TF_dry[idx], SOLENOID_ON);
    }
  }

  // Waiting for closing sol
  wait_openAI(WAIT_TIME_HARDWARE*2);
}


void off_Solenoids(ModbusMaster232 l_node_in)
{
  // SOL OFF(NO LIGHT)
  if(flag_start_IMM == 1U)
  {
    for (int idx = 0U; idx < NUM_SEVEN_FOLD; idx++)
    {
      l_node_in.writeSingleCoil(arr_addr_Solenoid_OFF_SF[idx], SOLENOID_OFF);
      l_node_in.writeSingleCoil(arr_addr_Solenoid_ON_SF[idx], SOLENOID_OFF);
    }
    for (int idx = 0U; idx < NUM_TWELVE_FOLD; idx++) {
      l_node_in.writeSingleCoil(arr_addr_Solenoid_OFF_TF[idx], SOLENOID_OFF);
      l_node_in.writeSingleCoil(arr_addr_Solenoid_ON_TF[idx], SOLENOID_OFF);
    }
  }
  else if(flag_start_DRY == 1U)
  {
    for (int idx = 0U; idx < NUM_SEVEN_FOLD_DRY; idx++)
    {
      l_node_in.writeSingleCoil(arr_addr_Solenoid_OFF_SF_dry[idx], SOLENOID_OFF);
      l_node_in.writeSingleCoil(arr_addr_Solenoid_ON_SF_dry[idx], SOLENOID_OFF);
    }
    for (int idx = 0U; idx < NUM_TWELVE_FOLD_DRY; idx++) {
      l_node_in.writeSingleCoil(arr_addr_Solenoid_OFF_TF_dry[idx], SOLENOID_OFF);
      l_node_in.writeSingleCoil(arr_addr_Solenoid_ON_TF_dry[idx], SOLENOID_OFF);
    }
  }
  wait_openAI(WAIT_TIME_HARDWARE);
}

void open_Solenoids(ModbusMaster232 l_node_in)
{
  // SOL ON(Red)
  if(flag_start_IMM == 1U)
  {
    for (int idx = 0U; idx < NUM_SEVEN_FOLD; idx++)
    {
      l_node_in.writeSingleCoil(arr_addr_Solenoid_ON_TF[idx], SOLENOID_OFF);
      l_node_in.writeSingleCoil(arr_addr_Solenoid_OFF_TF[idx], SOLENOID_ON);
    }
    for (int idx = 0U; idx < NUM_TWELVE_FOLD; idx++) {
      l_node_in.writeSingleCoil(arr_addr_Solenoid_OFF_TF[idx], SOLENOID_ON);
      l_node_in.writeSingleCoil(arr_addr_Solenoid_ON_TF[idx], SOLENOID_OFF);
    }
  }
  else if(flag_start_DRY == 1U)
  {
    for (int idx = 0U; idx < NUM_SEVEN_FOLD_DRY; idx++)
    {
      l_node_in.writeSingleCoil(arr_addr_Solenoid_OFF_SF_dry[idx], SOLENOID_ON);
      l_node_in.writeSingleCoil(arr_addr_Solenoid_ON_SF_dry[idx], SOLENOID_OFF);
    }
    for (int idx = 0U; idx < NUM_TWELVE_FOLD_DRY; idx++) {
      l_node_in.writeSingleCoil(arr_addr_Solenoid_OFF_TF_dry[idx], SOLENOID_ON);
      l_node_in.writeSingleCoil(arr_addr_Solenoid_ON_TF_dry[idx], SOLENOID_OFF);
    }
  }
  wait_openAI(WAIT_TIME_HARDWARE);
}

void stabilization()
{
  tProgress.setText("Stabilizing after closing solenoids...");
  Serial.println("Stabilizing after closing solenoids...");

  tResult.setText("Stabilizing...");
  wait_openAI(wait_time_stbl_ms);
}

void init_MainValve(ModbusMaster232 l_node_in) {
  tProgress.setText("Initializing Main valve...");
  Serial.println("Initializing Main valve...");
  wait_openAI(WAIT_TIME_HARDWARE);

  // main valve
  l_node_in.writeSingleCoil(addr_mainValve_ON, MAIN_VALVE_OFF);
  l_node_in.writeSingleCoil(addr_mainValve_OFF, MAIN_VALVE_ON);
}

void init_Solenoid(ModbusMaster232 l_node_in) {
  tProgress.setText("Initializing Solenoids...");
  Serial.println("Initializing Solenoids...");
  wait_openAI(WAIT_TIME_HARDWARE);


  // SOL OPEN(RED)
  for (int idx = 0U; idx < NUM_SEVEN_FOLD; idx++) {
    l_node_in.writeSingleCoil(arr_addr_Solenoid_OFF_SF[idx], SOLENOID_OFF);
    l_node_in.writeSingleCoil(arr_addr_Solenoid_ON_SF[idx], SOLENOID_ON);
  }
  for (int idx = 0U; idx < NUM_TWELVE_FOLD; idx++) {
    l_node_in.writeSingleCoil(arr_addr_Solenoid_OFF_TF[idx], SOLENOID_OFF);
    l_node_in.writeSingleCoil(arr_addr_Solenoid_ON_TF[idx], SOLENOID_ON);
  }
  tProgress.setText("Waiting for benting air...");
  wait_openAI(WAIT_TIME_HARDWARE_BENT);


  // SOL CLOSE(GREEN)
  for (int idx = 0U; idx < NUM_SEVEN_FOLD; idx++) {
    l_node_in.writeSingleCoil(arr_addr_Solenoid_OFF_SF[idx], SOLENOID_ON);
    l_node_in.writeSingleCoil(arr_addr_Solenoid_ON_SF[idx], SOLENOID_OFF);
  }
  for (int idx = 0U; idx < NUM_TWELVE_FOLD; idx++) {
    l_node_in.writeSingleCoil(arr_addr_Solenoid_OFF_TF[idx], SOLENOID_ON);
    l_node_in.writeSingleCoil(arr_addr_Solenoid_ON_TF[idx], SOLENOID_OFF);
  }
  
  if(flag_start_DRY == 1U) {
    l_node_in.writeSingleCoil(arr_addr_Solenoid_OFF_SF[6], SOLENOID_OFF);
    l_node_in.writeSingleCoil(arr_addr_Solenoid_ON_SF[6], SOLENOID_ON);

    l_node_in.writeSingleCoil(arr_addr_Solenoid_OFF_TF[9], SOLENOID_OFF);
    l_node_in.writeSingleCoil(arr_addr_Solenoid_ON_TF[9], SOLENOID_ON);
  }
}


void init_Elec_Regulator(ModbusMaster232 l_node_in, uint16_t l_addr_elec_reg_w_in) {
  tProgress.setText("Initializing Regulator...");
  Serial.println("Initializing Regulator...");
  wait_openAI(WAIT_TIME_HARDWARE);

  l_node_in.writeSingleRegister(l_addr_elec_reg_w_in, 0);
}


void set_MainValve(ModbusMaster232 l_node_in, uint16_t l_arr_addr_mainvalve_on, uint16_t l_arr_addr_mainvalve_off) {
  tProgress.setText("Setting Main valve...");
  Serial.println("Setting Main valve...");
  wait_openAI(WAIT_TIME_HARDWARE*2);


  //main open
  l_node_in.writeSingleCoil(l_arr_addr_mainvalve_on, MAIN_VALVE_OFF);
  l_node_in.writeSingleCoil(l_arr_addr_mainvalve_off, MAIN_VALVE_ON);

  //main close
  // l_node_in.writeSingleCoil(l_arr_addr_mainvalve_on, MAIN_VALVE_ON);
  // l_node_in.writeSingleCoil(l_arr_addr_mainvalve_off, MAIN_VALVE_OFF);
}


void Param_init() {
  Serial.println("****************Initialization****************");
  //init_Elec_Regulator(node, addr_elecRegulatorInput);
  
  flag_serialHMI = OFF;
  flag_toMain_setting = OFF;

  
  wait_time_stbl_ms = 15000;
  wait_time_test_cnt_500ms = 60;


  MsTimer2::stop();

  flag_start_IMM = 0;
  flag_start_DRY = 0;
  cnt_loop = 1U;
  cnt_timer = 1U;
  cnt_timeOut_reg = 0U;

  gl_sum_fold = 0;
  time_cur = 0;
  gl_STATE = INIT_HARDWARE;
  flag_timeEnd = 0U;
  flag_toMain = 0U;
  flag_failCur = 0U;
  gl_avgcur_sum = 0.0f;

  // initialization HMI buttons
  stat_2_5_bar = 0;
  stat_7_5_bar = 0;

  stat_2_5_bar_dry = 0;
  stat_7_5_bar_dry = 0;

  flag_HW_setting = NONE;
  gl_DEBUG_MODE = W_PLC;

  flag_DISP_deactivation = OFF;

  for (int idx = 0U; idx < NUM_SEVEN_FOLD; idx++) {
    stat_sf[idx] = 0;
  }
  for (int idx = 0U; idx < NUM_TWELVE_FOLD; idx++) {
    stat_tf[idx] = 0;
  }

  for (int idx = 0U; idx < NUM_SEVEN_FOLD; idx++) {
    data_sf[idx] = 0;
  }
  for (int idx = 0U; idx < NUM_TWELVE_FOLD; idx++) {
    data_tf[idx] = 0;
  }

  for (int idx = 0U; idx < NUM_SEVEN_FOLD; idx++) {
    stat_sf_p2[idx] = 0;
  }
  for (int idx = 0U; idx < NUM_TWELVE_FOLD; idx++) {
    stat_tf_p2[idx] = 0;
  }

  // initialization fail stat cur
  for (int idx = 0U; idx < NUM_SEVEN_FOLD; idx++) {
    fail_stat_sf_cur[idx] = 0;
  }
  for (int idx = 0U; idx < NUM_TWELVE_FOLD; idx++) {
    fail_stat_tf_cur[idx] = 0;
  }

  // initialization fail stat fin
  for (int idx = 0U; idx < NUM_SEVEN_FOLD; idx++) {
    fail_stat_sf_fin[idx] = 0;
  }
  for (int idx = 0U; idx < NUM_TWELVE_FOLD; idx++) {
    fail_stat_tf_fin[idx] = 0;
  }

  for (int idx = 0U; idx < NUM_SEVEN_FOLD_DRY; idx++) {
    stat_sf_dry[idx] = 0;
  }
  for (int idx = 0U; idx < NUM_TWELVE_FOLD_DRY; idx++) {
    stat_tf_dry[idx] = 0;
  }
  for (int idx = 0U; idx < NUM_SEVEN_FOLD_DRY; idx++) {
    data_sf_dry[idx] = 0;
  }
  for (int idx = 0U; idx < NUM_TWELVE_FOLD_DRY; idx++) {
    data_tf_dry[idx] = 0;
  }

  for (int idx = 0U; idx < NUM_SEVEN_FOLD; idx++) {
    refPresure_sf[idx] = 0;
  }
  for (int idx = 0U; idx < NUM_TWELVE_FOLD; idx++) {
    refPresure_tf[idx] = 0;
  }

  for (int idx = 0U; idx < NUM_SEVEN_FOLD_DRY; idx++) {
    refPresure_sf_dry[idx] = 0;
  }
  for (int idx = 0U; idx < NUM_TWELVE_FOLD_DRY; idx++) {
    refPresure_tf_dry[idx] = 0;
  }



  

  for (int idx = 0U; idx < NUM_SEVEN_FOLD_DRY; idx++) {
    stat_sf_p2_dry[idx] = 0;
  }
  for (int idx = 0U; idx < NUM_TWELVE_FOLD_DRY; idx++) {
    stat_tf_p2_dry[idx] = 0;
  }

  // initialization fail stat cur
  for (int idx = 0U; idx < NUM_SEVEN_FOLD_DRY; idx++) {
    fail_stat_sf_cur_dry[idx] = 0;
  }
  for (int idx = 0U; idx < NUM_TWELVE_FOLD_DRY; idx++) {
    fail_stat_tf_cur_dry[idx] = 0;
  }

  // initialization fail stat fin
  for (int idx = 0U; idx < NUM_SEVEN_FOLD_DRY; idx++) {
    fail_stat_sf_fin_dry[idx] = 0;
  }
  for (int idx = 0U; idx < NUM_TWELVE_FOLD_DRY; idx++) {
    fail_stat_tf_fin_dry[idx] = 0;
  }

  ////


  for (int idx = 0U; idx < NUM_SEVEN_FOLD; idx++) {
    data_sf_ma[idx] = 0;
  }
  for (int idx = 0U; idx < NUM_SEVEN_FOLD; idx++) {
    data_sf_oldest[idx] = 0;
  }
  for (int idx = 0U; idx < NUM_TWELVE_FOLD; idx++) {
    data_tf_ma[idx] = 0;
  }
  for (int idx = 0U; idx < NUM_TWELVE_FOLD; idx++) {
    data_tf_oldest[idx] = 0;
  }
  for (int idx = 0U; idx < NUM_SEVEN_FOLD; idx++) {
    for (int jdx = 0U; jdx < PERIOD_MA; jdx++) {
      data_sf_queue[idx][jdx] = 0;
    }
  }
  for (int idx = 0U; idx < NUM_TWELVE_FOLD; idx++) {
    for (int jdx = 0U; jdx < PERIOD_MA; jdx++) {
      data_tf_queue[idx][jdx] = 0;
    }
  }
  for (int idx = 0U; idx < NUM_SEVEN_FOLD_DRY; idx++) {
    data_sf_ma_dry[idx] = 0;
  }
  for (int idx = 0U; idx < NUM_SEVEN_FOLD_DRY; idx++) {
    data_sf_oldest_dry[idx] = 0;
  }
  for (int idx = 0U; idx < NUM_TWELVE_FOLD_DRY; idx++) {
    data_tf_ma_dry[idx] = 0;
  }
  for (int idx = 0U; idx < NUM_TWELVE_FOLD_DRY; idx++) {
    data_tf_oldest_dry[idx] = 0;
  }
  for (int idx = 0U; idx < NUM_SEVEN_FOLD_DRY; idx++) {
    for (int jdx = 0U; jdx < PERIOD_MA; jdx++) {
      data_sf_queue_dry[idx][jdx] = 0;
    }
  }
  for (int idx = 0U; idx < NUM_TWELVE_FOLD_DRY; idx++) {
    for (int jdx = 0U; jdx < PERIOD_MA; jdx++) {
      data_tf_queue_dry[idx][jdx] = 0;
    }
  }
}


void init_hardWare()
{
  tStatus.setText("Status : "); 
  tProgress.setText("Initializing Hardware...");
  Serial.println("Initializing Hardware...");
  wait_openAI(WAIT_TIME_HARDWARE);
  
  init_Solenoid(node);  // sol valve open
  init_MainValve(node); // main valve open
  
  init_Elec_Regulator(node, addr_elecRegulatorInput);         // regulator zero

  tProgress.setText("Complete to init Hardware");
  Serial.println("Complete to init Hardware");
  wait_openAI(WAIT_TIME_HARDWARE);
  flag_init = ON;
}

void set_leakTestMode()
{
  if (flag_start_IMM == ON && flag_start_DRY == ON) {
    gl_STATE = RUN_DIAGNOSTIC;
  } else if (flag_start_DRY == ON) {
    gl_STATE = RUN_DRY;
  } else if (flag_start_IMM == ON) {
    gl_STATE = RUN_IMM;
  }
}

void set_hardWare()
{
  flag_init = OFF;
  close_Solenoid(node);
  set_elecRegulator(node, addr_elecRegulatorInput, val_regulatorInput, addr_elecRegulatorOutput);
  set_Solenoid(node);
  wait_openAI(WAIT_TIME_STABILIZATION_BF_CLOSE_SOL);
  close_Solenoid(node);
  off_Solenoids(node);
  off_Elec_Regulator(node, addr_elecRegulatorInput);
  HMI_DISP_vis_BUTTON("b", 0, ON);
  stabilization();

  tProgress.setText("Complete to set Hardware");
  Serial.println("Complete to set Hardware");
  wait_openAI(WAIT_TIME_HARDWARE);
}

void init_HMI_Disp()
{
  tStatus.setText("Duration time : ");
  time_cur = millis();

  
  HMI_DISP_vis_BUTTON("tPercent", 0, ON);
}

void set_HMI_Disp()
{
  if (flag_DISP_deactivation == OFF) {
    HMI_DISP_deactivation_BUTTON_ALL("btN");
    flag_DISP_deactivation = ON;
  }

  gl_avgcur_sum = gl_avgcur_sum + HMI_DISP_AvgCur_WF();
  
  if(cnt_loop % period_realTimeMonitoring_cnt_500ms == 0 && flag_realTimeMonitoring == ON)
  {
    HMI_DISP_Cur_PRS_ALL();
  }
}

void set_HMI_Disp_Done()
{
  // if fail
  if (int_cal_sum_arrFail() > 0) {
    flag_failCur = 1U;
  }
  if (flag_failCur == 1U) {
    HMI_DISP_vis_BUTTON_ALL();
    HMI_DISP_fail_BUTTON_ALL("bt");
    HMI_DISP_Cur_PRS_ALL();

    tResult.setText("FAIL");
    Serial_Mon("flag_failCur :", flag_failCur);

    tStatus.setText("Status : ");
    tProgress.setText("Fail : Current value(each hose)");
  }
  // After Finishing(PERIOD_ALL)
  else if (cnt_timer > (int)wait_time_test_cnt_500ms) {
    HMI_DISP_Cur_PRS_ALL();
    Serial_Mon("cnt_loop final", cnt_loop);
    Serial_Mon("cnt_timer final", cnt_timer);

    // Calculation all avg
    gl_final_avg_data = gl_avgcur_sum / cnt_loop;
    flag_timeEnd = 1U;

    // Average : Send average data to HMI
    fT_sendData2HMI(gl_final_avg_data, "tAVG");

    /*********** 2-2 [CHECK FAIL] for each final average pressure *************/
    CheckFail(thr_pressure_each_fin);
    HMI_DISP_vis_BUTTON_ALL();
    HMI_DISP_fail_BUTTON_ALL("bt");
  }
}

void get_refPresure()
{
  if(flag_start_IMM == 1U)
  {
    for (int idx = 0U ; idx < NUM_SEVEN_FOLD; idx++)
    {
      if(stat_sf[idx] == 1)
      {
        refPresure_sf[idx] = -data_sf[idx];
        Serial_Mon("refPresure_sf[idx]", refPresure_sf[idx]);
      }
    }
    for (int idx = 0U ; idx < NUM_TWELVE_FOLD; idx++)
    {
      if(stat_tf[idx] == 1)
      {
        refPresure_tf[idx] = -data_tf[idx];
        
        Serial_Mon("refPresure_tf[idx]", refPresure_tf[idx]);
      }
    }
  }
  else if(flag_start_DRY == 1U)
  {
    for (int idx = 0U ; idx < NUM_SEVEN_FOLD_DRY; idx++)
    {
      if(stat_sf_dry[idx] == 1)
      {
        refPresure_sf_dry[idx] = -data_sf_dry[idx];
        Serial_Mon("refPresure_sf_dry[idx]", refPresure_sf_dry[idx]);
      }
    }
    for (int idx = 0U ; idx < NUM_TWELVE_FOLD_DRY; idx++)
    {
      if(stat_tf_dry[idx] == 1)
      {
        refPresure_tf_dry[idx] = -data_tf_dry[idx];
        Serial_Mon("refPresure_tf_dry[idx]", refPresure_tf_dry[idx]);
      }
    }
  }
}