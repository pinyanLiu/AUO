#include "mysql_func.hpp"
#define num_it  12 //amount of interrupted load
#define num_ut  2  //amount of uninterrupted load
#define num_vr  1  //amount of varying load

MYSQL_FUNC::MYSQL_FUNC(std::string iP, std::string name, std::string passwd, std::string database)
{
	mysql_lib = new MYSQL_LIB(iP,name,passwd,database);
	interrupt_load = new INTERRUPT_LOAD[num_it];
	uninterrupt_load =  new UNINTERRUPT_LOAD[num_ut];
	varying_load = new VARYING_LOAD[num_vr];
//	get_experimental_parameters();
//	get_plan_flag();
//	get_load_model();
//	get_price();
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
	
	mysql_lib->operate("SELECT value FROM AUO_BaseParameter WHERE parameter_name = 'simulate_price'");
	this->ep.simulate_price = mysql_lib->getRow_string();

	this->ep.num_of_it_load = num_it;
	this->ep.num_of_un_load = num_ut;
	this->ep.num_of_vr_load = num_vr;
}

void MYSQL_FUNC::get_plan_flag()
{
	mysql_lib->operate("SELECT flag FROM GHEMS_flag WHERE variable_name = 'Pgrid'");
	this->pf.Pgrid = mysql_lib->getRow_and_atoi();

	mysql_lib->operate("SELECT flag FROM GHEMS_flag WHERE variable_name = 'PV'");
	this->pf.PV = mysql_lib->getRow_and_atoi();

	mysql_lib->operate("SELECT flag FROM GHEMS_flag WHERE variable_name = 'FC'");
	this->pf.FC = mysql_lib->getRow_and_atoi();

	mysql_lib->operate("SELECT flag FROM GHEMS_flag WHERE variable_name = 'Pess'");
	this->pf.Pess = mysql_lib->getRow_and_atoi();

	mysql_lib->operate("SELECT flag FROM GHEMS_flag WHERE variable_name = 'Sell'");
	this->pf.Sell = mysql_lib->getRow_and_atoi();

	mysql_lib->operate("SELECT flag FROM GHEMS_flag WHERE variable_name = 'SOC_change'");
	this->pf.SOC_change = mysql_lib->getRow_and_atoi();

	mysql_lib->operate("SELECT flag FROM GHEMS_flag WHERE variable_name = 'DR'");
	this->pf.DR = mysql_lib->getRow_and_atoi();

	mysql_lib->operate("SELECT flag FROM GHEMS_flag WHERE variable_name = 'Comfort'");
	this->pf.Comfort = mysql_lib->getRow_and_atoi();
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

void MYSQL_FUNC::get_interrupt_data()
{
		std::vector<int> int_result;
		std::vector<float> float_result;
		std::vector<std::string> string_result;
		mysql_lib->operate("SELECT start_time FROM load_list  WHERE group_id = '1' ");
		int_result = mysql_lib->getArray_and_atoi();
		for (size_t i = 0; i < num_it; i++)
		{
			interrupt_load[i].start_time = int_result[i];	
		}
		mysql_lib->operate("SELECT end_time FROM load_list  WHERE group_id = '1' ");
		int_result = mysql_lib->getArray_and_atoi();
		for (size_t i = 0; i < num_it; i++)
		{
			interrupt_load[i].end_time = int_result[i];	
		}

		mysql_lib->operate("SELECT operation_time FROM load_list  WHERE group_id = '1' ");
		int_result = mysql_lib->getArray_and_atoi();
		for (size_t i = 0; i < num_it; i++)
		{
			interrupt_load[i].operation_time = int_result[i];	
		}

		mysql_lib->operate("SELECT power1 FROM load_list  WHERE group_id = '1' ");
		float_result = mysql_lib->getArray_and_atof();
		for (size_t i = 0; i < num_it; i++)
		{
			interrupt_load[i].max_power = float_result[i];	
		}

		mysql_lib->operate("SELECT equip_name FROM load_list  WHERE group_id = '1' ");
		string_result = mysql_lib->getArray_string();
		for (size_t i = 0; i < num_it; i++)
		{
			interrupt_load[i].equip_name = string_result[i];	
		}

		for (size_t i = 0; i < num_it; i++)
		{
			interrupt_load[i].id = i+1;
			interrupt_load[i].group_id = 1;
		}
		
		
		
		
}

void MYSQL_FUNC::get_uninterrupt_data()
{
		std::vector<int> int_result;
		std::vector<float> float_result;
		std::vector<std::string> string_result;
		
		mysql_lib->operate("SELECT start_time FROM load_list  WHERE group_id = '2' ");
		int_result = mysql_lib->getArray_and_atoi();
		for (size_t i = 0; i < num_ut; i++)
		{
			uninterrupt_load[i].start_time = int_result[i];	
		}
		mysql_lib->operate("SELECT end_time FROM load_list  WHERE group_id = '2' ");
		int_result = mysql_lib->getArray_and_atoi();
		for (size_t i = 0; i < num_ut; i++)
		{
			uninterrupt_load[i].end_time = int_result[i];	
		}

		mysql_lib->operate("SELECT operation_time FROM load_list  WHERE group_id = '2' ");
		int_result = mysql_lib->getArray_and_atoi();
		for (size_t i = 0; i < num_ut; i++)
		{
			uninterrupt_load[i].operation_time = int_result[i];	
		}

		mysql_lib->operate("SELECT power1 FROM load_list  WHERE group_id = '2' ");
		float_result = mysql_lib->getArray_and_atof();
		for (size_t i = 0; i < num_ut; i++)
		{
			uninterrupt_load[i].max_power = float_result[i];	
		}


		mysql_lib->operate("SELECT equip_name FROM load_list  WHERE group_id = '2' ");
		string_result = mysql_lib->getArray_string();
		for (size_t i = 0; i < num_ut; i++)
		{
			uninterrupt_load[i].equip_name = string_result[i];	
		}

		for (size_t i = 0; i < num_ut; i++)
		{
			uninterrupt_load[i].id = i+1+num_it;
			uninterrupt_load[i].group_id = 2;
		}
		
}

void MYSQL_FUNC::get_varying_data()
{
		std::vector<int> int_result;
		std::vector<float> float_result;
		std::vector<std::string> string_result;
		
		mysql_lib->operate("SELECT start_time FROM load_list  WHERE group_id = '3' ");
		int_result = mysql_lib->getArray_and_atoi();
		for (size_t i = 0; i < num_vr; i++)
		{
			varying_load[i].start_time = int_result[i];	
		}
		mysql_lib->operate("SELECT end_time FROM load_list  WHERE group_id = '3' ");
		int_result = mysql_lib->getArray_and_atoi();
		for (size_t i = 0; i < num_vr; i++)
		{
			varying_load[i].end_time = int_result[i];	
		}

		mysql_lib->operate("SELECT operation_time FROM load_list  WHERE group_id = '3' ");
		int_result = mysql_lib->getArray_and_atoi();
		for (size_t i = 0; i < num_vr; i++)
		{
			varying_load[i].operation_time = int_result[i];	
		}

		mysql_lib->operate("SELECT power1 FROM load_list  WHERE group_id = '3' ");
		float_result = mysql_lib->getArray_and_atof();
		for (size_t i = 0; i < num_vr; i++)
		{
			varying_load[i].power_block[0] = float_result[i];	
		}

		mysql_lib->operate("SELECT power2 FROM load_list  WHERE group_id = '3' ");
		float_result = mysql_lib->getArray_and_atof();
		for (size_t i = 0; i < num_vr; i++)
		{
			varying_load[i].power_block[1] = float_result[i];	
		}
		
		mysql_lib->operate("SELECT power3 FROM load_list  WHERE group_id = '3' ");
		float_result = mysql_lib->getArray_and_atof();
		for (size_t i = 0; i < num_vr; i++)
		{
			varying_load[i].power_block[2] = float_result[i];	
		}

		mysql_lib->operate("SELECT block1 FROM load_list  WHERE group_id = '3' ");
		int_result = mysql_lib->getArray_and_atoi();
		for (size_t i = 0; i < num_vr; i++)
		{
			varying_load[i].op_time_block[0] = int_result[i];	
		}
	
		mysql_lib->operate("SELECT block2 FROM load_list  WHERE group_id = '3' ");
		int_result = mysql_lib->getArray_and_atoi();
		for (size_t i = 0; i < num_vr; i++)
		{
			varying_load[i].op_time_block[1] = int_result[i];	
		}

		mysql_lib->operate("SELECT block3 FROM load_list  WHERE group_id = '3' ");
		int_result = mysql_lib->getArray_and_atoi();
		for (size_t i = 0; i < num_vr; i++)
		{
			varying_load[i].op_time_block[2] = int_result[i];	
		}

		mysql_lib->operate("SELECT equip_name FROM load_list  WHERE group_id = '3' ");
		string_result = mysql_lib->getArray_string();
		for (size_t i = 0; i < num_vr; i++)
		{
			varying_load[i].equip_name = string_result[i];	
		}

		for (size_t i = 0; i < num_vr; i++)
		{
			varying_load[i].id = i+1+num_it+num_ut;
			varying_load[i].group_id = 3;
		}

}


MYSQL_FUNC::~MYSQL_FUNC()
{

	delete []interrupt_load;
	delete []uninterrupt_load;
	delete []varying_load;
	delete mysql_lib;
}
