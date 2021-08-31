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

class OPTIMIZE
{
private:

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
            
        }EXPERIMENTAL_PARAMETERS;
    
    EXPERIMENTAL_PARAMETERS ep;
    glp_prob* mip;
    int num_of_variable;
    double **coefficient;
    int Total_Row;
    int Total_Col;
    int coef_row_num = 0, bnd_row_num = 1;
    int remain_timeblock;//time block - sample time

    void cal_remain_timeblock();
    void cal_var_num();
    void cal_Total_Row();
    void cal_Total_Col();
    void get_EP(MYSQL_FUNC::EXPERIMENTAL_PARAMETERS EP);
    // --------- SET COLUMN FUNCS --------- //
    void set_Pgrid_col();
    void set_Psell_col();
    void set_Pess_col();
    void set_Pess_change_col();
    void set_Ppv_col();


public:
    std::vector<std::string> variable_name;
    OPTIMIZE(char* prob_name,char* extremum,MYSQL_FUNC::EXPERIMENTAL_PARAMETERS EP,MYSQL_FUNC::PLAN_FLAG PF);
    ~OPTIMIZE();
    void set_variable_name(MYSQL_FUNC::PLAN_FLAG PF);//push variable under each situation
    void set_obj(MYSQL_FUNC::PLAN_FLAG PF);//set objective function
    void set_constraint_matrix(MYSQL_FUNC::PLAN_FLAG PF);//set whole row,include balanced function and restricted function
    void set_col(MYSQL_FUNC::PLAN_FLAG PF);//set whole variable constraint
    void set_cal_parm();//set the calculation method and parameter
    void outport_file();

private:
    int find_variableName_position(std::string target);


};



#endif