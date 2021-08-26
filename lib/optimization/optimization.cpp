#include "optimization.hpp"
#define DEBUG
#define delta_T 0.25

OPTIMIZE::OPTIMIZE(char* prob_name,char* extremum,MYSQL_FUNC::EXPERIMENTAL_PARAMETERS EP,MYSQL_FUNC::PLAN_FLAG PF)
{
	get_EP(EP);
	mip = glp_create_prob();
	glp_set_prob_name(mip, prob_name);
	if(extremum == "max" || extremum == "MAX")
		glp_set_obj_dir(mip, GLP_MAX);
	else if (extremum == "min" || extremum == "MIN")
		glp_set_obj_dir(mip, GLP_MIN);
//get remain time block
	remain_timeblock = get_remain_timeblock(EP);
//construct glpk matrix
	set_variable_name(PF);
	cal_var_num();
	cal_Total_Col(EP);
	cal_Total_Row(EP);
	coefficient = NEW2D(Total_Row, Total_Col, float);
	glp_add_rows(mip, Total_Row);
	glp_add_cols(mip, Total_Col);

}

OPTIMIZE::~OPTIMIZE()
{
	delete []coefficient;
	glp_delete_prob(mip);
}

void OPTIMIZE::set_variable_name(MYSQL_FUNC::PLAN_FLAG PF)
{
    if (PF.Pgrid == 1)
		variable_name.push_back("Pgrid");
	if (PF.DR == 1)
		variable_name.push_back("mu_grid");
	if (PF.Sell == 1)
		variable_name.push_back("Psell");
	if (PF.Pess == 1)
	{
		variable_name.push_back("Pess");
		variable_name.push_back("Pcharge");
		variable_name.push_back("Pdischarge");
		variable_name.push_back("SOC");
		variable_name.push_back("Z");
		if (PF.SOC_change)
		{
			variable_name.push_back("SOC_change");
			variable_name.push_back("SOC_increase");
			variable_name.push_back("SOC_decrease");
			variable_name.push_back("SOC_Z");
		}
	}

	/*if (PF.FC == 1)
	{
		variable_name.push_back("Pfc");
		variable_name.push_back("Pfct");
		variable_name.push_back("PfcON");
		variable_name.push_back("PfcOFF");
		variable_name.push_back("muFC");
		for (int i = 0; i < piecewise_num; i++)
			variable_name.push_back("zPfc" + to_string(i + 1));
		for (int i = 0; i < piecewise_num; i++)
			variable_name.push_back("lambda_Pfc" + to_string(i + 1));
	}*/
	/*
	if(PF.Comfort){

	}
	*/
	/*
	if(PF.PV){

	}
	*/
}

void OPTIMIZE::cal_var_num()
{
	this->num_of_variable = variable_name.size();
	#ifdef DEBUG
		std::cout<<"num_of_variable:"<<num_of_variable<<std::endl;
	#endif
}

void OPTIMIZE::cal_Total_Row()
{
	this->Total_Row = (this->time_block - this->Global_next_simulate_timeblock) * 200 + 1;//should have a better way to calculate this but maybe i'll do it later.
	#ifdef DEBUG
		std::cout<<"Total_Row:"<<Total_Row<<std::endl;
	#endif
}
void OPTIMIZE::cal_Total_Col()
{
	this->Total_Col = num_of_variable * (this->time_block - this->Global_next_simulate_timeblock);
	#ifdef DEBUG
		std::cout<<"Total_Col:"<<Total_Col<<std::endl;
	#endif
}
void OPTIMIZE::set_obj(MYSQL_FUNC::PLAN_FLAG PF)
{
	
	
}

void OPTIMIZE::set_col(MYSQL_FUNC::PLAN_FLAG PF)
{
	for(int i = 0; i<remain_timeblock;i++)
	{
		if(PF.Pgrid)
		{

		}
		else if(PF.PV)
		{

		}
		else if (PF.FC)
		{

		}
		else if (PF.Pess)
		{

				if (PF.SOC_change)
				{
				
				}

		}
		else if (PF.Sell)
		{

		}
		else if (PF.DR)
		{

		}
		else if (PF.Comfort)
		{

		}


	}


}

int OPTIMIZE::find_variableName_position(string target)
{
	auto it = find(variable_name.begin(), variable_name.end(), target);

	// If element was found
	if (it != variable_name.end())
		return (it - variable_name.begin());
	else
		std::cout<<"can't find "<<target<<"in variable name"<<std::endl;
		return -1;
}

int OPTIMIZE::get_remain_timeblock()
{
	return (this->time_block - this->Global_next_simulate_timeblock)
}

void get_EP(MYSQL_FUNC::EXPERIMENTAL_PARAMETERS EP)
{
	this->time_block = EP.time_block;
	this->vess_cess  = EP.vess_cess;
	this->SOC_min    = EP.SOC_min;
	this->SOC_MAX    = EP.SOC_MAX;
	this->SOC_threshold = EP.SOC_threshold;
	this->ini_SOC 	 = EP.ini_SOC;
	this->now_SOC 	 = EP.now_SOC;
	this->P_bat_max  = EP.P_bat_max;
	this->P_bat_min  = EP.P_bat_min;
	this->P_grid_max = EP.P_grid_max;
	this->real_time  = EP.real_time;
	this->Global_next_simulate_timeblock = EP.Global_next_simulate_timeblock;
	this->simulate_price = EP.simulate_price;
}

void OPTIMIZE::set_Pgrid_col()
{
	glp_set_col_bnds(mip, (find_variableName_position(variable_name, "Pgrid") + 1 + i * num_of_variable), GLP_DB, 0.0, this->P_grid_max); 
	glp_set_col_kind(mip, (find_variableName_position(variable_name, "Pgrid") + 1 + i * num_of_variable), GLP_CV);
}
/*
void OPTIMIZE::set_Psell_col()
{
	glp_set_col_bnds(mip, (find_variableName_position(variable_name, "Psell") + 1 + i * num_of_variable), GLP_DB, -0.00001, this->Psell_max);
	glp_set_col_kind(mip, (find_variableName_position(variable_name, "Psell") + 1 + i * num_of_variable), GLP_CV);
}
*/
void OPTIMIZE::set_Pess_col()
{
	glp_set_col_bnds(mip, (find_variableName_position(variable_name, "Pess") + 1 + i * num_of_variable), GLP_DB, -this->P_bat_min, this->P_bat_max); // Pess
	glp_set_col_kind(mip, (find_variableName_position(variable_name, "Pess") + 1 + i * num_of_variable), GLP_CV);
	glp_set_col_bnds(mip, (find_variableName_position(variable_name,"Pcharge") + 1 + i * num_of_variable), GLP_UP,0.0, this->P_bat_max); // Pess +
	glp_set_col_kind(mip,(find_variableName_position(variable_name,"Pcharge") + 1 + i * num_of_variable),GLP_CV);
	glp_set_col_bnds(mip,(find_variableName_position(variable_name,"Pdischarge") + 1 + i * num_of_variable),GLP_LO, 0.0,-this->P_bat_min); // Pess -
	glp_set_col_kind(mip,(find_variableName_position(variable_name,"Pdischarge") + 1 + i * num_of_variable),GLP_CV);
	glp_set_col_bnds(mip,(find_variableName_position(variable_name,"SOC") + 1 + i * num_of_variable), GLP_DB,this->SOC_min,this->SOC_max); //SOC
	glp_set_col_kind(mip,(find_variableName_position(variable_name,"SOC") + 1 + i * num_of_variable), GLP_CV);
	glp_set_col_bnds(mip,(find_variableName_position(variable_name, "Z")+ 1 + i * num_of_variable), GLP_DB, 0.0, 1.0); //Z
	glp_set_col_kind(mip,(find_variableName_position(variable_name, "Z")+ 1 + i * num_of_variable), GLP_BV);
	if (SOC_change_flag)
	{
		glp_set_col_bnds(mip,(find_variableName_position(variable_name,"SOC_change") + 1 + i * num_of_variable, GLP_DB,(-this->P_bat_min * delta_T) / (this->vess_cess)(this->P_bat_max * delta_T) / (this->vess_cess));
		glp_set_col_kind(mip,(find_variableName_position(variable_name,"SOC_change") + 1 + i * num_of_variable, GLP_CV);
		glp_set_col_bnds(mip,(find_variableName_position(variable_name,"SOC_increase") + 1 + i *num_of_variable),GLP_UP, 0.0, (this->P_bat_max * delta_T) /(this->vess_cess));
		glp_set_col_kind(mip,(find_variableName_position(variable_name,"SOC_increase") + 1 + i *num_of_variable), GLP_CV;
		glp_set_col_bnds(mip,(find_variableName_position(variable_name,"SOC_decrease") + 1 + i *num_of_variable),GLP_LO, (-this->P_bat_min * delta_T) /(this->vess_cess), 0.0);
		glp_set_col_kind(mip,(find_variableName_position(variable_name,"SOC_decrease") + 1 + i *num_of_variable), GLP_CV;
		glp_set_col_bnds(mip,(find_variableName_position(variable_name,"SOC_Z") + 1 + i * num_of_variable),GLP_DB, 0.0,1.0);
		glp_set_col_kind(mip,(find_variableName_position(variable_name,"SOC_Z") + 1 + i * num_of_variable),GLP_BV);
	}
