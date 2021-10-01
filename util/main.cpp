#include "mysql_func.hpp"
#include "optimization.hpp"
#include "glpk.h"
using namespace std;
int main()
{
    MYSQL_FUNC mysql_func("140.124.42.65", "root", "fuzzy314", "Hems_lib");
    OPTIMIZE opt(mysql_func.ep, mysql_func.gpf, mysql_func.lpf);
    opt.get_IT(mysql_func.interrupt_load);

    while (opt.ep.Global_next_simulate_timeblock != 96)
    {
        cout << "sample time " << opt.ep.Global_next_simulate_timeblock << endl;
        opt.set_col(mysql_func.gpf, mysql_func.lpf);
        opt.set_obj(mysql_func.gpf, mysql_func.price);
        //cout << "where are you bitch" << endl;
        opt.set_situation(mysql_func.gpf, mysql_func.lpf);
        opt.load_matrix();
        opt.set_opt_parm();
        opt.update_final_result(mysql_func.gpf, mysql_func.lpf);
        opt.outport_file();
        for (int i = 0; i < 11; i++)
        {
            for (int j = 0; j < 95; j++)
            {
                cout << opt.final_result->IT_switch[i][j] << " ";
            }
            cout << endl;
        }

        if (opt.err != 0)
        {
            return 0;
        }
        opt.delete_prob();
        if (opt.ep.Global_next_simulate_timeblock == 95)
            break;
        opt.reset_coeff_matrix();
        opt.update_experiment_parameter();
        opt.reset_problem();
        cout << "---------------------" << endl;
    }

    return 0;
}