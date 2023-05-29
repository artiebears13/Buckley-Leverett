#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include "cmath"

std::pair<std::vector<double>,
        std::pair<std::vector<double>, std::vector<double>>>
read_data(std::string filename) {
    std::vector<double> x, y, z;
    double sw_origin, x_val, y_val, z_val;

// открытие файла с координатами
    std::ifstream file(filename);

// проверка, удалось ли открыть файл
    if (!file.is_open()) {
        std::cout << "Ошибка открытия файла" << std::endl;
        exit(-1);
    }


// считывание координат из файла
    while (file >>sw_origin>> x_val >> y_val >> z_val) {
        x.push_back(x_val);
        y.push_back(y_val);
        z.push_back(z_val);
    }
    std::pair<std::vector<double>, std::vector<double>> params(z,y);
    std::pair<std::vector<double>,std::pair<std::vector<double>, std::vector<double>>> k(x, params);
//    std::pair<std::vector<double>,
//            std::pair<std::vector<double>, std::vector<double>>> res(x, k);
// закрытие файла
    file.close();
    return k;
}

double f(double x, double power) {
    double k0 = 0.9;
//    std::cout<<"x: "<<x<<" pow: "<<power<<std::endl;
    return pow(x, power) * k0;
}

double f2(double x, double power) {
    double kw = 1;
//    std::cout<<"x: "<<x<<" pow: "<<power<<std::endl;
    return pow(1-x, power) * kw;
}

double estimate(double sw, double target, double start) {

    double b = 5;
    double a = 0;
    double eps = 0.001;

    double step = (b - a) / 10;
    double power = start;
    int direction = 1;
    while (std::abs(f(sw, power) - target) > eps) {
        if (f(sw, power) > target) {
            if (direction == 1) {
                power += step;
            } else {
                direction = 1;
                step = step / 2;
                power+=step;
            }
        }
        else{
            if (direction==1){
                direction = -1;
                step = step/2;
                power+=step*direction;
            }
            else{
                power+=step*direction;
            }
        }
    }
    return power;
}

double estimate2(double sw, double target,double start) {

    double b = 5;
    double a = 0;
    double eps = 0.001;

    double step = (b - a) / 10;
    double pwr = start;
    int direction = 1;

    double iters = 0;
    while (std::abs(f2(sw, pwr) - target) > eps){ 
        
        if (f2(sw, pwr) > target) {
            if (direction == 1) {
                
                pwr += step;
            } else {
                
                direction = 1;
                step = step / 2;
                pwr+=step;
            }
        }
        else{
            if (direction==1){

                
                direction = -1;
                step = step/2;
                pwr+= step * direction;
            }
            else{

                
                pwr+= step * direction;
            }
        }


        iters++;
    }
    return pwr;
}

std::pair<std::vector<double>, std::vector<double>> 
parameter_estimation(std::vector<double> x, std::vector<double> y, std::vector<double> y2) {
    
    std::vector<double> pow1_vec={};
    std::vector<double> pow2_vec={};
    double init = 1.;
    double init2 = 1.;
    for (int i = 0; i < x.size() / 2; ++i) {
        std::cout << "=============" << x[i] << "=======" << y[i] << std::endl;
        init = estimate(x[i], y[i], init);
        
        init2 = estimate2(x[i], y2[i], init2);
        pow1_vec.push_back(init);
        pow2_vec.push_back(init2);
        
        std::cout << "i: " << i << " nw: " << init << " no: " << init2<< std::endl;
    }
    std::pair<std::vector<double>, std::vector<double>> res(pow1_vec, pow2_vec);
    return res;
}

void find_stats(std::vector<double> values){
    double mean = 0;
    for (double value : values) {
        mean += value;
    }
    mean /= values.size();

    // calculate the standard deviation
    double std_dev = 0;
    for (double value : values) {
        std_dev += std::pow(value - mean, 2);
    }
    std_dev /= values.size();
    std_dev = std::sqrt(std_dev);

    // calculate the L2-norm
    double norm = 0;
    for (double value : values) {
        norm += std::pow(value, 2);
    }
    norm = std::sqrt(norm);

    // print the results
    std::cout << "Mean: " << mean << std::endl;
    std::cout << "Standard deviation: " << std_dev << std::endl;
    // std::cout << "L2-norm: " << norm << std::endl;
}


int main(int argc, char *argv[]) {

    std::cout<<argv[1]<<std::endl;

    auto data = read_data(argv[1]);
    auto pwrs = parameter_estimation(data.first, data.second.first, data.second.second);
    std::cout<<"=====    nw:     ====="<<std::endl;
    find_stats(pwrs.first);
    std::cout<<std::endl;
    std::cout<<"=====    no:     ====="<<std::endl;
    find_stats(pwrs.second);

    
    return 0;
}


