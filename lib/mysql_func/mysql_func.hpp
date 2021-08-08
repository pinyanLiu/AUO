#ifndef __MYSQL_FUNC_HPP__
#define __MYSQL_FUNC_HPP__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
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
            long int P_bat_min;//the minimun power for batterydis   harging
            long int P_bat_max;//the maximun power for batterydis   harging
            double  P_grid_max;
            int real_time;
            int Global_next_simulate_timeblock;
            std::string simulate_price;
            
        }EXPERIMENTAL_PARAMETERS;
        
        typedef struct 
        {
            bool Pgrid;
            bool mu_grid;
            bool Psell;
            bool Pess;
            bool SOC;
            bool SOC_change;//for limiting battery has to dischargeat   east 80% per day.
            bool Pfc;
            bool mu_fc;
        }PLAN_FLAG;
        std::vector<float> load_model;
        std::vector<float> price;
        EXPERIMENTAL_PARAMETERS ep;//ep for EXPERIMENTAL_PARAMETERS
        PLAN_FLAG pf;//pf for PLAN_FLAG
     public:
        MYSQL_FUNC(std::string iP, std::string name, std::string passwd, std::string database);
       // EXPERIMENTAL_PARAMETERS get_experimental_parameters();
        void get_experimental_parameters();
        void get_plan_flag();
        void get_load_model(std::vector<float> LD);//LD for loadmodel
        void get_price(std::vector<float> PRICE);//PRICE for .......PRICE
        //對資料庫進行sql操作
        
        ~MYSQL_FUNC();
     private:
     MYSQL_LIB* mysql_lib;
};
                

#endif