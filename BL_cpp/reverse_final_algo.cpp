#include <iostream>
#include "source/BLproblem.h"
#include <cstdlib>
#include "math.h"


double get_mse( std::vector<double> target_sw, double no, double nw){
    
    
    double sw0 = 0.80;
    double so0 = 0.20; 
    int time_steps = 10000; //number of time steps for 1s (total steps = time_steps*T)
    BLproblem BL(100, sw0, so0, nw, no);
    auto sw = BL.solver(0, 0.01, 1000, sw0, so0); // returns pair (x, sw(x))  
    double mse = 0;
    for (int i=0; i<target_sw.size(); i++){
        mse+=(target_sw[i] - sw.second[i])*(target_sw[i] - sw.second[i]);
        // std::cout<<"this is mse:"<<mse<<std::endl;
    }
    mse/=target_sw.size();
    // std::cout<<"no: "<<no<<" nw: "<<nw<<" mse: "<<mse<<std::endl;
    return mse;
}

std::vector<double> find_local_min(double no, double nw, std::vector<double> target_sw){
    
    double next_no, next_nw;
    double step = 0.1;
    double center, up, left, right, down, lu, ld, ru, rd;
    
    center = get_mse(target_sw, no, nw);
    while (true)
    {
        double min = 10;
        left = get_mse(target_sw, no, nw - step);
        if (left < min && left < center)
        {
            min = left;
            next_no = no;
            next_nw = nw - step;
        }
        right = get_mse(target_sw, no, nw + step);
        if (right < min && right < center)
        {
            min = right;
            next_no = no;
            next_nw = nw + step;
        }
        up = get_mse(target_sw, no + step, nw);
        if (up < min && up < center)
        {
            min = up;
            next_no = no + step;
            next_nw = nw;
        }
        down = get_mse(target_sw, no - step, nw);
        if (down < min && down < center)
        {
            min = down;
            next_no = no - step;
            next_nw = nw;
        }
        ld = get_mse(target_sw, no - step, nw - step);
        if (ld < min && ld < center)
        {
            min = ld;
            next_no = no - step;
            next_nw = nw - step;
        }
        rd = get_mse(target_sw, no - step, nw + step);
        if (rd < min && rd < center)
        {
            min = rd;
            next_no = no - step;
            next_nw = nw + step;
        }
        ru = get_mse(target_sw, no + step, nw + step);
        if (ru < min && ru < center)
        {
            min = ru;
            next_no = no + step;
            next_nw = nw + step;
        }
        lu = get_mse(target_sw, no + step, nw - step);
        if (lu < min && lu < center)
        {
            min = lu;
            next_no = no + step;
            next_nw = nw - step;
        }
        if (min < center)
        {
            nw = next_nw;
            no = next_no;
            center = min;
            std::cout<<"choose: min="<<min<<"  no="<<no<<" nw="<<nw<<std::endl; 
        }
        else
        {
            std::vector<double> res = {no, nw, center};
            return res;
            break;
        }
    }
}



std::vector<double> parse_sw_from_file(std::string filename){
    std::vector<double> sw;
    
    double first_value;
    double second_value;
    std::ifstream file(filename);
    while (file >> first_value >> second_value) {
        
        sw.push_back(second_value);
    }
    return sw;
}


int main(int argc, char** argv) {

    double no;
    double nw;
    
    std::cout<<argc<<std::endl;
    for (int i = 0; i < argc; i++){
        std::cout<<"i: "<<argv[i]<<std::endl;
    }
    

    double sw0 = 0.80;
    double so0 = 0.20;
    std::string filename = "../ofp/sw_data";
    auto target_sw = parse_sw_from_file(filename);
    std::cout<<"parsed\n";
    std::cout<<"size: "<<target_sw.size();
    auto res = find_local_min(5., 5., target_sw);
    
    std::cout<<res[0]<<" "<<res[1]<<" "<<res[2]<<std::endl;
   
    
    
        // std::cout << "wrong number of parameters ('./BL_cpp nw no')" << std::endl;
    // }
    // else{
    //     nw = std::stod(argv[1]);
    //     no = std::stod(argv[2]);
    // }
    // int time_steps = 10000; //number of time steps for 1s (total steps = time_steps*T)
    // std::string filename = "../ofp/ofp_new";
    // for (double n_w: nw){
    //     for (double n_o:no){
    //         BLproblem BL(100, sw0, so0, n_w, n_o);
    //         auto sw = BL.solver(0, 0.01, 10000, sw0, so0); // returns pair (x, sw(x))
    //         std::string filename = "../ofp/sw_data_rev_" + std::to_string(n_w)+"_" +std::to_string(n_o);
    //         BL.save_SW(sw, filename);
    //         auto command = "python3 ../ofp/comparison.py ../ofp/sw_data " + filename;
    //         system(command.c_str());

    //      }
    // }
    
//    std::vector<double> times = {0.001,0.002,0.004,0.006,0.008,0.01};
//    BL.solver_dynamic(0, times, time_steps, sw0, so0);  //dynamic solution - generates files in folder 'results'

    
    // auto ofp = BL.get_OFP(sw.second);
    // BL.save_OFP(sw.second, ofp, filename);  //save to file, columns: sw, k_ro, k_rw
    // BL.save_SW(sw, "../ofp/sw_data_rev");
    // std::cout<<"===================================================="<<std::endl;
    // std::cout << "results saved to file: " + filename << std::endl<< std::endl<<"To plot solution use command:" << std::endl
    //           << "    python3 ../ofp/plot_ofp.py " + filename << std::endl;

    // std::cout<<"===================================================="<<std::endl;


    return 0;
}