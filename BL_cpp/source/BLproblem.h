#pragma once

#include "params.h"
#include "cmath"
#include <iostream>
#include <vector>
#include "params.h"
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>


class BLproblem {
private:
    int N;                              //number of steps
    double h;                           //step
    double phi = PHI;                   //porosity
    double kw0 = KW_0;                   //water relative permeability
    double ko0 = KO_0;                   //oil relative permeability
    double mu_w = MU_W;               //wetting phase viscosity
    double mu_o = MU_O;               //non wetting phase viscosity
    double u = U;                     //inject rate
    std::vector<double> nw = {};          //water exponent for modified Brooks-Corey functions
    std::vector<double> no = {};          //oil exponent for modified Brooks-Corey functions
    std::vector<double> m = {};           //
    std::vector<double> sw = {};          //water saturation
    std::vector<double> sw_prev = {};     //water saturation previous layer


public:
    BLproblem(int Nx, double sw0, double so0, double start_nw, double start_no) {
        N = Nx;
        for (int i = 0; i < N; ++i) {
            double n = 1. + i / 100.;
            sw_prev.push_back(so0);
            sw.push_back(0.0);
            nw.push_back(start_nw);
            no.push_back(start_no);
//            nw.push_back(2.0+1.0*i/100);
//            no.push_back(2.0+1.0*i/100);
            m.push_back(2.0);
//            std::cout << nw[i] << " " << no[i] << std::endl;
        }
        sw_prev[0] = sw0;
        h = 1. / (Nx - 1);
    }

/*  return (sw^nw)*kw0*/
    double k_rw(double sw_cur);

//    return (1-sw_cur)^no * ko0
    double k_ro(double sw_cur);

//    return sw_cur^m
    double pc(double &sw_cur);

//    return sw_cur*(porosity)
    double sigma(double &sw_cur);

//
    double f(double &sw_cur);

    double g(double &sw_cur);

/// solver of Buckley-Lewerett equation
/// \param t0: start time
/// \param tn: stop time
/// \param time_steps: number steps by time
/// \param sw0: initial water saturation
/// \param swn: initial oil saturation
/// \return: pair (x, sw(x))
    std::pair<std::vector<double>, std::vector<double>>
    solver(double t0,
           double tn,
           double time_steps,
           double sw0,
           double swn);

/// solves Buckley-Lewerett equation, write solution to files
/// \param t0 start time
/// \param tn vector of times where you want to get solution
/// \param time_steps step by time
/// \param sw0 initial water saturation
/// \param swn initial oil saturation
    void solver_dynamic(double t0, std::vector<double> tn, double time_steps, double sw0, double swn, std::string filename);

///
/// \param sw - vector of water saturation
/// \return pair(ko(sw), kw(sw))
    std::pair<std::vector<double>, std::vector<double>>
    get_OFP(std::vector<double> sw);

    std::vector<double>
    get_pc(std::vector<double> sw);

    ///
    /// \param sw  vector - water saturation
    /// \param perms pair(ko(sw), kw(sw))
    /// \param filename
    void save_OFP(std::vector<double> sw,
                  std::pair<std::vector<double>, std::vector<double>> perms,
                  std::string filename);

//    save "sw pc"
    void save_PC(std::vector<double> sw,
                 std::vector<double> pc,
                 std::string filename);

//    save "first second"
    void save_SW(std::pair<std::vector<double>,
            std::vector<double>> data,
                 std::string filename);

    void save_SO(std::pair<std::vector<double>,
            std::vector<double>> data,
                 std::string filename);


};


