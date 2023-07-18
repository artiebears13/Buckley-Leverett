#include <iostream>
#include "source/BLproblem.h"
#include <cstdlib>

int main(int argc, char** argv) {

    double nw;
    double no;
    std::cout<<argc<<std::endl;
    for (int i = 0; i < argc; i++){
        std::cout<<"i: "<<argv[i]<<std::endl;
    }
    

    double sw0 = 0.80;
    double so0 = 0.20;
    if (argc==1){
        nw = 2.5638;
        no = 3.5;
    }
    else if (argc==2){
        std::cout << "wrong number of parameters ('./BL_cpp nw no')" << std::endl;
    }
    else{
        nw = std::stod(argv[1]);
        no = std::stod(argv[2]);
    }
    int time_steps = 10000; //number of time steps for 1s (total steps = time_steps*T)
    std::string filename = "../ofp/ofp_new";

    BLproblem BL(100, sw0, so0, nw, no);
//    std::vector<double> times = {0.001,0.002,0.004,0.006,0.008,0.01};
//    BL.solver_dynamic(0, times, time_steps, sw0, so0);  //dynamic solution - generates files in folder 'results'

    auto sw = BL.solver(0, 0.01, 10000, sw0, so0); // returns pair (x, sw(x))
    auto ofp = BL.get_OFP(sw.second);
    BL.save_OFP(sw.second, ofp, filename);  //save to file, columns: sw, k_ro, k_rw
    BL.save_SW(sw, "../ofp/sw_data");
    std::cout<<"===================================================="<<std::endl;
    std::cout << "results saved to file: " + filename << std::endl<< std::endl<<"To plot solution use command:" << std::endl
              << "    python3 ../ofp/plot_ofp.py " + filename << std::endl;

    std::cout<<"===================================================="<<std::endl;


    return 0;
}
