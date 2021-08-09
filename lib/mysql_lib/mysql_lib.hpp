//--------------------------------------
// This lib is the base command of mysql
//--------------------------------------
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
        void  print_result(void);  //列印查詢結果
        int   getRow_and_atoi(void);
        std::vector<int> getArray_and_atoi(void);
        float getRow_and_atof(void);
        std::vector<float> getArray_and_atof(void);
        std::string getRow_string(void); //without turning type
        std::vector<std::string> getArray_string(void); //without turning type
        bool setBuffer_and_operate(char* command);
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

};

#endif