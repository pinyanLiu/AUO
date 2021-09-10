//-------------------------------------------
//This lib is the commands based on mysql_lib
//-------------------------------------------

#ifndef __MYSQL_FUNC_HPP__
#define __MYSQL_FUNC_HPP__
#include <stdio.h> // c 輸入輸出
#include <stdlib.h> //數值與字符串轉換函數, 偽隨機數生成函數, 動態內存分配函數, 進程控制函數
#include <string.h>
#include <iostream> //c++ 的輸入輸出
#include <vector>
#include "mysql_lib.hpp"

class MYSQL_FUNC
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
            int num_of_it_load;
            int num_of_ut_load;
            int num_of_vr_load;
            std::string simulate_price;
            
        }EXPERIMENTAL_PARAMETERS;
        
        typedef struct 
        {
            bool Pgrid;
            bool PV;
            bool FC;
            bool Pess;
            bool Sell;
            bool DR;
            bool Comfort;
            bool SOC_change;//for limiting battery has to discharge at least 80% per day.
        }GLOBAL_PLAN_FLAG;

        typedef struct 
        {
            bool interrupt;
            bool uninterrupt;
            bool varying;
        }LOCAL_PLAN_FLAG;
        

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
            int op_time_block[3];
            float power_block[3];
            bool started_flag;
            std::string equip_name;
        }VARYING_LOAD;
        

        std::vector<int> load_model;
        std::vector<float> price;
        EXPERIMENTAL_PARAMETERS ep;//ep for EXPERIMENTAL_PARAMETERS
        GLOBAL_PLAN_FLAG gpf;//gpf for GLOBAL_PLAN_FLAG
        LOCAL_PLAN_FLAG lpf;//lpf for LOCAL_PLAN_FLAG
        INTERRUPT_LOAD *interrupt_load;
        UNINTERRUPT_LOAD* uninterrupt_load;
        VARYING_LOAD* varying_load;
     public:
        MYSQL_FUNC(std::string iP, std::string name, std::string passwd, std::string database);
        void get_experimental_parameters();
        void get_global_plan_flag();
        void get_local_plan_flag();
        void get_load_model();//LD for loadmodel
        void get_price();//PRICE for .......PRICE
        void get_interrupt_data();
        void get_uninterrupt_data();
        void get_varying_data();
        ~MYSQL_FUNC();
     private:
     MYSQL_LIB* mysql_lib;
};
                

#endif