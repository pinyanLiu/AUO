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
            std::cout << mysql_row[i]<<" "<<i;
        }
        std::cout << std::endl;
    }
}
void MYSQL_LIB::free_result(void)   
{
	mysql_free_result(mysql_res);
}

//-----------class import in mysql----------------
MYSQL_FUNC::MYSQL_FUNC(std::string iP, std::string name, std::string passwd, std::string database)
{
	mysql_lib = new MYSQL_LIB(iP,name,passwd,database);

}

MYSQL_FUNC::EXPERIMENTAL_PARAMETERS MYSQL_FUNC::get_experimental_parameters()
{
	EXPERIMENTAL_PARAMETERS EP;
	mysql_lib->operate("SELECT value FROM AUO_BaseParameter WHERE parameter_name = 'time_block'");
	mysql_lib->mysql_row = mysql_lib->getRow();
	EP.time_block = atoi(mysql_row[0]);
	mysql_lib->free_result();

	mysql_lib->operate("SELECT value FROM AUO_BaseParameter WHERE parameter_name = 'Vess*Cess'");
	mysql_lib->mysql_row = mysql_lib->getRow();
	EP.vess_cess = atoi(mysql_lib->mysql_row[0]);
	mysql_lib->free_result();

	operate("SELECT value FROM AUO_BaseParameter WHERE parameter_name = 'SOCmin'");
	mysql_row = getRow();
	EP.SOC_min = atof(mysql_row[0]);
	free_result();

	operate("SELECT value FROM AUO_BaseParameter WHERE parameter_name = 'SOCmax'");
	mysql_row = getRow();
	EP.SOC_max = atof(mysql_row[0]);
	free_result();

	operate("SELECT value FROM AUO_BaseParameter WHERE parameter_name = 'SOCthres'");
	mysql_row = getRow();
	EP.SOC_threshold = atof(mysql_row[0]);
	free_result();

	operate("SELECT value FROM AUO_BaseParameter WHERE parameter_name = 'ini_SOC'");
	mysql_row = getRow();
	EP.ini_SOC = atof(mysql_row[0]);
	free_result();

	operate("SELECT value FROM AUO_BaseParameter WHERE parameter_name = 'now_SOC'");
	mysql_row = getRow();
	EP.now_SOC = atof(mysql_row[0]);
	free_result();

	operate("SELECT value FROM AUO_BaseParameter WHERE parameter_name = 'Pbatmin'");
	mysql_row = getRow();
	EP.P_bat_min = atoi(mysql_row[0]);
	free_result();

	operate("SELECT value FROM AUO_BaseParameter WHERE parameter_name = 'Pbatmax'");
	mysql_row = getRow();
	EP.P_bat_max = atoi(mysql_row[0]);
	free_result();

	operate("SELECT value FROM AUO_BaseParameter WHERE parameter_name = 'Pgridmax'");
	mysql_row = getRow();
	EP.P_grid_max = atoi(mysql_row[0]);
	free_result();

	operate("SELECT value FROM AUO_BaseParameter WHERE parameter_name = 'real_time'");
	mysql_row = getRow();
	EP.realtime = atoi(mysql_row[0]);
	free_result();

	operate("SELECT value FROM AUO_BaseParameter WHERE parameter_name = 'Global_next_simulate_timeblock'");
	mysql_row = getRow();
	EP.Global_next_simulate_timeblock = atoi(mysql_row[0]);
	free_result();


	operate("SELECT value FROM AUO_BaseParameter WHERE parameter_name = 'simulate_price'");
	mysql_row = getRow();
	EP.simulate_price = mysql_row[0];
	free_result();
	return EP;
}

MYSQL_FUNC::PLAN_FLAG MYSQL_FUNC::get_plan_flag()
{
	PLAN_FLAG PF;
	operate("SELECT flag FROM AUO_flag WHERE variable_name = 'Pgrid'");
	mysql_row = getRow();
	PF.Pgrid = bool(mysql_row[0]);
	free_result();

	operate("SELECT flag FROM AUO_flag WHERE variable_name = 'mu_grid'");
	mysql_row = getRow();
	PF.mu_grid = mysql_row[0];

	free_result();

	operate("SELECT flag FROM AUO_flag WHERE variable_name = 'Psell'");
	mysql_row = getRow();
	PF.Psell = (mysql_row[0]);
	free_result();

	operate("SELECT flag FROM AUO_flag WHERE variable_name = 'Pess'");
	mysql_row = getRow();
	PF.Pess = (mysql_row[0]);
	free_result();

	operate("SELECT flag FROM AUO_flag WHERE variable_name = 'SOC'");
	mysql_row = getRow();
	PF.SOC = (mysql_row[0]);
	free_result();

	operate("SELECT flag FROM AUO_flag WHERE variable_name = 'SOC_change'");
	mysql_row = getRow();
	PF.SOC_change = (mysql_row[0]);
	free_result();


	operate("SELECT flag FROM AUO_flag WHERE variable_name = 'Pfc'");
	mysql_row = getRow();
	PF.Pfc = (mysql_row[0]);
	free_result();


	operate("SELECT flag FROM AUO_flag WHERE variable_name = 'muFC'");
	mysql_row = getRow();
	PF.mu_fc = (mysql_row[0]);

	free_result();


	return PF;
}
/*
void MYSQL_FUNC::get_load_model()
{

}
void MYSQL_FUNC::get_price()
{

}
*/
