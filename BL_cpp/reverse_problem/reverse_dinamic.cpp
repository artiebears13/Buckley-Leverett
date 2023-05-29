#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "math.h"
#include <random>

#define N_T 6

class ReverseSolver{
public:
    double t0;
    double sw0;
    double swn;
    double porosity;
    double mu_w;
    double mu_o;
    double h;
    double N;
    int n_times;
    std::vector<double> times;
    std::vector<std::vector<double>> x;
    std::vector<std::vector<double>> so;
    std::vector<std::vector<double>> sw;
    std::vector<std::vector<double>> k_rw;
    std::vector<std::vector<double>> k_ro;
    std::vector<std::vector<double>> pc;

    std::vector<std::vector<double>> n_w;
    std::vector<std::vector<double>> n_o;
    std::string working_file;
    double std;

    


    ReverseSolver(std::string filename, int N_steps, double input_std){
        working_file = filename;
        read_file(filename);
        n_times = N_steps;
        std = input_std;

    }

    void read_file(std::string filename){
        unsigned line_number = 1;
        std::string line;
        std::ifstream file(filename);
        int current_time = 0;
        int current_x = 0;

        std::vector<double> cur_x;
        std::vector<double> cur_so;
        std::vector<double> cur_sw;
        std::vector<double> cur_k_rw;
        std::vector<double> cur_k_ro;
        std::vector<double> cur_pc;

     

        while (getline(file, line))
        {
            
            if (line_number >= 3 && line_number <= 10){
                read_params(line);
                }
            
            if (line_number == 11){
                auto times_splitted = split(line);
                n_times = times_splitted.size()-1;
                for (int i = 1; i < times_splitted.size(); i++){
                    times.push_back(std::stod(times_splitted[i]));
                }
                
            }

            if (line_number>11){
                if (current_x==100){
                    current_x = 0;
                    current_time++;
                    x.push_back(cur_x);
                    sw.push_back(cur_sw);
                    so.push_back(cur_so);
                    k_ro.push_back(cur_k_ro);
                    k_rw.push_back(cur_k_rw);
                    pc.push_back(cur_pc);
                    cur_x={};
                    cur_so={};
                    cur_sw={};
                    cur_k_rw={};
                    cur_k_ro={};
                    cur_pc={};
                }
                auto splitted_line = split(line);
                cur_x.push_back(std::stod(splitted_line[0]));
                cur_sw.push_back(std::stod(splitted_line[1]));
                cur_so.push_back(std::stod(splitted_line[2]));
                cur_k_ro.push_back(std::stod(splitted_line[3]));
                cur_k_rw.push_back(std::stod(splitted_line[4]));
                cur_pc.push_back(std::stod(splitted_line[5]));

                current_x++;
                
            }
            line_number++;
        }
        x.push_back(cur_x);
        sw.push_back(cur_sw);
        so.push_back(cur_so);
        k_ro.push_back(cur_k_ro);
        k_rw.push_back(cur_k_rw);
        pc.push_back(cur_pc);
    }

    void read_params(std::string line){
        auto parameter = line.substr(0, line.find(" "));
        parameter = parameter.substr(0, line.find(":"));
        double value = std::stod(line.substr(line.rfind(" ")));
        
        if (parameter == "t0"){
            t0 = value;
            return;}
        if (parameter =="sw0"){
            sw0 = value;
            return;}
        if (parameter == "swn"){
            swn = value;
            return;}
        if (parameter == "porosity"){

            porosity = value;
            return;}
        if (parameter == "mu_w"){
            mu_w = value;
            return;}
        if (parameter == "mu_o"){
            mu_o = value;
            return;}
        if (parameter == "h"){
            h = value;
            return;}
        if (parameter == "N"){
            N = value;
            return;}

        return;
    }

    std::vector<std::string> 
    split(std::string str) {
        std::vector<std::string> result;
        std::stringstream ss(str);
        std::string token;
        while (getline(ss, token, ' ')) {
            result.push_back(token);
        }
        return result;
    }

    void
    add_noise_dynamic(){
        std::default_random_engine generator; // генератор случайных чисел
        generator.seed(42);
        for (int i = 0; i < n_times; i++){
            sw[i] = add_noise_static(sw[i], generator);
        }
        plot_noise();

        
    }

    void plot_noise(){
        auto filename = working_file + "_noised.png";
        // std::cout << "writing results to file: " + filename << std::endl;
        // std::cout << "LINE: x sw ko kw pc " << std::endl;

        std::ofstream file(filename);
        file << "file written by Artem Medvedev" << std::endl;
        file << "PARAMS:" << std::endl;
        file << "t0: " << t0 << std::endl;
        file << "sw0: " << sw0 << std::endl;
        file << "swn: " << swn << std::endl;
        file << "porosity: " << porosity << std::endl;
        file << "mu_w: " << mu_w << std::endl;
        file << "mu_o: " << mu_o << std::endl;
        file << "h: " << h << std::endl;
        file << "N: " << N << std::endl;
        file << "t";
        for (int i = 0; i < times.size(); ++i) {
            file << " " << times[i];
        }
        file << std::endl;

        for (int i = 0; i < times.size(); ++i) {
            // std::cout << "solving for t = " << times[i] << std::endl;

            
            for (int j = 0; j < N; ++j) {
                file << x[i][j] << " " << sw[i][j] << " " << 1 - sw[i][j] << " "
                    << k_ro[i][j] << " " << k_rw[i][j] << " "
                    << pc[i][j] << std::endl;
            }
        }
        // std::cout << "writing completed!" << std::endl;
        // auto command = "python3 ../source/visualize.py " + filename;
        // std::cout << "creating graphs:" << std::endl;
        // system(command.c_str());


    }

    // Add noise - normal distribution with mu=0, std=5% of value
    std::vector<double> 
    add_noise_static(std::vector<double> values, std::default_random_engine& generator) {

        
        std::normal_distribution<double> distribution{0.0, std}; // нормальное распределение с дисперсией 5%

        // for (auto &value: values) {
        for (int i = 0; i < values.size(); i++){
            double noise = distribution(generator) ; // вычисление шума
            values[i] += noise; // добавление шума к значению элемента вектора
        }

        // вывод измененного вектора значений
        

        return values;
    }


    std::pair<std::pair<double, double>, std::pair<double, double>>
    parameter_estimation_dynamic(){
        
        for (int i = 0; i < n_times; i++){
	//for (int i = 0; i < 1; i++){
            auto cur_corey_coefs = parameter_estimation_static(sw[i], k_ro[i],k_rw[i]);
            n_o.push_back(cur_corey_coefs.first);
            n_w.push_back(cur_corey_coefs.second);

            std::cout<<"======================================"<<std::endl;
            std::cout<<"=====    no:     ====="<<std::endl;
            find_stats_static(cur_corey_coefs.first);
            std::cout<<std::endl;
            std::cout<<"=====    nw:     ====="<<std::endl;
            find_stats_static(cur_corey_coefs.second);

        }
        auto res = find_stats();
        return res;
        
    }

    void find_stats_static(std::vector<double> values){
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
        std::cout << "L2-norm: " << norm << std::endl;
        
    }

    std::pair<std::pair<double, double>, std::pair<double, double>>
    find_stats(){
        double mean_no = 0;
        double mean_nw = 0;
        double std_dev_no = 0;
        double std_dev_nw = 0;
        double size_of_vectors = N_T*N/2;
        double size_check=0;
        std::cout<<"======STATS: "<<size_of_vectors<<std::endl;
        std::cout<<"len(n_o): "<<n_o.size()<<std::endl;

        for (std::vector<double> no_vec : n_o){
            for (int i=0; i<no_vec.size(); i++){// no_value : no_vec){
                // std::cout<<no_vec[i]<<std::endl;
                if (!std::isnan(no_vec[i])){
                mean_no+=no_vec[i];
                }
                else{
                    mean_no+=no_vec[i-1];
                }
                size_check++;
            }
        }
        
        
        for (std::vector<double> nw_vec : n_w){
            for (int i=0; i<nw_vec.size(); i++){// no_value : no_vec){
                if (!std::isnan(nw_vec[i])){
                mean_nw+=nw_vec[i];
                }
                else{
                    mean_nw+=nw_vec[i-1];
                }
            }
        }
        std::cout<<"======STATS_check: "<<size_check<<std::endl;

        mean_nw/=size_check;
        mean_no/=size_check;
        size_check = 0;
        for (std::vector<double> no_vec : n_o){
            for (int i=0; i<no_vec.size(); i++){
                if (!std::isnan(no_vec[i])){
                    
                    std_dev_no += std::pow(no_vec[i] - mean_no, 2);
                }
                else{
                    std_dev_no += std::pow(no_vec[i-1] - mean_no, 2);
                }
                size_check++;
            }

        }
        
        for (std::vector<double> nw_vec : n_w){
            for (int i=0; i<nw_vec.size(); i++){
                if (!std::isnan(nw_vec[i])){
                    // size_check++;
                    std_dev_nw += std::pow(nw_vec[i] - mean_nw, 2);
                }
                else{
                    std_dev_nw += std::pow(nw_vec[i-1] - mean_nw, 2);
                }
            }
        }

        double l2_no = std::sqrt(std_dev_no);
        double l2_nw = std::sqrt(std_dev_nw);


        // calculate the standard deviation
        
        std_dev_no /= size_check;
        std_dev_no = std::sqrt(std_dev_no);
        
        std_dev_nw /= size_check;
        std_dev_nw = std::sqrt(std_dev_nw);

        // calculate the L2-norm
        // double norm = 0;
        // for (double value : values) {
        //     norm += std::pow(value, 2);
        // }
        // norm = std::sqrt(norm);

        // print the results
        
        std::cout<<"\t\t  RESULTS FOR "<<n_times<<" TIME STEPS: "<<std::endl;
        std::cout<<"============================="<<std::endl;
        std::cout<<"               n_o           "<<std::endl<<std::endl;
        std::cout << "Mean: " << mean_no << std::endl;
        std::cout << "Standard deviation: " << std_dev_no << std::endl;
        // std::cout << "L2 norm: " << l2_no << std::endl;

        std::cout<<"============================="<<std::endl;
        std::cout<<"               n_w           "<<std::endl<<std::endl;
        std::cout << "Mean: " << mean_nw << std::endl;
        std::cout << "Standard deviation: " << std_dev_nw << std::endl;
        // std::cout << "L2 norm: " << l2_nw << std::endl;
        // std::cout << "L2-norm: " << norm << std::endl;
        std::pair<double, double> no_stats(mean_no, std_dev_no);
        std::pair<double, double> nw_stats(mean_nw, std_dev_nw);
        std::pair<std::pair<double, double>, std::pair<double, double>> stats(no_stats, nw_stats);
        return stats;
        
}

void write_res(int number, std::pair<std::pair<double, double>, std::pair<double, double>> res)
{
        std::string file_name = "../results/res_std_" + std::to_string(std) + ".txt";
        std::ofstream myfile;
        myfile.open(file_name, std::ios::app);
        myfile << number << " " << res.first.first << " " << res.first.second << " " << res.second.first << " " << res.second.second << std::endl;
        myfile.close();
}

    std::pair<std::vector<double>, std::vector<double>> 
    parameter_estimation_static(std::vector<double> x, std::vector<double> k_ro, std::vector<double> k_rw) {
        
        std::vector<double> no_vec={};
        std::vector<double> nw_vec={};
        double init_no = 1.;
        // std_dev_nw /= size;
        double init_nw = 1.;
        for (int i = 0; i < x.size() / 2; ++i) {
            // std::cout << "=============" << x[i] << "=======" << k_ro[i] << std::endl;
            init_no = estimate_no_log(x[i], k_ro[i], init_no);
            
            init_nw = estimate_nw_log(x[i], k_rw[i], init_nw);
            no_vec.push_back(init_no);
            nw_vec.push_back(init_nw);
            
            // std::cout << "i: " << i << " nw: " << init_no << " no: " << init_nw<< std::endl;
        }
        std::pair<std::vector<double>, std::vector<double>> res(no_vec, nw_vec);
        return res;
    }



    double f_w(double x, double power) {
        double kw = 0.9;
    //    std::cout<<"x: "<<x<<" pow: "<<power<<std::endl;
        return std::pow(x, power) * kw;
    }

    double f_o(double x, double power) {
        double ko = 1;
    //    std::cout<<"x: "<<x<<" pow: "<<power<<std::endl;
        return std::pow(1-x, power) * ko;
    }

    double estimate_no_log(double sw, double target, double start) {
        double ko0 = 1;
        double no = std::log(target/ko0)/std::log(1.0-sw);
        return no;

    }

    double estimate_nw_log(double sw, double target, double start) {
        double kw0 = 0.9;
        double nw = std::log(target/kw0)/std::log(sw);
        return nw;

    }

    double estimate_no(double sw, double target, double start) {

        double b = 5;
        double a = 0;
        double eps = 0.001;

        double step = (b - a) / 10;
        double power = start;
        int direction = 1;

        while (std::abs(f_o(sw, power) - target) > eps) {
            // std::cout<<power<<std::endl;
            if (f_o(sw, power) > target) {
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

    
    double estimate_nw(double sw, double target,double start) {
        ///\param target: k_rw
        ///
        double b = 5;
        double a = 0;
        double eps = 0.001;

        double step = (b - a) / 10;
        double pwr = start;
        int direction = 1;

        double iters = 0;
        while (std::abs(f_w(sw, pwr) - target) > eps){ 
            
            if (f_w(sw, pwr) > target) {
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

};

//res_31-03-2023_17-20-20
int main(int argc, char *argv[]){
    std::vector<double> std_vec;
    int std_steps = 10;
    double std_step = 0.05/std_steps;
    for (int i = 0; i < std_steps; i++)
    {
        std_vec.push_back(0.005 + i * std_step);
    }
    
    int n_steps = std::stoi(argv[2]);
    
    // for (int i = 0; i < solver.sw.size(); i++){
    //     std::cout << solver.x[i][0]<<" "
    //         <<solver.sw[i][0]<<" "
    //         <<solver.so[i][0]<<" "
    //         <<solver.k_ro[i][0]<<" "
    //         <<solver.k_rw[i][0]<<" "
    //         <<solver.pc[i][0]<<" "<<std::endl;
            
    // }
    // std::cout<<solver.t0<<" "<<solver.sw0<<" "<<solver.swn<<" "<<solver.porosity<<" "<<solver.mu_w<<" "<<solver.mu_o<<" "<<solver.h<<" "<<solver.N<<std::endl;
    // for (int i = 0; i < solver.n_times; i++){
        // std::cout<<solver.times[i]<<std::endl;
    // }
    for (auto std_dev: std_vec){
        for (int i = 1; i < n_steps; i++)
        {
            ReverseSolver solver(argv[1], i, std_dev);
            solver.add_noise_dynamic();
            auto res = solver.parameter_estimation_dynamic();
            solver.write_res(i, res);
        }
    }
    
    
    

return 0;
}
