#include <iostream>
#include "source/BLproblem.h"
#include <cstdlib>

int main() {


    double sw0 = 80;
    double so0 =20;

    BLproblem A(100, sw0, so0);
//    double time = 1;
//    std::pair<std::vector<double>, std::vector<double>> solution;
//    solution = A.solver(0, time, 100 * time, sw0, so0);
//    std::string filename = "../results/SW_t_" + std::to_string(time) + "_sw0_" + std::to_string(int(sw0)) + "_so0_" + std::to_string(int(so0));
//    A.save_SW(solution, filename);
//    auto command = "python3 ../plot_sw.py "+filename;
//    system(command.c_str());
    std::vector<double> times = {1,10,20,30};
    A.solver_dynamic(0,times,100,sw0,so0);

    return 0;
}
