#ifndef __IMPORT_HPP__
#define __IMPORT_HPP__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>

class IMPORT
{
private:

    typedef struct 
    {
      int time_block;
      int vess_cess;
      float SOC_max;
      float SOC_min;
      float SOC_threshold;
      float ini_SOC;
      float now_SOC;
      int P_bat_min;//the minimun power for battery discharging
      int P_bat_max;//the maximun power for battery discharging
      float P_grid_max;
      bool realtime;
      int next_simulate_timeblock;
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
    bool SOC_change;//for limiting battery has to discharge at least 80% per day.
    bool Pfc;
    bool mu_fc;
}PLAN_FLAG;

std::vector<float> load_model;
std::vector<float> price;


public:
    IMPORT();
    get_experimental_parameters();
    get_plan_flag();
    get_load_model();
    get_price();

    ~IMPORT();
};

#endif