#include "mysql_lib.hpp"

MYSQL_LIB::MYSQL_LIB(const std::string ip,const std::string name,const std::string passwd,const std::string database)
{
	server_ip=ip;
    user_name=name;
    password=passwd;
    database_name=database;

    //初始化mysql   
    mysql_conn=mysql_init(NULL);

   //連線mysql  
    if(!mysql_real_connect(mysql_conn,server_ip.c_str(),user_name.c_str(),password.c_str(),database_name.c_str(),0,NULL,0)) //後面三個引數分別是port,unix_socket,client_flag.
    {
        std::cout<<"fail to connect mysql"<<std::endl;
        exit(1);
    }else{
        std::cout <<"success to connect mysql"<<std::endl;
    }
}


MYSQL_LIB::~MYSQL_LIB()
{
	//釋放儲存結果
    //mysql_free_result(mysql_res);
    //關閉mysql連線
    mysql_close(mysql_conn);
   std::cout << "mysql close"<<std::endl;

}

bool MYSQL_LIB::operate(const std::string &operation)
{
     if(mysql_query(mysql_conn,operation.c_str()))
    {
        std::cout<<"mysql操作失敗"<<std::endl;
        return false;
    }
    //將操作結果儲存在結果集  
    mysql_res=mysql_use_result(mysql_conn);
    return true;
}

MYSQL_ROW  MYSQL_LIB::getRow(void)
{
    return mysql_fetch_row(mysql_res);
}

int MYSQL_LIB::getColNum(void)
{
    return mysql_num_fields(mysql_res);
}
void MYSQL_LIB::print_result(void)
{
    //獲得每行的欄位數
    int num=mysql_num_fields(mysql_res);
    //迴圈讀取所有的行
    while((mysql_row=mysql_fetch_row(mysql_res))!=NULL){
        //將每行欄位都讀出來.
        for(int i=0;i<num;i++){
            std::cout << mysql_row[i]<<" ";
        }
        std::cout << std::endl;
    }
}
void MYSQL_LIB::free_result(void)   
{
	mysql_free_result(mysql_res);
}

float MYSQL_LIB::getRow_and_atof(void)
{
	mysql_row = getRow();
	float result = atof(mysql_row[0]);
	free_result();
	return result;
}

std::vector<float> MYSQL_LIB::getArray_and_atof(void)
{
	//獲得每行的欄位數
    int num=mysql_num_fields(mysql_res);
    std::vector<float> result ;
    //迴圈讀取所有的行
    while((mysql_row=mysql_fetch_row(mysql_res))!=NULL){
        //將每行欄位都寫入result陣列.
        for(int i=0;i<num;i++){
			result.push_back(atof(mysql_row[i]));
        }
    }
	free_result();
	
	return result;
}

int MYSQL_LIB::getRow_and_atoi(void)
{
	mysql_row = getRow();
	int result = atoi(mysql_row[0]);
	free_result();
	return result;
}

std::vector<int> MYSQL_LIB::getArray_and_atoi(void)
{	
 //獲得每行的欄位數
    int num=mysql_num_fields(mysql_res);
    std::vector<int> result ;
    //迴圈讀取所有的行
    while((mysql_row=mysql_fetch_row(mysql_res))!=NULL){
        //將每行欄位都寫入result陣列
        for(int i=0;i<num;i++){
			result.push_back(atoi(mysql_row[i]));
        }
    }
	free_result();
	
	return result;
}

std::string MYSQL_LIB::getRow_string(void) //without turning type
{
	mysql_row = getRow();
	std::string result = mysql_row[0];
	free_result();
	return result;
}

std::vector<std::string> MYSQL_LIB::getArray_string(void) //without turning type
{	
	//獲得每行的欄位數
    int num=mysql_num_fields(mysql_res);
    std::vector<std::string> result ;
    //迴圈讀取所有的行
    while((mysql_row=mysql_fetch_row(mysql_res))!=NULL){
        //將每行欄位都寫入result陣列.
        for(int i=0;i<num;i++){
			result.push_back(mysql_row[i]);
        }
    }
	free_result();
	
	return result;
}

bool MYSQL_LIB::setBuffer_and_operate(char* command)
{
    snprintf(mysql_buffer, sizeof(mysql_buffer), command );
         if(mysql_query(mysql_conn,mysql_buffer))
    {
        std::cout<<"mysql操作失敗"<<std::endl;
        return false;
    }
    //將操作結果儲存在結果集  
    mysql_res=mysql_use_result(mysql_conn);
    return true;
}