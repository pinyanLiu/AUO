#include "mysql_func.hpp"
#include "optimization.hpp"
//#define DEBUG
using namespace std;
int main(){

MYSQL_FUNC mysql_func("140.124.42.65","root", "fuzzy314", "Hems_lib");
mysql_func.get_experimental_parameters();

#ifdef DEBUG
cout<<"realtime = "<<mysql_func.ep.real_time<<endl;
cout<<"ini_SOC = "<<mysql_func.ep.ini_SOC<<endl;
cout<<"now_SOC = "<<mysql_func.ep.now_SOC<<endl;
cout<<"P_bat_max = "<<mysql_func.ep.P_bat_max<<endl;
cout<<"P_bat_min = "<<mysql_func.ep.P_bat_min<<endl;
cout<<"P_grid_max = "<<mysql_func.ep.P_grid_max<<endl;
cout<<"simulate_price = "<<mysql_func.ep.simulate_price<<endl;
cout<<"SOC_max = "<<mysql_func.ep.SOC_max<<endl;
cout<<"SOC_min = "<<mysql_func.ep.SOC_min<<endl;
cout<<"SOC_threshold = "<<mysql_func.ep.SOC_threshold<<endl;
cout<<"time_block = "<<mysql_func.ep.time_block<<endl;
cout<<"vess_cess = "<<mysql_func.ep.vess_cess<<endl;
#endif
mysql_func.get_plan_flag();

#ifdef DEBUG
cout<<"Pgrid = "<<mysql_func.pf.Pgrid<<endl;
cout<<"Psell = "<<mysql_func.pf.Sell<<endl;
cout<<"Pess = "<<mysql_func.pf.Pess<<endl;
cout<<"SOC change = "<<mysql_func.pf.SOC_change<<endl;
cout<<"FC = "<<mysql_func.pf.FC<<endl;
cout<<"DR = "<<mysql_func.pf.DR<<endl;
cout<<"Comfort = "<<mysql_func.pf.Comfort<<endl;
cout<<"PV = "<<mysql_func.pf.PV<<endl;
#endif

mysql_func.get_load_model();
#ifdef DEBUG
	for (size_t i = 0; i < mysql_func.load_model.size(); i++)
	{
		printf("load_model[%d] = %d\n",i,mysql_func.load_model[i]);
	}
#endif
mysql_func.get_price();
#ifdef DEBUG	
	for (size_t i = 0; i < mysql_func.price.size(); i++)
	{
		printf("price[%d] = %f\n",i,mysql_func.price[i]);
	}
#endif
OPTIMIZE opt("GHEMS","max",mysql_func.ep,mysql_func.pf);

return 0;
}