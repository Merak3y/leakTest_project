#ifndef MyFuncDryLibrary_H
#define MyFuncDryLibrary_H


// EXTERN VARIABLE


// FUNCTION
void HMI_DISP_vis_BUTTON(String str_hose_name_in, int l_int_hose_idx_in, int l_vis_flag_in);
void HMI_DISP_fail_BUTTON(String str_hose_name_in, int l_int_hose_idx_in, int l_fail_flag_in);
void HMI_DISP_deactivation_BUTTON(String str_hose_name_in, int l_int_hose_idx_in, int l_activation_flag_in);
void HMI_DISP_onText(String l_str_obj_name_in, int l_hose_idx, float l_f_data_in);


void HMI_DISP_fail_BUTTON_ALL(String l_str_hose_name_in);
void HMI_DISP_Cur_PRS_ALL();
void HMI_DISP_vis_BUTTON_ALL();
void HMI_DISP_deactivation_BUTTON_ALL(String str_hose_name_in);

void HMI_DISP_WF(int l_int_data_in, int l_wf_id_in);
float HMI_DISP_AvgCur_WF();

void HMI_DISP_Time(float l_f_data_in, String l_str_obj_name_in);



#endif