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

double estimate(double x, double y, double init) {
    double eps = 0.001;
    double result = f(x, init);
    double correction=eps;
    double res;

    int direction = (result-y)/std::abs(result-y);
    while (std::abs(result - y) < eps){

    }

    else {
        correction = result/y/x;
        if (result<y){correction*=-1;}
        std::cout<<"correction: "<<correction<<" init: "<<init<<std::endl;
        init += correction;

        return estimate(x,y,init);

    }

}

void parameter_estimation(std::vector<double> x, std::vector<double> y){
    double init = 1.;
    for (int i = 0; i < x.size()/2; ++i) {
        std::cout<<"============="<<x[i]<<"======="<<y[i]<<std::endl;
        init = estimate(x[i],y[i],init);
        std::cout<<"i: "<<i<<" power: "<<init<<std::endl;
    }
}


int main(){
    std::cout<<"hui";
    auto data = read_data("../ofp/ofp1");
    auto a = estimate(0.8, 0.576,1);
//    parameter_estimation(data.first, data.second);
    return 0;
}


