#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include "cmath"
#include <random>

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
    std::pair<std::vector<double>, std::vector<double>> params(z, y);
    std::pair<std::vector<double>, std::pair<std::vector<double>, std::vector<double>>> k(x, params);
//    std::pair<std::vector<double>,
//            std::pair<std::vector<double>, std::vector<double>>> res(x, k);
// закрытие файла
    file.close();
    return k;
}

std::vector<double> add_noise(std::vector<double> values) {
    std::default_random_engine generator; // генератор случайных чисел
    std::normal_distribution<double> distribution(0.0, 0.05); // нормальное распределение с дисперсией 5%

    for (auto &value: values) {
        double noise = distribution(generator) * value; // вычисление шума
        value += noise; // добавление шума к значению элемента вектора
    }

    // вывод измененного вектора значений
    for (const auto &value: values) {
        std::cout << value << " ";
    }

    return values;
}

int main(int argc, char *argv[]) {
    auto data = read_data(argv[1]);
    auto sw_with_noise = add_noise(data.first);
    std::ofstream output_file(argv[2]);
    if (output_file.is_open()) {
        for (int i = 0; i < data.first.size(); i++) {
            output_file << data.first[i] << " " << sw_with_noise[i] << " " << data.second.second[i] << " "
                        << data.second.first[i] << "\n";
        }
        output_file.close();
    } else {
        std::cout << "Unable to open output file.\n";
    }
    std::cout << "use:  python3 ../ofp/plot_sw_with_noise.py "<<argv[2] << std::endl;
    return 0;
};