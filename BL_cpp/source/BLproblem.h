#pragma once

#include "cmath"
#include <iostream>
#include <vector>
#include "params.h"

class BLproblem {
private:
    int N;                           //number of steps
    double h;                        //step
    double phi = 0.9;                //porosity
    double kw0 = 0.9;                //
    double ko0 = 0.9;                //
    double mu_w = 0.7e-3;            //
    double mu_o = 1.0e-3;            //
    double u = 3.0;                  //inject rate
    std::vector<double> nw;          //
    std::vector<double> no;          //
    std::vector<double> m;           //
    std::vector<double> sw;          //water saturation
    std::vector<double> sw_prev;     //water saturation previous layer


public:
    BLproblem(int Nx, double sw0, double so0) {
        N = Nx;
        for (int i = 0; i < N; ++i) {
            sw_prev.push_back(so0);
            sw.push_back(so0);
            nw.push_back(2.0);
            no.push_back(2.0);
            m.push_back(2.0);
        }
        sw_prev[0] = sw0;
        h = 1 / (Nx - 1);
    }

//  return (sw^nw)*kw0
    double k_rw(double &sw);

//    return (1-sw)^no * ko0
    double k_ro(double &sw);

//    return sw^m
    double pc(double &sw);

//    return sw*(porosity)
    double sigma(double &sw);

//
    double f(double &sw);

    double g(double &sw);

    std::pair<std::vector<double>, std::vector<double>> solver(double t0, double tn, double time_steps);

    std::pair<std::vector<double>, std::vector<double>> get_OFP(std::vector<double> sw);

    std::vector<double> get_pc(std::vector<double> sw);
}


