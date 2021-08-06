#ifndef __MYSQL_LIB_HPP__
#define __MYSQL_LIB_HPP__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <mysql.h>


class MYSQL_LIB
{

    public:
        MYSQL_LIB(const std::string iP,const std::string name,const std::string passwd,const std::string database);
        //對資料庫進行sql操作
        bool operate(const std::string &operation);
        //獲取行
        MYSQL_ROW getRow(void);
        int   getColNum(void);   //得到每行的列數.
        void  freeResult(void);  //
        void  print_result(void);  //列印查詢結果
        void  free_result(void);    //清除result暫存空間
        ~MYSQL_LIB();
    private:
        std::string server_ip;    //資料庫地址
        std::string user_name;    //使用者名稱
        std::string password;     //使用者密碼
        std::string database_name; //資料庫名  
        char        mysql_buffer[2000] = {'\0'};
        MYSQL       *mysql_conn;
        MYSQL_RES   *mysql_res;
        MYSQL_ROW   mysql_row;

    public:
        class IMPORT
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
                    int realtime;
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
                EXPERIMENTAL_PARAMETERS ep;//ep for EXPERIMENTAL_PARAMETERS
                PLAN_FLAG pf;//pf for PLAN_FLAG
                
            public:
                IMPORT();
                EXPERIMENTAL_PARAMETERS get_experimental_parameters();
                PLAN_FLAG get_plan_flag();
                void get_load_model(std::vector<float> LD);//LD for load model
                void get_price(std::vector<float> PRICE);//PRICE for ....... PRICE
                //對資料庫進行sql操作
                
                ~IMPORT();

            
            private:
        };
                

};

#endif