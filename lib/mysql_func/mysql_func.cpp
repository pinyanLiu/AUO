#include "mysql_func.hpp"

MYSQL_FUNC::MYSQL_FUNC(std::string iP, std::string name, std::string passwd, std::string database)
{
	mysql_lib = new MYSQL_LIB(iP,name,passwd,database);

}

MYSQL_FUNC::EXPERIMENTAL_PARAMETERS MYSQL_FUNC::get_experimental_parameters()
{
	EXPERIMENTAL_PARAMETERS EP;
	mysql_lib->operate("SELECT value FROM AUO_BaseParameter WHERE parameter_name = 'time_block'");
	EP.time_block = mysql_lib->getRow_and_atoi();

	mysql_lib->operate("SELECT value FROM AUO_BaseParameter WHERE parameter_name = 'Vess*Cess'");
	EP.vess_cess = mysql_lib->getRow_and_atoi();
	
	mysql_lib->operate("SELECT value FROM AUO_BaseParameter WHERE parameter_name = 'SOCmin'");
	EP.SOC_min = mysql_lib->getRow_and_atof();

	mysql_lib->operate("SELECT value FROM AUO_BaseParameter WHERE parameter_name = 'SOCmax'");
	EP.SOC_max = mysql_lib->getRow_and_atof();

	mysql_lib->operate("SELECT value FROM AUO_BaseParameter WHERE parameter_name = 'SOCthres'");
	EP.SOC_threshold = mysql_lib->getRow_and_atof();

	mysql_lib->operate("SELECT value FROM AUO_BaseParameter WHERE parameter_name = 'ini_SOC'");
	EP.ini_SOC = mysql_lib->getRow_and_atof();

	mysql_lib->operate("SELECT value FROM AUO_BaseParameter WHERE parameter_name = 'now_SOC'");
	EP.now_SOC = mysql_lib->getRow_and_atof();

	mysql_lib->operate("SELECT value FROM AUO_BaseParameter WHERE parameter_name = 'Pbatmin'");
	EP.P_bat_min = mysql_lib->getRow_and_atoi();

	mysql_lib->operate("SELECT value FROM AUO_BaseParameter WHERE parameter_name = 'Pbatmax'");
	EP.P_bat_max = mysql_lib->getRow_and_atoi();

	mysql_lib->operate("SELECT value FROM AUO_BaseParameter WHERE parameter_name = 'Pgridmax'");
	EP.P_grid_max =  mysql_lib->getRow_and_atoi();

	mysql_lib->operate("SELECT value FROM AUO_BaseParameter WHERE parameter_name = 'real_time'");
	EP.real_time = mysql_lib->getRow_and_atoi();

	mysql_lib->operate("SELECT value FROM AUO_BaseParameter WHERE parameter_name = 'Global_next_simulate_timeblock'");
	EP.Global_next_simulate_timeblock = mysql_lib->getRow_and_atoi();

	mysql_lib->operate("SELECT value FROM AUO_BaseParameter WHERE parameter_name = 'simulate_price'");
	EP.simulate_price = mysql_lib->getRow_string();

	return EP;
}

MYSQL_FUNC::PLAN_FLAG MYSQL_FUNC::get_plan_flag()
{
	PLAN_FLAG PF;
	mysql_lib->operate("SELECT flag FROM AUO_flag WHERE variable_name = 'Pgrid'");
	PF.Pgrid = mysql_lib->getRow_and_atoi();

	mysql_lib->operate("SELECT flag FROM AUO_flag WHERE variable_name = 'mu_grid'");
	PF.mu_grid = mysql_lib->getRow_and_atoi();

	mysql_lib->operate("SELECT flag FROM AUO_flag WHERE variable_name = 'Psell'");
	PF.Psell = mysql_lib->getRow_and_atoi();

	mysql_lib->operate("SELECT flag FROM AUO_flag WHERE variable_name = 'Pess'");
	PF.Pess = mysql_lib->getRow_and_atoi();

	mysql_lib->operate("SELECT flag FROM AUO_flag WHERE variable_name = 'SOC'");
	PF.SOC = mysql_lib->getRow_and_atoi();

	mysql_lib->operate("SELECT flag FROM AUO_flag WHERE variable_name = 'SOC_change'");
	PF.SOC_change = mysql_lib->getRow_and_atoi();


	mysql_lib->operate("SELECT flag FROM AUO_flag WHERE variable_name = 'Pfc'");
	PF.Pfc = mysql_lib->getRow_and_atoi();

	mysql_lib->operate("SELECT flag FROM AUO_flag WHERE variable_name = 'muFC'");
	PF.mu_fc = mysql_lib->getRow_and_atoi();


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

MYSQL_FUNC::~MYSQL_FUNC()
{
mysql_lib -> ~MYSQL_LIB();
}
