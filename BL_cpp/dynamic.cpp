#include <iostream>
#include "source/BLproblem.h"
#include <cstdlib>

int main(int argc, char *argv[]) {


    double sw0 = 0.80;
    double so0 = 0.20;
    double nw = 2.5638;
    double no = 3;
    int time_steps = 10000; //number of time steps for 1s (total steps = time_steps*T)
    std::string filename = "../ofp/ofp";

    BLproblem BL(100, sw0, so0, nw, no);
    // std::vector<double> times = {0.001, 0.0015, 0.002, 0.0025,0.003, 0.0035,0.004, 0.0045,0.005, 0.0055, 
    //                                  0.006, 0.0065,0.007, 0.0075, 0.008, 0.0085,0.009, 0.0095, 0.01, 0.0105,
    //                                  0.011, 0.0115,0.012};
                
    std::vector<double> times;
    int count = 0;
    double start = 0.0001;
    double end = 0.012;
    double step = (end-start)/300;
    for (double i = start; i<end; i+=step){
        times.push_back(i);
        count++;
    }
    // std::vector<double> times;
    // int N = std::stoi(argv[1]);
    // for (int i = 0; i < N; i++)
    // {
        // times.push_back(times_vec[i]);
    // }
    
    BL.solver_dynamic(0, times, time_steps, sw0, so0, "");  //dynamic solution - generates files in folder 'results'
    std::cout<<">>> NUMBER OF TIME STEPS: "<<count<<std::endl;
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
