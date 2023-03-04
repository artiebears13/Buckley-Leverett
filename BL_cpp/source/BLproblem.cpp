#include "BLproblem.h"


double BLproblem::k_rw(double &sw){
    return pow(sw, nw[0])*kw0;

};

double BLproblem::k_ro(double &sw){
    return pow(1-sw, no[0])*ko0;
};

double BLproblem::pc(double &sw){
    return pow(sw,m[0]);
};

double BLproblem::sigma(double &sw){
    return sw*phi;
};

double BLproblem::f(double &sw){
    return (k_rw(sw)/mu_w)/(k_rw(sw)/mu_w + k_ro(sw)/mu_o);
};

double BLproblem::g(double &sw);

std::pair<std::vector<double>, std::vector<double>> BLproblem::solver(double t0, double tn, double time_steps);

std::pair<std::vector<double>, std::vector<double>> BLproblem::get_OFP(std::vector<double> sw);

std::vector<double> BLproblem::get_pc(std::vector<double> sw);
