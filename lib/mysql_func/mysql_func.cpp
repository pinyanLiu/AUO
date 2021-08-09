#include "mysql_func.hpp"

MYSQL_FUNC::MYSQL_FUNC(std::string iP, std::string name, std::string passwd, std::string database)
{
	mysql_lib = new MYSQL_LIB(iP,name,passwd,database);
}

//MYSQL_FUNC::EXPERIMENTAL_PARAMETERS MYSQL_FUNC::get_experimental_parameters()
void MYSQL_FUNC::get_experimental_parameters()
{
	mysql_lib->operate("SELECT value FROM AUO_BaseParameter WHERE parameter_name = 'time_block'");
	this->ep.time_block = mysql_lib->getRow_and_atoi();

	mysql_lib->operate("SELECT value FROM AUO_BaseParameter WHERE parameter_name = 'Vess*Cess'");
	this->ep.vess_cess = mysql_lib->getRow_and_atoi();
	
	mysql_lib->operate("SELECT value FROM AUO_BaseParameter WHERE parameter_name = 'SOCmin'");
	this->ep.SOC_min = mysql_lib->getRow_and_atof();

	mysql_lib->operate("SELECT value FROM AUO_BaseParameter WHERE parameter_name = 'SOCmax'");
	this->ep.SOC_max = mysql_lib->getRow_and_atof();

	mysql_lib->operate("SELECT value FROM AUO_BaseParameter WHERE parameter_name = 'SOCthres'");
	this->ep.SOC_threshold = mysql_lib->getRow_and_atof();

	mysql_lib->operate("SELECT value FROM AUO_BaseParameter WHERE parameter_name = 'ini_SOC'");
	this->ep.ini_SOC = mysql_lib->getRow_and_atof();

	mysql_lib->operate("SELECT value FROM AUO_BaseParameter WHERE parameter_name = 'now_SOC'");
	this->ep.now_SOC = mysql_lib->getRow_and_atof();

	mysql_lib->operate("SELECT value FROM AUO_BaseParameter WHERE parameter_name = 'Pbatmin'");
	this->ep.P_bat_min = mysql_lib->getRow_and_atoi();

	mysql_lib->operate("SELECT value FROM AUO_BaseParameter WHERE parameter_name = 'Pbatmax'");
	this->ep.P_bat_max = mysql_lib->getRow_and_atoi();

	mysql_lib->operate("SELECT value FROM AUO_BaseParameter WHERE parameter_name = 'Pgridmax'");
	this->ep.P_grid_max =  mysql_lib->getRow_and_atoi();

	mysql_lib->operate("SELECT value FROM AUO_BaseParameter WHERE parameter_name = 'real_time'");
	this->ep.real_time = mysql_lib->getRow_and_atoi();

	mysql_lib->operate("SELECT value FROM AUO_BaseParameter WHERE parameter_name = 'Global_next_simulate_timeblock'");
	this->ep.Global_next_simulate_timeblock = mysql_lib->getRow_and_atoi();
	
	mysql_lib->operate("SELECT value FROM AUO_BaseParameter WHERE parameter_name = 'simulate_price'");this->ep.simulate_price = mysql_lib->getRow_string();
}

void MYSQL_FUNC::get_plan_flag()
{
	mysql_lib->operate("SELECT flag FROM AUO_flag WHERE variable_name = 'Pgrid'");
	this->pf.Pgrid = mysql_lib->getRow_and_atoi();

	mysql_lib->operate("SELECT flag FROM AUO_flag WHERE variable_name = 'mu_grid'");
	this->pf.mu_grid = mysql_lib->getRow_and_atoi();

	mysql_lib->operate("SELECT flag FROM AUO_flag WHERE variable_name = 'Psell'");
	this->pf.Psell = mysql_lib->getRow_and_atoi();

	mysql_lib->operate("SELECT flag FROM AUO_flag WHERE variable_name = 'Pess'");
	this->pf.Pess = mysql_lib->getRow_and_atoi();

	mysql_lib->operate("SELECT flag FROM AUO_flag WHERE variable_name = 'SOC'");
	this->pf.SOC = mysql_lib->getRow_and_atoi();

	mysql_lib->operate("SELECT flag FROM AUO_flag WHERE variable_name = 'SOC_change'");
	this->pf.SOC_change = mysql_lib->getRow_and_atoi();

	mysql_lib->operate("SELECT flag FROM AUO_flag WHERE variable_name = 'Pfc'");
	this->pf.Pfc = mysql_lib->getRow_and_atoi();

	mysql_lib->operate("SELECT flag FROM AUO_flag WHERE variable_name = 'muFC'");
	this->pf.mu_fc = mysql_lib->getRow_and_atoi();
}

void MYSQL_FUNC::get_load_model()
{
	mysql_lib->operate("SELECT powerConsumption FROM AUO_history_energyConsumption WHERE id <= '96'");
	this->load_model = mysql_lib->getArray_and_atoi();
}

void MYSQL_FUNC::get_price()
{
	mysql_lib->operate( "SELECT "+this->ep.simulate_price+" FROM price WHERE price_period <= "+std::to_string(this->ep.time_block));
	this->price = mysql_lib->getArray_and_atof();
}


MYSQL_FUNC::~MYSQL_FUNC()
{
	delete mysql_lib;
}
