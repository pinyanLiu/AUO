#ifndef __OPTIMIZATION_HPP__
#define __OPTIMIZATION_HPP__
#include "glpk.h"
#include "mysql_func.hpp"
#include "new_2d.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include<algorithm>
#include<charconv> 

class OPTIMIZE
{
public:

    typedef struct 
        {
            int time_block;
            long int vess_cess;
            float SOC_min;
            float SOC_max;
            float SOC_threshold;
            float ini_SOC;
            float now_SOC;
            long int P_bat_min;//the minimun power for battery discharging
            long int P_bat_max;//the maximun power for battery discharging
            double  P_grid_max;
            int real_time;
            int Global_next_simulate_timeblock;
            std::string simulate_price;
            int num_of_it_load;
            int num_of_ut_load;
            int num_of_vr_load;
            
        }EXPERIMENTAL_PARAMETERS;

    typedef struct 
        {
            int id;
            int group_id;
            int start_time;
            int end_time;
            int operation_time;//total operation time set by user
            int remain_op_time;
            int already_op_time;
            float max_power;
            std::string equip_name;
        }INTERRUPT_LOAD;
     typedef struct 
        {
            int id;
            int group_id;
            int start_time;
            int end_time;
            int operation_time;
            int remain_op_time;
            int already_op_time;
            float max_power;
            bool started_flag;
            std::string equip_name;
        }UNINTERRUPT_LOAD;
    typedef struct 
        {
            int id;
            int group_id;
            int start_time;
            int end_time;
            int operation_time;
            int remain_op_time;
            int already_op_time;
            std::vector<int> op_time_block[3];
            std::vector<float> power_block[3];
            bool started_flag;
            std::string equip_name;
        }VARYING_LOAD;
    

    EXPERIMENTAL_PARAMETERS ep;
    glp_prob* mip;
    int num_of_variable;
    double **coefficient;
    int Total_Row;
    int Total_Col;
    int bnd_row_num = 1;
    int remain_timeblock;//time block - sample time

    void cal_remain_timeblock();
    void cal_var_num();
    void cal_Total_Row();
    void cal_Total_Col();
    void get_EP(MYSQL_FUNC::EXPERIMENTAL_PARAMETERS EP); //get EP data from myql_func 
    void set_problem(char* prob_name,char* extremum);
    // --------- SET EACH COLUMN CONSTRAINTS FUNCS --------- //
    void set_interrupt_col();
    void set_uninterrupt_col();
    void set_varying_col();
    void set_Pgrid_col();
    void set_Psell_col();
    void set_Pess_col();
    void set_Pess_change_col();
    void set_Ppv_col();
    //========= SET EACH ROW CONSTRAINTS FUNCS --------------//
    void set_interrupt_row();
    void set_uninterrupt_row();
    void set_varying_row();

    //========= SET COEFFICIENT MATRIX  =====================//
    void set_interrupt_coeff();



public:
    std::vector<std::string> variable_name;
    OPTIMIZE(MYSQL_FUNC::EXPERIMENTAL_PARAMETERS EP,MYSQL_FUNC::GLOBAL_PLAN_FLAG GPF,MYSQL_FUNC::LOCAL_PLAN_FLAG LPF);
    ~OPTIMIZE();
    void set_variable_name(MYSQL_FUNC::GLOBAL_PLAN_FLAG GPF,MYSQL_FUNC::LOCAL_PLAN_FLAG LPF);//push variable under each situation
    void set_constraint_matrix(MYSQL_FUNC::GLOBAL_PLAN_FLAG GPF,MYSQL_FUNC::LOCAL_PLAN_FLAG LPF);//set whole row data,include balanced function and restricted function
    void set_row(MYSQL_FUNC::GLOBAL_PLAN_FLAG GPF,MYSQL_FUNC::LOCAL_PLAN_FLAG LPF);//set whole row constraint
    void set_col(MYSQL_FUNC::GLOBAL_PLAN_FLAG GPF,MYSQL_FUNC::LOCAL_PLAN_FLAG LPF);//set whole variable constraint
    void set_obj(MYSQL_FUNC::GLOBAL_PLAN_FLAG GPF,std::vector<float> price);//set objective function
    void outport_file();
    void get_IT(MYSQL_FUNC::INTERRUPT_LOAD* IT);//get interrupted load data from mysql_func
    void get_UT(MYSQL_FUNC::UNINTERRUPT_LOAD* UT);//get uninterrupted load data from mysql_func
    void get_VR(MYSQL_FUNC::VARYING_LOAD* VR);//get varying load data from mysql_func
    INTERRUPT_LOAD* interrupt_load;
    UNINTERRUPT_LOAD* uninterrupt_load;
    VARYING_LOAD* varying_load;

private:
    int find_variableName_position(std::string target);


};



#endif