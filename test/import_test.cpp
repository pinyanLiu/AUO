#include "import.hpp"
using namespace std;
int main(){

IMPORT import("140.124.42.65","root", "fuzzy314", "chig");
//import.ep = import.get_experimental_parameters();
import.pf = import.get_plan_flag();
cout<<"Pgrid = "<<import.pf.Pgrid<<endl;


return 0;
}