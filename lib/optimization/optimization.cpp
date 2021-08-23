#include "optimization.hpp"
#define DEBUG

OPTIMIZE::OPTIMIZE(char* prob_name,char* extremum,MYSQL_FUNC::EXPERIMENTAL_PARAMETERS EP,MYSQL_FUNC::PLAN_FLAG PF)
{
	mip = glp_create_prob();
	glp_set_prob_name(mip, prob_name);
	if(extremum == "max" || extremum == "MAX")
		glp_set_obj_dir(mip, GLP_MAX);
	else if (extremum == "min" || extremum == "MIN")
		glp_set_obj_dir(mip, GLP_MIN);

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
	glp_delete_prob(mip);
}

void OPTIMIZE::set_variable_name(MYSQL_FUNC::PLAN_FLAG pf)
{
    if (pf.Pgrid == 1)
		variable_name.push_back("Pgrid");
	if (pf.DR == 1)
		variable_name.push_back("mu_grid");
	if (pf.Sell == 1)
		variable_name.push_back("Psell");
	if (pf.Pess == 1)
	{
		variable_name.push_back("Pess");
		variable_name.push_back("Pcharge");
		variable_name.push_back("Pdischarge");
		variable_name.push_back("SOC");
		variable_name.push_back("Z");
		if (pf.SOC_change)
		{
			variable_name.push_back("SOC_change");
			variable_name.push_back("SOC_increase");
			variable_name.push_back("SOC_decrease");
			variable_name.push_back("SOC_Z");
		}
	}

	/*if (pf.FC == 1)
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
	if(pf.Comfort){

	}
	*/
	/*
	if(pf.PV){

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

void OPTIMIZE::cal_Total_Row(MYSQL_FUNC::EXPERIMENTAL_PARAMETERS EP)
{
	this->Total_Row = (EP.time_block - EP.Global_next_simulate_timeblock) * 200 + 1;//should have a better way to calculate this but maybe i'll do it later.
	#ifdef DEBUG
		std::cout<<"Total_Row:"<<Total_Row<<std::endl;
	#endif
}
void OPTIMIZE::cal_Total_Col(MYSQL_FUNC::EXPERIMENTAL_PARAMETERS EP)
{
	this->Total_Col = num_of_variable * (EP.time_block - EP.Global_next_simulate_timeblock);
	#ifdef DEBUG
		std::cout<<"Total_Col:"<<Total_Col<<std::endl;
	#endif
}
void OPTIMIZE::set_obj(MYSQL_FUNC::PLAN_FLAG pf)
{


}