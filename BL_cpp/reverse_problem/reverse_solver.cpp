#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include "cmath"

std::pair<std::vector<double>,
        std::pair<std::vector<double>, std::vector<double>>>
read_data(std::string filename) {
    std::vector<double> x, y, z;
    double x_val, y_val, z_val;

// открытие файла с координатами
    std::ifstream file(filename);

// проверка, удалось ли открыть файл
    if (!file.is_open()) {
        std::cout << "Ошибка открытия файла" << std::endl;
        exit(-1);
    }


// считывание координат из файла
    while (file >> x_val >> y_val >> z_val) {
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

double estimate(double sw, double target) {

    double b = 5;
    double a = 0;
    double eps = 0.001;

    double step = (b - a) / 10;
    double power = step;
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

double estimate2(double sw, double target) {

    double b = 5;
    double a = 0;
    double eps = 0.001;

    double step = (b - a) / 10;
    double power = step;
    int direction = 1;
    while (std::abs(f2(sw, power) - target) > eps) {
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

void parameter_estimation(std::vector<double> x, std::vector<double> y, std::vector<double> y2) {
    double init = 1.;
    double init2 = 1.;
    for (int i = 0; i < x.size() / 2; ++i) {
        std::cout << "=============" << x[i] << "=======" << y[i] << std::endl;
        init = estimate(x[i], y[i]);
        init2 = estimate2(x[i], y2[i]);
        std::cout << "i: " << i << " nw: " << init << " no: " << init2<< std::endl;
    }
}


int main() {

    auto data = read_data("../ofp/ofp2");
//    auto a = estimate(0.8, 0.576);
//    std::cout<<a<<std::endl;
    parameter_estimation(data.first, data.second.first, data.second.second);
    return 0;
}


