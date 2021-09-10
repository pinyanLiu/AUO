#include "optimization.hpp"
//
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
	cal_remain_timeblock();

//construct glpk matrix

	set_variable_name(PF);
	cal_var_num();
	cal_Total_Col();
	cal_Total_Row();
	coefficient = NEW2D(Total_Row, Total_Col, double);
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
	
	if(PF.PV){
		variable_name.push_back("Ppv");
	}
	
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
	this->Total_Row = (this->ep.time_block - this->ep.Global_next_simulate_timeblock) * 200 + 1;//should have a better way to calculate this but maybe i'll do it later.
	#ifdef DEBUG
		std::cout<<"Total_Row:"<<Total_Row<<std::endl;
	#endif
}
void OPTIMIZE::cal_Total_Col()
{
	this->Total_Col = num_of_variable * (this->ep.time_block - this->ep.Global_next_simulate_timeblock);
	#ifdef DEBUG
		std::cout<<"Total_Col:"<<Total_Col<<std::endl;
	#endif
}

void OPTIMIZE::cal_remain_timeblock()
{
	this->remain_timeblock = (this->ep.time_block - this->ep.Global_next_simulate_timeblock);
	#ifdef DEBUG
		std::cout<<"remain_timeblock:"<<remain_timeblock<<std::endl;
	#endif
}

void OPTIMIZE::set_obj(MYSQL_FUNC::PLAN_FLAG PF,std::vector<float> price)
{
	
	for (int j = 0; j < remain_timeblock; j++)
	{
		if (PF.Pgrid)
			glp_set_obj_coef(mip, (find_variableName_position("Pgrid") + 1 + j * num_of_variable), price[j + ep.Global_next_simulate_timeblock] * delta_T);
	/*	
		if (PF.Sell)
			glp_set_obj_coef(mip, (find_variableName_position("Psell") + 1 + j * num_of_variable), price[j + sample_time] * delta_T * (-1));
	
		if (PF.FC)
			glp_set_obj_coef(mip, (find_variableName_position("Pfct") + 1 + j * num_of_variable), Hydro_Price / Hydro_Cons * delta_T); //FC cost
	*/
	}
	
}

void OPTIMIZE::set_col(MYSQL_FUNC::PLAN_FLAG PF)
{
	for(int i = 0; i<remain_timeblock;i++)
	{
		if(PF.Pgrid)
		{
			set_Pgrid_col();
		}

		if(PF.PV)
		{
			set_Ppv_col();
		}
/*
		else if (PF.FC)
		{

		}
*/
		if (PF.Pess)
		{
			set_Pess_col();
				if (PF.SOC_change)
				{
					set_Pess_change_col();
				}
		}
/*
		else if (PF.Sell)
		{
			set_Psell_col();
		}

		else if (PF.DR)
		{

		}
		else if (PF.Comfort)
		{

		}
*/

	}
}


void OPTIMIZE::get_EP(MYSQL_FUNC::EXPERIMENTAL_PARAMETERS EP)
{
	this->ep.time_block = EP.time_block;
	this->ep.vess_cess  = EP.vess_cess;
	this->ep.SOC_min    = EP.SOC_min;
	this->ep.SOC_max    = EP.SOC_max;
	this->ep.SOC_threshold = EP.SOC_threshold;
	this->ep.ini_SOC 	 = EP.ini_SOC;
	this->ep.now_SOC 	 = EP.now_SOC;
	this->ep.P_bat_max  = EP.P_bat_max;
	this->ep.P_bat_min  = EP.P_bat_min;
	this->ep.P_grid_max = EP.P_grid_max;
	this->ep.real_time  = EP.real_time;
	this->ep.Global_next_simulate_timeblock = EP.Global_next_simulate_timeblock;
	this->ep.simulate_price = EP.simulate_price;
	this->ep.num_of_it_load = EP.num_of_it_load;
	this->ep.num_of_un_load = EP.num_of_un_load;
	this->ep.num_of_vr_load = EP.num_of_vr_load;

}

void OPTIMIZE::get_IT(MYSQL_FUNC::INTERRUPT_LOAD IT)
{
	this->interrupt_load.id = IT.id;
	this->interrupt_load.group_id  = IT.group_id;
	this->interrupt_load.SOC_min    = IT.SOC_min;
	this->interrupt_load.SOC_max    = IT.SOC_max;
	this->interrupt_load.SOC_threshold = IT.SOC_threshold;
	this->interrupt_load.ini_SOC 	 = IT.ini_SOC;
	this->interrupt_load.now_SOC 	 = IT.now_SOC;
	this->interrupt_load.P_bat_max  = IT.P_bat_max;
	this->interrupt_load.P_bat_min  = IT.P_bat_min;
	this->interrupt_load.P_grid_max = IT.P_grid_max;
	this->interrupt_load.real_time  = IT.real_time;
	this->interrupt_load.Global_next_simulate_timeblock = IT.Global_next_simulate_timeblock;
	this->interrupt_load.simulate_price = IT.simulate_price;
}
void OPTIMIZE::get_UT(MYSQL_FUNC::UNINTERRUPT_LOAD UT)
{
	this->ep.time_block = EP.time_block;
	this->ep.vess_cess  = EP.vess_cess;
	this->ep.SOC_min    = EP.SOC_min;
	this->ep.SOC_max    = EP.SOC_max;
	this->ep.SOC_threshold = EP.SOC_threshold;
	this->ep.ini_SOC 	 = EP.ini_SOC;
	this->ep.now_SOC 	 = EP.now_SOC;
	this->ep.P_bat_max  = EP.P_bat_max;
	this->ep.P_bat_min  = EP.P_bat_min;
	this->ep.P_grid_max = EP.P_grid_max;
	this->ep.real_time  = EP.real_time;
	this->ep.Global_next_simulate_timeblock = EP.Global_next_simulate_timeblock;
	this->ep.simulate_price = EP.simulate_price;
}
void OPTIMIZE::get_VR(MYSQL_FUNC::VARYING_LOAD VR)
{
	this->ep.time_block = EP.time_block;
	this->ep.vess_cess  = EP.vess_cess;
	this->ep.SOC_min    = EP.SOC_min;
	this->ep.SOC_max    = EP.SOC_max;
	this->ep.SOC_threshold = EP.SOC_threshold;
	this->ep.ini_SOC 	 = EP.ini_SOC;
	this->ep.now_SOC 	 = EP.now_SOC;
	this->ep.P_bat_max  = EP.P_bat_max;
	this->ep.P_bat_min  = EP.P_bat_min;
	this->ep.P_grid_max = EP.P_grid_max;
	this->ep.real_time  = EP.real_time;
	this->ep.Global_next_simulate_timeblock = EP.Global_next_simulate_timeblock;
	this->ep.simulate_price = EP.simulate_price;
}



void OPTIMIZE::set_Pgrid_col()
{
	for(int i = 0;i<remain_timeblock;i++)
	{
		glp_set_col_name(mip, (find_variableName_position("Pgrid") + 1 + i * num_of_variable),("Pgrid_"+std::to_string(ep.Global_next_simulate_timeblock+i)).c_str());
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
	for(int i = 0;i<remain_timeblock;i++)
	{
		glp_set_col_name(mip, (find_variableName_position("Pess") + 1 + i * num_of_variable),("Pess_"+std::to_string(ep.Global_next_simulate_timeblock+i)).c_str());
		glp_set_col_bnds(mip, (find_variableName_position( "Pess") + 1 + i * num_of_variable), GLP_DB, -this->ep.P_bat_min, this->ep.P_bat_max); // Pess
		glp_set_col_kind(mip, (find_variableName_position( "Pess") + 1 + i * num_of_variable), GLP_CV);
//------------------------------------------------------------------------------------------------------
		glp_set_col_name(mip, (find_variableName_position("Pcharge") + 1 + i * num_of_variable),("Pcharge_"+std::to_string(ep.Global_next_simulate_timeblock+i)).c_str());
		glp_set_col_bnds(mip, (find_variableName_position("Pcharge") + 1 + i * num_of_variable), GLP_DB,0.0, this->ep.P_bat_max); // Pess +
		glp_set_col_kind(mip,(find_variableName_position("Pcharge") + 1 + i * num_of_variable),GLP_CV);
//-------------------------------------------------------------------------------------------------------
		glp_set_col_name(mip, (find_variableName_position("Pdischarge") + 1 + i * num_of_variable),("Pdischarge_"+std::to_string(ep.Global_next_simulate_timeblock+i)).c_str());		
		glp_set_col_bnds(mip,(find_variableName_position("Pdischarge") + 1 + i * num_of_variable),GLP_DB,-this->ep.P_bat_min, 0.0); // Pess -
		glp_set_col_kind(mip,(find_variableName_position("Pdischarge") + 1 + i * num_of_variable),GLP_CV);
//------------------------------------------------------------------------------------------------------
		glp_set_col_name(mip, (find_variableName_position("SOC") + 1 + i * num_of_variable),("SOC_"+std::to_string(ep.Global_next_simulate_timeblock+i)).c_str());		
		glp_set_col_bnds(mip,(find_variableName_position("SOC") + 1 + i * num_of_variable), GLP_DB,this->ep.SOC_min,this->ep.SOC_max); //SOC
		glp_set_col_kind(mip,(find_variableName_position("SOC") + 1 + i * num_of_variable), GLP_CV);
//----------------------------------------------------------------------------------------------------
		glp_set_col_name(mip, (find_variableName_position("Z") + 1 + i * num_of_variable),("Z_"+std::to_string(ep.Global_next_simulate_timeblock+i)).c_str());		
		glp_set_col_bnds(mip,(find_variableName_position( "Z")+ 1 + i * num_of_variable), GLP_DB, 0.0, 1.0); //Z
		glp_set_col_kind(mip,(find_variableName_position( "Z")+ 1 + i * num_of_variable), GLP_BV);
		

	}

}

void OPTIMIZE::set_Pess_change_col()
{		
	for(int i = 0;i<remain_timeblock;i++)
	{
		{
			glp_set_col_name(mip, (find_variableName_position("SOC_change") + 1 + i * num_of_variable),("SOC_change_"+std::to_string(ep.Global_next_simulate_timeblock+i)).c_str());
			glp_set_col_bnds(mip,(find_variableName_position("SOC_change") + 1 + i * num_of_variable), GLP_DB,(-this->ep.P_bat_min * delta_T) / (this->ep.vess_cess),(this->ep.P_bat_max * delta_T) / (this->ep.vess_cess));
			glp_set_col_kind(mip,(find_variableName_position("SOC_change") + 1 + i * num_of_variable), GLP_CV);
//----------------------------------------------------------------------------------------------------
			glp_set_col_name(mip, (find_variableName_position("SOC_increase") + 1 + i * num_of_variable),("SOC_increase_"+std::to_string(ep.Global_next_simulate_timeblock+i)).c_str());
			glp_set_col_bnds(mip,(find_variableName_position("SOC_increase") + 1 + i * num_of_variable),GLP_DB, 0.0, (this->ep.P_bat_max * delta_T) /(this->ep.	vess_cess));
			glp_set_col_kind(mip,(find_variableName_position("SOC_increase") + 1 + i * num_of_variable), GLP_CV);
//----------------------------------------------------------------------------------------------------			
			glp_set_col_name(mip, (find_variableName_position("SOC_decrease") + 1 + i * num_of_variable),("SOC_decrease_"+std::to_string(ep.Global_next_simulate_timeblock+i)).c_str());
			glp_set_col_bnds(mip,(find_variableName_position("SOC_decrease") + 1 + i * num_of_variable),GLP_DB, (-this->ep.P_bat_min * delta_T) /(this->ep.	vess_cess), 0.0);
			glp_set_col_kind(mip,(find_variableName_position("SOC_decrease") + 1 + i * num_of_variable), GLP_CV);
//----------------------------------------------------------------------------------------------------
			glp_set_col_name(mip, (find_variableName_position("SOC_Z") + 1 + i * num_of_variable),("SOC_Z_"+std::to_string(ep.Global_next_simulate_timeblock+i)).c_str());
			glp_set_col_bnds(mip,(find_variableName_position("SOC_Z") + 1 + i * num_of_variable),GLP_DB, 0.0,1.0);
			glp_set_col_kind(mip,(find_variableName_position("SOC_Z") + 1 + i * num_of_variable),GLP_BV);
		}

	}
	
}
void OPTIMIZE::set_Ppv_col()
{
	for(int i = 0;i<remain_timeblock;i++)
	{
		glp_set_col_name(mip, (find_variableName_position("Ppv") + 1 + i * num_of_variable),("Ppv_"+std::to_string(ep.Global_next_simulate_timeblock+i)).c_str());
		glp_set_col_bnds(mip, (find_variableName_position("Ppv") + 1 + i * num_of_variable), GLP_FX, 1, 1); 
		glp_set_col_kind(mip, (find_variableName_position("Ppv") + 1 + i * num_of_variable), GLP_CV);
	}
	
}

void OPTIMIZE::set_interrupt_row()
{


}













void OPTIMIZE::outport_file()
{
	glp_write_lp(mip, NULL, "Hems_lib.txt");
}


//---------------------------------------------------------------------------------------

int OPTIMIZE::find_variableName_position(std::string target)
{
	auto it = std::find(variable_name.begin(),variable_name.end(),target);
	// If element was found
	if (it != variable_name.end())
		return (it - variable_name.begin());
	else
	{
		std::cout<<"can't find "<<target<<"in variable name"<<std::endl;
		return -1;
	}

}