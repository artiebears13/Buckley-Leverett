#include <iostream>
#include "source/BLproblem.h"
#include <cstdlib>

int main() {


    double sw0 = 0.80;
    double so0 = 0.20;
    double nw = 2.5638;
    double no = 3;
    int time_steps = 10000; //number of time steps for 1s (total steps = time_steps*T)
    std::string filename = "../ofp/ofp";

    BLproblem BL(100, sw0, so0, nw, no);
    std::vector<double> times = {0.001,0.002,0.004,0.006,0.008,0.01};
    BL.solver_dynamic(0, times, time_steps, sw0, so0);  //dynamic solution - generates files in folder 'results'
//
//    auto sw = BL.solver(0, 0.01, 100, sw0, so0); // returns pair (x, sw(x))
//    auto ofp = BL.get_OFP(sw.second);
//    BL.save_OFP(sw.second, ofp, filename);  //save to file, columns: sw, k_ro, k_rw
//    std::cout<<"===================================================="<<std::endl;
//    std::cout << "results saved to file: " + filename << std::endl<< std::endl<<"To plot solution use command:" << std::endl
//              << "    python3 ../ofp/plot_ofp.py " + filename << std::endl;
//
//    std::cout<<"===================================================="<<std::endl;


    return 0;
}
