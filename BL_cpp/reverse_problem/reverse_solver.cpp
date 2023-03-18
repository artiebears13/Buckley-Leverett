#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include "cmath"

std::pair<std::vector<double>,
        std::vector<double>>
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
    std::pair<std::vector<double>, std::vector<double>> k(x, z);
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

//void parameter_estimation(std::vector<double> x, std::vector<double> y) {
//    double init = 1.;
//    for (int i = 0; i < x.size() / 2; ++i) {
//        std::cout << "=============" << x[i] << "=======" << y[i] << std::endl;
//        init = estimate(x[i], y[i], init);
//        std::cout << "i: " << i << " power: " << init << std::endl;
//    }
//}


int main() {

    auto data = read_data("../ofp/ofp1");
    auto a = estimate(0.8, 0.576);
//    parameter_estimation(data.first, data.second);
    return 0;
}


