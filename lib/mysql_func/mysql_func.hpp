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
            std::string simulate_price;
            
        }EXPERIMENTAL_PARAMETERS;
        
        typedef struct 
        {
            bool Pgrid;
            bool PV;
            bool FC;
            bool Pess;
            bool Sell;
            bool DR;//for limiting battery has to dischargeat   east 80% per day.
            bool Comfort;
            bool SOC_change;
        }PLAN_FLAG;
        std::vector<int> load_model;
        std::vector<float> price;
        EXPERIMENTAL_PARAMETERS ep;//ep for EXPERIMENTAL_PARAMETERS
        PLAN_FLAG pf;//pf for PLAN_FLAG
     public:
        MYSQL_FUNC(std::string iP, std::string name, std::string passwd, std::string database);
       // EXPERIMENTAL_PARAMETERS get_experimental_parameters();
        void get_experimental_parameters();
        void get_plan_flag();
        void get_load_model();//LD for loadmodel
        void get_price();//PRICE for .......PRICE
        //對資料庫進行sql操作
        
        ~MYSQL_FUNC();
     private:
     MYSQL_LIB* mysql_lib;
};
                

#endif