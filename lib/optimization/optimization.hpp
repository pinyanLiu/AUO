#ifndef __OPTIMIZATION_HPP__
#define __OPTIMIZATION_HPP__
#include "glpk.h"
#include "mysql_func.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

class OPTIMIZE
{
private:
    glp_prob* mip;
    int num_of_variable;
public:
    OPTIMIZE(/* args */);
    ~OPTIMIZE();
        std::vector<std::string> variable_name;
    void set_variable_name(MYSQL_FUNC::PLAN_FLAG pf);
    int  cal_var_num();
    void set_obj(MYSQL_FUNC::PLAN_FLAG pf);//set objective function
    void set_constraint_matrix(MYSQL_FUNC::PLAN_FLAG pf);//set whole row
    void set_col();//set whole variable constraint
    void set_cal_parm();//set the calculation method and parameter
};



#endif