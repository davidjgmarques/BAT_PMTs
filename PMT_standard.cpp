//
//  PMT_standard.cpp
//  LIMEPMTfits
//
//  Created by Stefano Piacentini on 23/09/22.
//  Modified by Francesco Borra on 28/06/23.
//

#include "PMT_standard.hpp"
#include <TMath.h>
#include <BAT/BCMath.h>
#include <cmath>


// PMTfit class
PMTfit::PMTfit(const std::string& mode, int nth, 
               int index, double *L, double x, double y) : BCModel(mode)
{
    std::cout<<"Starting fit for '"<<mode<<" reconstruction'"<<std::endl;

    mode_ = mode;
    Lmax = 20000;
    cmax = 2;
    index_ = index;
    xTrue = x;
    yTrue = y;

    // std::cout << "Dentro il fit" << std::endl << std::endl;

    for (int i = 0; i < 4; ++i) {
        data[i] = L[i];
        // std::cout << "L"<<i<<":\t" << L[i] << "\t";
    }
        // std::cout << std::endl;
        
    //DEFINING parameters
    if (mode_.compare("association") == 0) {
        AddParameter("L", 0, Lmax, "L", "[a.u.]");

        //--- Parameters for LIME ---//
        // AddParameter("x", 0, 33, "x", "[cm]");
        // AddParameter("y", 0, 33, "y", "[cm]");
        
        //--- Parameters for CYGNO04 ---//
        AddParameter("x", 0, 50, "x", "[cm]");
        AddParameter("y", 0, 80, "y", "[cm]");
        
        AddParameter("c1", 0., cmax, "c1", "[counts]");
        AddParameter("c2", 0., cmax, "c2", "[counts]");
        AddParameter("c3", 0., cmax, "c3", "[counts]");
        AddParameter("c4", 0., cmax, "c4", "[counts]");
        
        //------- PMT CALIBRATION -------//         
        
        //--- Parameters for CYGNO 04 ---//
        // We are using simulation, so the PMTs are intrinsically inter-calibrated
        GetParameter("c1").Fix(1.0);
        GetParameter("c2").Fix(1.0);
        GetParameter("c3").Fix(1.0);
        GetParameter("c4").Fix(1.0);

        //--- Parameters for LIME ---//
        // GetParameter("c1").Fix(1.0);
        // GetParameter("c2").Fix(0.965);
        // GetParameter("c3").Fix(0.860);
        // GetParameter("c4").Fix(0.827);

    } else if (mode_.compare("PMTcalibration") == 0){

        // MISSING changes for CYGNO-04, only necessary when it's working

        AddParameter("L", 0, Lmax, "L", "[a.u.]");
        GetParameter("L").Fix(40000.0); // just to have c_i values smaller, can put any value, 
                                        // we are only interested in the c_i ratios
        AddParameter("x", 0, 33, "x", "[cm]");
        AddParameter("y", 0, 33, "y", "[cm]"); 
        //  FIXING x and y COORDINATES         
        GetParameter("x").Fix(xTrue);
        GetParameter("y").Fix(yTrue);

        // The prior for the c_i can be tweaked to reduce parameter space
        AddParameter("c1", 0., cmax, "c1", "[counts]");
        AddParameter("c2", 0., cmax, "c2", "[counts]");
        AddParameter("c3", 0., cmax, "c3", "[counts]");
        AddParameter("c4", 0., cmax, "c4", "[counts]");

    } else {
        throw std::runtime_error("Unknown model '"+mode_+"'.\n");
    }
    
    omp_set_dynamic(0);
    omp_set_num_threads(nth);
}


// Compute Likelihood
double PMTfit::LogLikelihood(const std::vector<double>& pars) {

    double LL = 0.;
    
    for(unsigned int j=0; j<4; j++) {
        double Lj = data[j];  // here the data
        // std::cout<< "i: " << j << "\t" << "L: "<< Lj << std::endl;

        double sLj = 0.1*Lj; // for now set to 10% of the integral
        
        int k = 3 +j;  // index for c_i (pars[3] is c_1 and so on)
        // std::cout<< "k: " << k << "\t" << "cj: "<< pars[k] << std::endl;
        std::cout.flush();
        double tmp = D2(pars[1], pars[2], j);                // compute r_i**2
        LL += BCMath::LogGaus(Lj,                             // x, namely Lj
                             (pars[0]*pars[k])/(pow(tmp, 2)), // mu, namely the light computed in the step (c_i * Lj / r_i^4)
                             sLj,                             // sigma
                             true                             // norm factor
                             );
    }

    return LL;
}

// Define prior
double PMTfit::LogAPrioriProbability(const std::vector<double>& pars) {
    double LL = 0.;
    //flat priors everywhere
    if(pars[0]<0 || pars[0]>Lmax) {
        LL += log(0.0);
    } else {
        LL += log(1.0/Lmax);
    }

    //--- Parameters for LIME ---//
    // for(unsigned int i=1; i<3; i++) {
    //     if(pars[i]<0 || pars[i]>33.) {
    //         LL += log(0.0);
    //     } else {
    //         LL += log(1.0/33.);
    //     }
    // }
    
    //--- Parameters for CYGNO 04 ---//
    for(unsigned int i=1; i<3; i++) {

        if (i == 1) {
            if(pars[i]<0 || pars[i]>50.) {
                LL += log(0.0);
            } else {
                LL += log(1.0/50.);
            }
        }
        else if (i == 2) {
            if(pars[i]<0 || pars[i]>80.) {
                LL += log(0.0);
            } else {
                LL += log(1.0/80.);
            }
        }
    }
    
    for(unsigned int j=3; j<7; j++){
        if(pars[j]<0. || pars[j]>cmax) {
            LL += log(0.0);
        } else {
            LL += log(1.0/cmax);
        }
    }
        
    return LL;
}

// Function to calculate distance between the PMT and the chosen position
double PMTfit::D2(double x, double y, int i) {
    if (i == 0) {
        return (x - x1)*(x - x1) + (y - y1)*(y - y1) + zGEM*zGEM;
    } else if (i == 1) {
        return (x - x2)*(x - x2) + (y - y2)*(y - y2) + zGEM*zGEM;
    } else if (i == 2) {
        return (x - x3)*(x - x3) + (y - y3)*(y - y3) + zGEM*zGEM;
    } else if (i == 3) {
        return (x - x4)*(x - x4) + (y - y4)*(y - y4) + zGEM*zGEM;
    } else {
        throw std::runtime_error("Uknown value of PMT index.\n");
    }
    return 0.;
}