#include "optimization.hpp"

OPTIMIZE::OPTIMIZE(/* args */)
{
}

OPTIMIZE::~OPTIMIZE()
{
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