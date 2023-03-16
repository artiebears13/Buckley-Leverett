#include <iostream>
#include "source/BLproblem.h"
#include <cstdlib>

int main() {


    double sw0 =80;
    double so0 =20;

    BLproblem A(100, sw0, so0);
    std::vector<double> times = {1,10,20,50,100,200};
    A.solver_dynamic(0,times,100,sw0,so0);

//    auto sw = A.solver(0,10,100,sw0,so0);
//    auto ofp = A.get_OFP(sw.second);
//    A.save_OFP(sw.second,ofp,"../ofp/ofp1");


    return 0;
}
