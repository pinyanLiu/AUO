#include "optimization.hpp"
//
#define DEBUG
#define delta_T 0.25

OPTIMIZE::OPTIMIZE(MYSQL_FUNC::EXPERIMENTAL_PARAMETERS EP, MYSQL_FUNC::GLOBAL_PLAN_FLAG GPF, MYSQL_FUNC::LOCAL_PLAN_FLAG LPF)
{
	get_EP(EP);
	cal_remain_timeblock(); //get remain time block
	set_variable_name(GPF, LPF);
	cal_var_num();
	cal_Total_Col();
	cal_Total_Row();
	interrupt_load = new INTERRUPT_LOAD[ep.num_of_it_load];
	uninterrupt_load = new UNINTERRUPT_LOAD[ep.num_of_ut_load];
	varying_load = new VARYING_LOAD[ep.num_of_vr_load];
	final_result = new Final_result;
	coefficient = NEW2D(Total_Row, Total_Col, double);
	bnd_row_num = 1;
	//glpk part
	set_problem("GHEMS", "min");
	glp_add_rows(mip, Total_Row);
	glp_add_cols(mip, Total_Col);
	glp_create_index(mip);
	//
	final_result->IT_switch.resize(ep.num_of_it_load, std::vector<bool>(96));
	final_result->UT_switch.resize(ep.num_of_ut_load, std::vector<bool>(96));
	final_result->VR_switch.resize(ep.num_of_vr_load, std::vector<bool>(96));

	//construct glpk matrix
}

OPTIMIZE::~OPTIMIZE()
{
	delete[] interrupt_load;
	delete[] uninterrupt_load;
	delete[] varying_load;
	delete[] coefficient;
	delete final_result;
	glp_delete_prob(mip);
}

void OPTIMIZE::set_problem(char *prob_name, char *extremum)
{
	mip = glp_create_prob();
	glp_set_prob_name(mip, prob_name);
	if (extremum == "max" || extremum == "MAX")
		glp_set_obj_dir(mip, GLP_MAX);
	else if (extremum == "min" || extremum == "MIN")
		glp_set_obj_dir(mip, GLP_MIN);
}

void OPTIMIZE::set_variable_name(MYSQL_FUNC::GLOBAL_PLAN_FLAG GPF, MYSQL_FUNC::LOCAL_PLAN_FLAG LPF)
{

	if (LPF.interrupt)
	{
		for (int i = 0; i < ep.num_of_it_load; i++)
		{
			variable_name.push_back("interrupt" + std::to_string(i + 1));
		}
	}
	if (LPF.uninterrupt)
	{
		for (int i = 0; i < ep.num_of_ut_load; i++)
		{
			variable_name.push_back("uninterrupt" + std::to_string(i + 1));
			variable_name.push_back("uninterrupt_Delta" + std::to_string(i + 1));
		}
	}
	if (LPF.varying)
	{
		for (int i = 0; i < ep.num_of_vr_load; i++)
		{
			variable_name.push_back("varying" + std::to_string(i + 1));
			variable_name.push_back("varying_Delta" + std::to_string(i + 1));
			variable_name.push_back("varying_Psi" + std::to_string(i + 1));
		}
	}
	if (GPF.Pgrid == 1)
		variable_name.push_back("Pgrid");
	if (GPF.DR == 1)
		variable_name.push_back("mu_grid");
	if (GPF.Sell == 1)
		variable_name.push_back("Psell");
	if (GPF.Pess == 1)
	{
		variable_name.push_back("Pess");
		variable_name.push_back("Pcharge");
		variable_name.push_back("Pdischarge");
		variable_name.push_back("SOC");
		variable_name.push_back("Z");
		if (GPF.SOC_change)
		{
			variable_name.push_back("SOC_change");
			variable_name.push_back("SOC_increase");
			variable_name.push_back("SOC_decrease");
			variable_name.push_back("SOC_Z");
		}
	}

	/*if (GPF.FC == 1)
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
	if(GPF.Comfort){

	}
	*/

	if (GPF.PV)
	{
		variable_name.push_back("Ppv");
	}
}

void OPTIMIZE::cal_var_num()
{
	this->num_of_variable = variable_name.size();
#ifdef DEBUG
	std::cout << "num_of_variable:" << num_of_variable << std::endl;
#endif
}

void OPTIMIZE::cal_Total_Row()
{
	this->Total_Row = (this->ep.time_block - this->ep.Global_next_simulate_timeblock) * 200 + 1; //should have a better way to calculate this but maybe i'll do it later.
#ifdef DEBUG
	std::cout << "Total_Row:" << Total_Row << std::endl;
#endif
}
void OPTIMIZE::cal_Total_Col()
{
	this->Total_Col = num_of_variable * (this->ep.time_block - this->ep.Global_next_simulate_timeblock);
#ifdef DEBUG
	std::cout << "Total_Col:" << Total_Col << std::endl;
#endif
}

void OPTIMIZE::cal_remain_timeblock()
{
	this->remain_timeblock = (this->ep.time_block - this->ep.Global_next_simulate_timeblock);
#ifdef DEBUG
	std::cout << "remain_timeblock:" << remain_timeblock << std::endl;
#endif
}

void OPTIMIZE::load_matrix()
{
	int *ia = new int[Total_Row * Total_Col + 1];
	int *ja = new int[Total_Row * Total_Col + 1];
	double *ar = new double[Total_Row * Total_Col + 1];
	for (int i = 0; i < Total_Row; i++)
	{
		for (int j = 0; j < Total_Col; j++)
		{
			ia[i * ((remain_timeblock)*num_of_variable) + j + 1] = i + 1;
			ja[i * ((remain_timeblock)*num_of_variable) + j + 1] = j + 1;
			ar[i * ((remain_timeblock)*num_of_variable) + j + 1] = coefficient[i][j];
		}
	}
	glp_load_matrix(mip, Total_Row * Total_Col, ia, ja, ar);
	delete[] ia;
	delete[] ja;
	delete[] ar;
}

void OPTIMIZE::set_situation(MYSQL_FUNC::GLOBAL_PLAN_FLAG GPF, MYSQL_FUNC::LOCAL_PLAN_FLAG LPF)
{
	balance_func(LPF);
	IT_constrain();
}

void OPTIMIZE::set_obj(MYSQL_FUNC::GLOBAL_PLAN_FLAG GPF, std::vector<float> price)
{

	for (int j = 0; j < remain_timeblock; j++)
	{
		if (GPF.Pgrid)
		{
			glp_set_obj_coef(mip, (find_variableName_position("Pgrid") + 1 + j * num_of_variable), price[j + ep.Global_next_simulate_timeblock] * delta_T);
		}
		/*	
		if (GPF.Sell)
			glp_set_obj_coef(mip, (find_variableName_position("Psell") + 1 + j * num_of_variable), price[j + sample_time] * delta_T * (-1));
	
		if (GPF.FC)

			glp_set_obj_coef(mip, (find_variableName_position("Pfct") + 1 + j * num_of_variable), Hydro_Price / Hydro_Cons * delta_T); //FC cost
	*/
	}
}
void OPTIMIZE::set_opt_parm()
{
	glp_iocp parm;
	glp_init_iocp(&parm);
	parm.tm_lim = 100000;
	parm.presolve = GLP_ON;
	parm.gmi_cuts = GLP_ON;
	parm.fp_heur = GLP_ON;
	parm.bt_tech = GLP_BT_BFS;
	parm.br_tech = GLP_BR_PCH;
	int err = glp_intopt(mip, &parm);
}

void OPTIMIZE::update_final_result()
{
}

void OPTIMIZE::set_col(MYSQL_FUNC::GLOBAL_PLAN_FLAG GPF, MYSQL_FUNC::LOCAL_PLAN_FLAG LPF)
{
	if (LPF.interrupt)
	{
		set_interrupt_col();
	}

	if (LPF.uninterrupt)
	{
		set_uninterrupt_col();
	}

	if (LPF.varying)
	{
		set_varying_col();
	}

	if (GPF.Pgrid)
	{
		set_Pgrid_col();
	}

	if (GPF.PV)
	{
		set_Ppv_col();
	}
	/*
		else if (GPF.FC)
		{

		}
*/
	if (GPF.Pess)
	{
		set_Pess_col();
		if (GPF.SOC_change)
		{
			set_Pess_change_col();
		}
	}
	/*
		else if (GPF.Sell)
		{
			set_Psell_col();
		}

		else if (GPF.DR)
		{

		}
		else if (GPF.Comfort)
		{

		}
*/
}

void OPTIMIZE::set_interrupt_col()
{
	for (int i = 0; i < remain_timeblock; i++)
	{
		for (int j = 0; j < ep.num_of_it_load; j++)
		{
			glp_set_col_name(mip, (find_variableName_position("interrupt" + std::to_string(j + 1)) + 1 + i * num_of_variable), ("interrupt" + std::to_string(ep.Global_next_simulate_timeblock + i) + "_" + std::to_string(j + 1)).c_str());

			glp_set_col_bnds(mip, (find_variableName_position("interrupt" + std::to_string(j + 1)) + 1 + i * num_of_variable), GLP_DB, 0.0, 1.0);

			glp_set_col_kind(mip, (find_variableName_position("interrupt" + std::to_string(j + 1)) + 1 + i * num_of_variable), GLP_BV);
		}
	}
}
void OPTIMIZE::set_uninterrupt_col()
{
	for (int i = 0; i < remain_timeblock; i++)
	{
		for (int j = 0; j < ep.num_of_ut_load; j++)
		{
			glp_set_col_name(mip, (find_variableName_position("uninterrupt" + std::to_string(j + 1)) + 1 + i * num_of_variable), ("uninterrupt" + std::to_string(ep.Global_next_simulate_timeblock + i) + "_" + std::to_string(j + 1)).c_str());

			glp_set_col_bnds(mip, (find_variableName_position("uninterrupt" + std::to_string(j + 1)) + 1 + i * num_of_variable), GLP_DB, 0.0, 1.0);

			glp_set_col_kind(mip, (find_variableName_position("uninterrupt" + std::to_string(j + 1)) + 1 + i * num_of_variable), GLP_BV);
			//uninterrupted delta
			glp_set_col_name(mip, (find_variableName_position("uninterrupt_Delta" + std::to_string(j + 1)) + 1 + i * num_of_variable), ("uninterrupt_Delta" + std::to_string(ep.Global_next_simulate_timeblock + i) + "_" + std::to_string(j + 1)).c_str());

			glp_set_col_bnds(mip, (find_variableName_position("uninterrupt_Delta" + std::to_string(j + 1)) + 1 + i * num_of_variable), GLP_DB, 0.0, 1.0);

			glp_set_col_kind(mip, (find_variableName_position("uninterrupt_Delta" + std::to_string(j + 1)) + 1 + i * num_of_variable), GLP_BV);
		}
	}
}
void OPTIMIZE::set_varying_col()
{
	for (int i = 0; i < remain_timeblock; i++)
	{
		for (int j = 0; j < ep.num_of_vr_load; j++)
		{
			glp_set_col_name(mip, (find_variableName_position("varying" + std::to_string(j + 1)) + 1 + i * num_of_variable), ("varying" + std::to_string(ep.Global_next_simulate_timeblock + i) + "_" + std::to_string(j + 1)).c_str());

			glp_set_col_bnds(mip, (find_variableName_position("varying" + std::to_string(j + 1)) + 1 + i * num_of_variable), GLP_DB, 0.0, 1.0);

			glp_set_col_kind(mip, (find_variableName_position("varying" + std::to_string(j + 1)) + 1 + i * num_of_variable), GLP_BV);

			glp_set_col_name(mip, (find_variableName_position("varying_Delta" + std::to_string(j + 1)) + 1 + i * num_of_variable), ("varying_Delta" + std::to_string(ep.Global_next_simulate_timeblock + i) + "_" + std::to_string(j + 1)).c_str());

			glp_set_col_bnds(mip, (find_variableName_position("varying_Delta" + std::to_string(j + 1)) + 1 + i * num_of_variable), GLP_DB, 0.0, 1.0);

			glp_set_col_kind(mip, (find_variableName_position("varying_Delta" + std::to_string(j + 1)) + 1 + i * num_of_variable), GLP_BV);

			glp_set_col_name(mip, (find_variableName_position("varying_Psi" + std::to_string(j + 1)) + 1 + i * num_of_variable), ("varying_Psi" + std::to_string(ep.Global_next_simulate_timeblock + i) + "_" + std::to_string(j + 1)).c_str());

			glp_set_col_bnds(mip, (find_variableName_position("varying_Psi" + std::to_string(j + 1)) + 1 + i * num_of_variable), GLP_DB, 0.0, 0.6); //0.6 is max of varying load power ,i'll rewrite later

			glp_set_col_kind(mip, (find_variableName_position("varying_Psi" + std::to_string(j + 1)) + 1 + i * num_of_variable), GLP_CV);
		}
	}
}

void OPTIMIZE::set_Pgrid_col()
{
	for (int i = 0; i < remain_timeblock; i++)
	{
		glp_set_col_name(mip, (find_variableName_position("Pgrid") + 1 + i * num_of_variable), ("Pgrid_" + std::to_string(ep.Global_next_simulate_timeblock + i)).c_str());
		glp_set_col_bnds(mip, (find_variableName_position("Pgrid") + 1 + i * num_of_variable), GLP_DB, 0.0, this->ep.P_grid_max);
		glp_set_col_kind(mip, (find_variableName_position("Pgrid") + 1 + i * num_of_variable), GLP_CV);
	}
}
/*
void OPTIMIZE::set_Psell_col()
{
	glp_set_col_bnds(mip, (find_variableName_position(  "Psell") + 1 + i * num_of_variable), GLP_DB, -0.00001, this->ep.Psell_max);
	glp_set_col_kind(mip, (find_variableName_position( "Psell") + 1 + i * num_of_variable), GLP_CV);
}
*/
void OPTIMIZE::set_Pess_col()
{
	for (int i = 0; i < remain_timeblock; i++)
	{
		glp_set_col_name(mip, (find_variableName_position("Pess") + 1 + i * num_of_variable), ("Pess_" + std::to_string(ep.Global_next_simulate_timeblock + i)).c_str());
		glp_set_col_bnds(mip, (find_variableName_position("Pess") + 1 + i * num_of_variable), GLP_DB, -this->ep.P_bat_min, this->ep.P_bat_max); // Pess
		glp_set_col_kind(mip, (find_variableName_position("Pess") + 1 + i * num_of_variable), GLP_CV);
		//------------------------------------------------------------------------------------------------------
		glp_set_col_name(mip, (find_variableName_position("Pcharge") + 1 + i * num_of_variable), ("Pcharge_" + std::to_string(ep.Global_next_simulate_timeblock + i)).c_str());
		glp_set_col_bnds(mip, (find_variableName_position("Pcharge") + 1 + i * num_of_variable), GLP_DB, 0.0, this->ep.P_bat_max); // Pess +
		glp_set_col_kind(mip, (find_variableName_position("Pcharge") + 1 + i * num_of_variable), GLP_CV);
		//-------------------------------------------------------------------------------------------------------
		glp_set_col_name(mip, (find_variableName_position("Pdischarge") + 1 + i * num_of_variable), ("Pdischarge_" + std::to_string(ep.Global_next_simulate_timeblock + i)).c_str());
		glp_set_col_bnds(mip, (find_variableName_position("Pdischarge") + 1 + i * num_of_variable), GLP_DB, -this->ep.P_bat_min, 0.0); // Pess -
		glp_set_col_kind(mip, (find_variableName_position("Pdischarge") + 1 + i * num_of_variable), GLP_CV);
		//------------------------------------------------------------------------------------------------------
		glp_set_col_name(mip, (find_variableName_position("SOC") + 1 + i * num_of_variable), ("SOC_" + std::to_string(ep.Global_next_simulate_timeblock + i)).c_str());
		glp_set_col_bnds(mip, (find_variableName_position("SOC") + 1 + i * num_of_variable), GLP_DB, this->ep.SOC_min, this->ep.SOC_max); //SOC
		glp_set_col_kind(mip, (find_variableName_position("SOC") + 1 + i * num_of_variable), GLP_CV);
		//----------------------------------------------------------------------------------------------------
		glp_set_col_name(mip, (find_variableName_position("Z") + 1 + i * num_of_variable), ("Z_" + std::to_string(ep.Global_next_simulate_timeblock + i)).c_str());
		glp_set_col_bnds(mip, (find_variableName_position("Z") + 1 + i * num_of_variable), GLP_DB, 0.0, 1.0); //Z
		glp_set_col_kind(mip, (find_variableName_position("Z") + 1 + i * num_of_variable), GLP_BV);
	}
}

void OPTIMIZE::set_Pess_change_col()
{
	for (int i = 0; i < remain_timeblock; i++)
	{
		{
			glp_set_col_name(mip, (find_variableName_position("SOC_change") + 1 + i * num_of_variable), ("SOC_change_" + std::to_string(ep.Global_next_simulate_timeblock + i)).c_str());
			glp_set_col_bnds(mip, (find_variableName_position("SOC_change") + 1 + i * num_of_variable), GLP_DB, (-this->ep.P_bat_min * delta_T) / (this->ep.vess_cess), (this->ep.P_bat_max * delta_T) / (this->ep.vess_cess));
			glp_set_col_kind(mip, (find_variableName_position("SOC_change") + 1 + i * num_of_variable), GLP_CV);
			//----------------------------------------------------------------------------------------------------
			glp_set_col_name(mip, (find_variableName_position("SOC_increase") + 1 + i * num_of_variable), ("SOC_increase_" + std::to_string(ep.Global_next_simulate_timeblock + i)).c_str());
			glp_set_col_bnds(mip, (find_variableName_position("SOC_increase") + 1 + i * num_of_variable), GLP_DB, 0.0, (this->ep.P_bat_max * delta_T) / (this->ep.vess_cess));
			glp_set_col_kind(mip, (find_variableName_position("SOC_increase") + 1 + i * num_of_variable), GLP_CV);
			//----------------------------------------------------------------------------------------------------
			glp_set_col_name(mip, (find_variableName_position("SOC_decrease") + 1 + i * num_of_variable), ("SOC_decrease_" + std::to_string(ep.Global_next_simulate_timeblock + i)).c_str());
			glp_set_col_bnds(mip, (find_variableName_position("SOC_decrease") + 1 + i * num_of_variable), GLP_DB, (-this->ep.P_bat_min * delta_T) / (this->ep.vess_cess), 0.0);
			glp_set_col_kind(mip, (find_variableName_position("SOC_decrease") + 1 + i * num_of_variable), GLP_CV);
			//----------------------------------------------------------------------------------------------------
			glp_set_col_name(mip, (find_variableName_position("SOC_Z") + 1 + i * num_of_variable), ("SOC_Z_" + std::to_string(ep.Global_next_simulate_timeblock + i)).c_str());
			glp_set_col_bnds(mip, (find_variableName_position("SOC_Z") + 1 + i * num_of_variable), GLP_DB, 0.0, 1.0);
			glp_set_col_kind(mip, (find_variableName_position("SOC_Z") + 1 + i * num_of_variable), GLP_BV);
		}
	}
}
void OPTIMIZE::set_Ppv_col()
{
	for (int i = 0; i < remain_timeblock; i++)
	{
		glp_set_col_name(mip, (find_variableName_position("Ppv") + 1 + i * num_of_variable), ("Ppv_" + std::to_string(ep.Global_next_simulate_timeblock + i)).c_str());
		glp_set_col_bnds(mip, (find_variableName_position("Ppv") + 1 + i * num_of_variable), GLP_FX, 1, 1);
		glp_set_col_kind(mip, (find_variableName_position("Ppv") + 1 + i * num_of_variable), GLP_CV);
	}
}

void OPTIMIZE::balance_func(MYSQL_FUNC::LOCAL_PLAN_FLAG LPF)
{
	//row bound
	for (int i = 0; i < (remain_timeblock); i++)
	{
		glp_set_row_name(mip, (bnd_row_num + i), ("balance_func_timeblock_" + std::to_string(i)).c_str());
		glp_set_row_bnds(mip, (bnd_row_num + i), GLP_UP, 0, 0);
	}
	bnd_row_num += remain_timeblock;
	//coeff matrix

	if (LPF.interrupt)
	{
		for (int h = 0; h < ep.num_of_it_load; h++)
		{
			if ((interrupt_load[h].end_time - ep.Global_next_simulate_timeblock) >= 0)
			{
				if ((interrupt_load[h].start_time - ep.Global_next_simulate_timeblock) >= 0)
				{
					for (int i = (interrupt_load[h].start_time - ep.Global_next_simulate_timeblock); i <= (interrupt_load[h].end_time - ep.Global_next_simulate_timeblock); i++)
					{
						coefficient[glp_find_row(mip, ("balance_func_timeblock_" + std::to_string(i)).c_str()) - 1][i * num_of_variable + find_variableName_position("interrupt" + std::to_string(h + 1))] = interrupt_load[h].max_power;
					}
				}
				else if ((interrupt_load[h].start_time - ep.Global_next_simulate_timeblock) < 0)
				{
					for (int i = 0; i <= (interrupt_load[h].end_time - ep.Global_next_simulate_timeblock); i++)
					{
						coefficient[glp_find_row(mip, ("balance_func_timeblock_" + std::to_string(i)).c_str()) - 1][i * num_of_variable + find_variableName_position("interrupt" + std::to_string(h + 1))] = interrupt_load[h].max_power;
					}
				}
			}
		}
	}
	// if (LPF.uninterrupt)
	// {
	// 	for (int h = 0; h < ep.num_of_ut_load; h++)
	// 	{
	// 		if ((uninterrupt_load[h].end_time - ep.Global_next_simulate_timeblock) >= 0)
	// 		{
	// 			if ((uninterrupt_load[h].start_time - ep.Global_next_simulate_timeblock) >= 0)
	// 			{
	// 				for (int i = (uninterrupt_load[h].start_time - ep.Global_next_simulate_timeblock); i <= (uninterrupt_load[h].end_time - ep.Global_next_simulate_timeblock); i++)
	// 				{
	// 					coefficient[glp_find_row(mip, ("balance_func_timeblock_" + std::to_string(i)).c_str())][i * num_of_variable + find_variableName_position("interrupt" + std::to_string(h + 1))] = uninterrupt_load[h].max_power;
	// 				}
	// 			}
	// 			else if ((uninterrupt_load[h].start_time - ep.Global_next_simulate_timeblock) < 0)
	// 			{
	// 				for (int i = 0; i <= (uninterrupt_load[h].end_time - ep.Global_next_simulate_timeblock); i++)
	// 				{
	// 					coefficient[glp_find_row(mip, ("balance_func_timeblock_" + std::to_string(i)).c_str())][i * num_of_variable + find_variableName_position("interrupt" + std::to_string(h + 1))] = uninterrupt_load[h].max_power;
	// 				}
	// 			}
	// 		}
	// 	}
	// }
	// if (LPF.varying)
	// {
	// 	for (int h = 0; h < ep.num_of_it_load; h++)
	// 	{
	// 		if ((varying_load[h].end_time - ep.Global_next_simulate_timeblock) >= 0)
	// 		{
	// 			if ((varying_load[h].start_time - ep.Global_next_simulate_timeblock) >= 0)
	// 			{
	// 				for (int i = (varying_laod[h].start_time - ep.Global_next_simulate_timeblock); i <= (varying_load[h].end_time - ep.Global_next_simulate_timeblock); i++)
	// 				{
	// 					coefficient[glp_find_row(mip, ("balance_func_timeblock_" + std::to_string(i)).c_str())][i * num_of_variable + find_variableName_position("interrupt" + std::to_string(h + 1))] = varying_load[h].max_power;
	// 				}
	// 			}
	// 			else if ((varying_load[h].start_time - ep.Global_next_simulate_timeblock) < 0)
	// 			{
	// 				for (int i = 0; i <= (varying_load[h].end_time - ep.Global_next_simulate_timeblock); i++)
	// 				{
	// 					coefficient[glp_find_row(mip, ("balance_func_timeblock_" + std::to_string(i)).c_str())][i * num_of_variable + find_variableName_position("interrupt" + std::to_string(h + 1))] = varying_load[h].max_power;
	// 				}
	// 			}
	// 		}
	// 	}
	// }
	for (int i = 0; i < (remain_timeblock); i++)
	{
		if (LPF.pgrid)
			coefficient[glp_find_row(mip, ("balance_func_timeblock_" + std::to_string(i)).c_str()) - 1][i * num_of_variable + find_variableName_position("Pgrid")] = -1.0;
		if (LPF.pess)
			coefficient[glp_find_row(mip, ("balance_func_timeblock_" + std::to_string(i)).c_str()) - 1][i * num_of_variable + find_variableName_position("Pess")] = 1.0;
	}
}

void OPTIMIZE::IT_constrain()
{
	//row bound
	for (int i = 0; i < ep.num_of_it_load; i++)
	{
		glp_set_row_name(mip, bnd_row_num + i, ("IT_" + std::to_string(i + 1) + "_OPtime").c_str());
		glp_set_row_bnds(mip, bnd_row_num + i, GLP_LO, interrupt_load[i].remain_op_time, 0.0);
	}
	bnd_row_num += ep.num_of_it_load;
	//coeff matrix
	for (int h = 0; h < ep.num_of_it_load; h++)
	{
		if ((interrupt_load[h].end_time - ep.Global_next_simulate_timeblock) >= 0)
		{
			if ((interrupt_load[h].start_time - ep.Global_next_simulate_timeblock) >= 0)
			{
				for (int i = (interrupt_load[h].start_time - ep.Global_next_simulate_timeblock); i <= (interrupt_load[h].end_time - ep.Global_next_simulate_timeblock); i++)
				{
					coefficient[glp_find_row(mip, ("IT_" + std::to_string(h + 1) + "_OPtime").c_str()) - 1][i * num_of_variable + find_variableName_position("interrupt" + std::to_string(h + 1))] = 1.0;
				}
			}
			else if ((interrupt_load[h].start_time - ep.Global_next_simulate_timeblock) < 0)
			{
				for (int i = 0; i <= (interrupt_load[h].end_time - ep.Global_next_simulate_timeblock); i++)
				{
					coefficient[glp_find_row(mip, ("IT" + std::to_string(h + 1) + "_OPtime").c_str()) - 1][i * num_of_variable + find_variableName_position("interrupt" + std::to_string(h + 1))] = 1.0;
				}
			}
		}
	}
}

void OPTIMIZE::outport_file()
{
	glp_write_lp(mip, NULL, "Hems_lib.txt");
}

//---------------------------------------------------------------------------------------

int OPTIMIZE::find_variableName_position(std::string target)
{
	auto it = std::find(variable_name.begin(), variable_name.end(), target);
	// If element was found
	if (it != variable_name.end())
		return (it - variable_name.begin());
	else
	{
		std::cout << "can't find " << target << "in variable name" << std::endl;
		return -1;
	}
}

void OPTIMIZE::get_EP(MYSQL_FUNC::EXPERIMENTAL_PARAMETERS EP)
{
	this->ep.time_block = EP.time_block;
	this->ep.vess_cess = EP.vess_cess;
	this->ep.SOC_min = EP.SOC_min;
	this->ep.SOC_max = EP.SOC_max;
	this->ep.SOC_threshold = EP.SOC_threshold;
	this->ep.ini_SOC = EP.ini_SOC;
	this->ep.now_SOC = EP.now_SOC;
	this->ep.P_bat_max = EP.P_bat_max;
	this->ep.P_bat_min = EP.P_bat_min;
	this->ep.P_grid_max = EP.P_grid_max;
	this->ep.real_time = EP.real_time;
	this->ep.Global_next_simulate_timeblock = EP.Global_next_simulate_timeblock;
	this->ep.simulate_price = EP.simulate_price;
	this->ep.num_of_it_load = EP.num_of_it_load;
	this->ep.num_of_ut_load = EP.num_of_ut_load;
	this->ep.num_of_vr_load = EP.num_of_vr_load;
}

void OPTIMIZE::get_IT(MYSQL_FUNC::INTERRUPT_LOAD *IT)
{
	for (size_t i = 0; i < ep.num_of_it_load; i++)
	{
		this->interrupt_load[i].id = IT[i].id;
		this->interrupt_load[i].group_id = IT[i].group_id;
		this->interrupt_load[i].start_time = IT[i].start_time;
		this->interrupt_load[i].end_time = IT[i].end_time;
		this->interrupt_load[i].operation_time = IT[i].operation_time;
		this->interrupt_load[i].remain_op_time = IT[i].operation_time;
		this->interrupt_load[i].already_op_time = 0;
		this->interrupt_load[i].max_power = IT[i].max_power;
		this->interrupt_load[i].equip_name = IT[i].equip_name;
	}
}
void OPTIMIZE::get_UT(MYSQL_FUNC::UNINTERRUPT_LOAD *UT)
{
	for (size_t i = 0; i < ep.num_of_ut_load; i++)
	{
		this->uninterrupt_load[i].id = UT[i].id;
		this->uninterrupt_load[i].group_id = UT[i].group_id;
		this->uninterrupt_load[i].start_time = UT[i].start_time;
		this->uninterrupt_load[i].end_time = UT[i].end_time;
		this->uninterrupt_load[i].operation_time = UT[i].operation_time;
		this->uninterrupt_load[i].remain_op_time = UT[i].operation_time;
		this->uninterrupt_load[i].already_op_time = 0;
		this->uninterrupt_load[i].max_power = UT[i].max_power;
		this->uninterrupt_load[i].equip_name = UT[i].equip_name;
	}
}
void OPTIMIZE::get_VR(MYSQL_FUNC::VARYING_LOAD *VR)
{
	for (size_t i = 0; i < ep.num_of_vr_load; i++)
	{
		this->varying_load[i].id = VR[i].id;
		this->varying_load[i].group_id = VR[i].group_id;
		this->varying_load[i].start_time = VR[i].start_time;
		this->varying_load[i].end_time = VR[i].end_time;
		this->varying_load[i].operation_time = VR[i].operation_time;
		this->varying_load[i].remain_op_time = VR[i].operation_time;
		this->varying_load[i].already_op_time = 0;
		for (size_t j = 0; j < 3; j++)
		{
			this->varying_load[i].op_time_block[j] = VR[i].op_time_block[j];
			this->varying_load[i].power_block[j] = VR[i].power_block[j];
		}
		this->varying_load[i].equip_name = VR[i].equip_name;
	}
}