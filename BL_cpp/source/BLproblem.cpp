#include <iomanip>
#include "BLproblem.h"


double BLproblem::k_rw(double &sw) {
    return pow(sw, nw[0]) * kw0;

};

double BLproblem::k_ro(double &sw) {
    return pow(1 - sw, no[0]) * ko0;
};

double BLproblem::pc(double &sw) {
    return pow(sw, m[0]);
};

double BLproblem::sigma(double &sw) {
    return sw * phi;
};

double BLproblem::f(double &sw) {
    return (k_rw(sw) / mu_w) / (k_rw(sw) / mu_w + k_ro(sw) / mu_o);
};

double BLproblem::g(double &sw) {
    return f(sw) * u;
};

std::pair<std::vector<double>, std::vector<double>>
BLproblem::solver(double t0, double tn, double time_steps, double sw0, double swn) {
    double t = t0;
    double tau = (tn - t0) / time_steps;
    std::ofstream file("log");
    file<<"tau:  "<<tau<<std::endl;
    file<<"h:  "<<h<<std::endl;
    file<<"phi:  "<<phi<<std::endl;
    while (t <= tn) {
        file<<"-------------------------"<<std::endl;
        file<<"t:  "<<t<<std::endl;



        sw[0] = sw0;
        sw[sw.size() - 1] = swn;
        for (int i = 1; i < sw.size() - 1; ++i) {
            file<<"sw:  "<<sw[i]<<" "<<sw[i-1]<<std::endl;
            sw[i] = tau / phi * (g(sw_prev[i]) - g(sw_prev[i - 1])) / h + sw_prev[i];
            file<<"i: "<<i<<"///////////////////////////////"<<std::endl;
            file<<"sw:  "<<sw[i]<<" "<<sw[i-1]<<std::endl;
            file<<"g = "<<g(sw_prev[i])<<" - "<<g(sw_prev[i - 1])<<std::endl;
            file<<"g:  "<<(g(sw_prev[i]) - g(sw_prev[i - 1]))<<std::endl;
        }
        t+=tau;
        auto tmp = sw_prev;
        sw_prev = sw;
        sw = tmp;
    }

    std::vector<double>  x(sw.size(),0.0);
    for (int i = 0; i < x.size(); ++i) {
        x[i] = i*h;
        sw[i] = sw[i]/100.;
    }

    std::pair<std::vector<double>, std::vector<double>> res(x,sw);
    return res;
};

void BLproblem::solver_dynamic(double t0, std::vector<double> tn, double time_steps, double sw0, double swn){
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%d-%m-%Y_%H-%M-%S");
    auto filename = oss.str();


    filename = "../results/res_" +  filename; // получаем текущее время

    std::cout<<"writing results to file: "+filename<<std::endl;
    std::cout<<"LINE: x sw ko kw pc "<<std::endl;

    std::ofstream file(filename);
    file<<"file written by Artem Medvedev"<<std::endl;
    file<<"PARAMS:"<<std::endl;
    file<<"t0: "<<t0<<std::endl;
    file<<"sw0: "<<sw0<<std::endl;
    file<<"swn: "<<swn<<std::endl;
    file<<"porosity: "<<phi<<std::endl;
    file<<"mu_w: "<<mu_w<<std::endl;
    file<<"mu_o: "<<mu_o<<std::endl;
    file<<"h: "<<h<<std::endl;
    file<<"N: "<<N<<std::endl;
    file<< "t";
    for (int i = 0; i < tn.size(); ++i) {
        file<<" "<<tn[i];
    }
    file<<std::endl;

    for (int i = 0; i < tn.size(); ++i) {
        std::cout<<"solving for t = "<<tn[i]<<std::endl;

        auto solution = solver(t0,tn[i],time_steps*tn[i],sw0,swn);
        auto ofp = get_OFP(solution.second);
        auto pc = get_pc(solution.second);
        for (int j = 0; j < solution.first.size(); ++j) {
            file<<solution.first[j]<<" "<<solution.second[j]<<" "<<1-solution.second[j]<<" "
            <<ofp.first[j] <<" "<<ofp.second[j]<<" "
            <<pc[j]<<std::endl;
        }
    }
    std::cout<<"writing completed!"<<std::endl;
    auto command = "python3 ../source/visualize.py "+filename;
    std::cout<<"creating graphs:"<<std::endl;
    system(command.c_str());



}

std::pair<std::vector<double>, std::vector<double>>
BLproblem::get_OFP(std::vector<double> sw){
    std::vector<double> ko(sw.size(), 0.0);
    std::vector<double> kw(sw.size(), 0.0);
    for (int i = 0; i < sw.size(); ++i) {
        ko[i] = k_ro(sw[i]);
        kw[i] = k_rw(sw[i]);
    }
    std::pair<std::vector<double>, std::vector<double>> res(ko,kw);
    return res;
};

std::vector<double> BLproblem::get_pc(std::vector<double> sw){
    std::vector<double> p(sw.size(), 0.0);
    for (int i = 0; i < sw.size(); ++i) {
        p[i] = pc(sw[i]);
    }
    return p;
}

void BLproblem::save_OFP(std::vector<double> sw,
                         std::pair<std::vector<double>,
                         std::vector<double>> perms,
                         std::string filename){
    std::ofstream file(filename);
    for (int i = 0; i < perms.first.size(); ++i) {
        file<<sw[i]<<" "<<perms.first[i]<<" "<<perms.second[i]<<std::endl;
    }
    file.close();
}

void BLproblem::save_PC(std::vector<double> sw,std::vector<double> pc, std::string filename){
    std::ofstream file(filename);
    for (int i = 0; i < sw.size(); ++i) {
        file<<sw[i]<<" "<<pc[i]<<std::endl;
    }
    file.close();
}

void BLproblem::save_SW(std::pair<std::vector<double>, std::vector<double>> data, std::string filename){
    std::ofstream file(filename);
    for (int i = 0; i < data.first.size(); ++i) {
        file<<data.first[i]<<" "<<data.second[i]<<std::endl;
    }
    file.close();
}

void BLproblem::save_SO(std::pair<std::vector<double>, std::vector<double>> data, std::string filename) {
    std::ofstream file(filename);
    for (int i = 0; i < data.first.size(); ++i) {
        file << data.first[i] << " " << 1.-data.second[i] << std::endl;
    }
    file.close();
}




