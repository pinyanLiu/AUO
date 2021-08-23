#ifndef __OPTIMIZATION_HPP__
#define __OPTIMIZATION_HPP__
#include "glpk.h"
#include "mysql_func.hpp"
#include "new_2d.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

class OPTIMIZE
{
private:
    glp_prob* mip;
    int num_of_variable;
    float **coefficient;
    int Total_Row;
    int Total_Col;
    int coef_row_num = 0, bnd_row_num = 1;


public:
    std::vector<std::string> variable_name;
    OPTIMIZE(char* prob_name,char* extremum,MYSQL_FUNC::EXPERIMENTAL_PARAMETERS);
    ~OPTIMIZE();
    void set_variable_name(MYSQL_FUNC::PLAN_FLAG pf);//push variable under each situation
    void cal_var_num();
    void set_obj(MYSQL_FUNC::PLAN_FLAG pf);//set objective function
    void set_constraint_matrix(MYSQL_FUNC::PLAN_FLAG pf);//set whole row
    void set_col();//set whole variable constraint
    void set_cal_parm();//set the calculation method and parameter
};



#endif