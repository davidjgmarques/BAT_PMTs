//
//  PMT_standard.hpp
//  LIMEPMTfits
//
//  Created by Stefano Piacentini on 23/09/22.
//  Modified by Francesco Borra on 28/06/23
//
#ifndef PMT_standard_hpp
#define PMT_standard_hpp

#include <BAT/BCModel.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <omp.h>

#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"

#include "Math/ProbFunc.h"
// #include "helper.hpp"


class PMTfit : public BCModel
{
public:

    PMTfit(const std::string& mode, int nth, int index, double *L, double x, double y);

    ~PMTfit(){};

    double LogLikelihood(const std::vector<double>& pars);

    double LogAPrioriProbability(const std::vector<double>& pars);

    double D2(double x, double y, int i);
    
private:
    double Lmax;
    double cmax;
    std::string mode_;
    int index_;


    // prior parameters
    double L_mean = 0;
    double L_std = 0;
    
    double c_mean[4] = {0.};
    double c_std[4] = {0.};
    
    
    //--------- PMT positions (in cm) ---------//

    
    //--- Parameters for LIME ---//

    double x1 = 2.3;
    double y1 = 30.7;

    double x2 = 30.7;
    double y2 = 30.7;

    double x3 = 30.7;
    double y3 = 2.3;

    double x4 = 2.3;
    double y4 = 2.3;

    
    //--- Parameters for CYGNO-04 ---//

    // -------- PMTS 1-2-3-4
    
    // double x1 = 11.67;
    // double y1 = 80.00;

    // double x2 = 38.33;
    // double y2 = 80.00;

    // double x3 = 11.67;
    // double y3 = 53.33;

    // double x4 = 38.33;
    // double y4 = 53.33;


    // -------- PMTS 3-4-5-6
    
    ///// DX = -0.83333
    
    // double x1 = 12.50;
    // double y1 = 53.33;

    // double x2 = 37.50;
    // double y2 = 53.33;

    // double x3 = 12.50;
    // double y3 = 26.67;

    // double x4 = 37.50;
    // double y4 = 26.67;


    ///// DX = 0

    // double x1 = 11.67;
    // double y1 = 53.33;

    // double x2 = 38.33;
    // double y2 = 53.33;

    // double x3 = 11.67;
    // double y3 = 26.67;

    // double x4 = 38.33;
    // double y4 = 26.67;


    ///// DX = 1.66666

    // double x1 = 10.00;
    // double y1 = 53.33;

    // double x2 = 40.00;
    // double y2 = 53.33;

    // double x3 = 10.00;
    // double y3 = 26.67;

    // double x4 = 40.00;
    // double y4 = 26.67;


    ///// DX = 4.166666

    // double x1 = 7.50;
    // double y1 = 53.33;

    // double x2 = 42.50;
    // double y2 = 53.33;

    // double x3 = 7.50;
    // double y3 = 26.67;

    // double x4 = 42.50;
    // double y4 = 26.67;

    // -------- PMTS 5-6-7-8

    // double x1 = 11.67;
    // double y1 = 26.67;

    // double x2 = 38.33;
    // double y2 = 26.67;

    // double x3 = 11.67;
    // double y3 = 0.0;

    // double x4 = 38.33;
    // double y4 = 0.0;


    // ---- Distance from the GEM plane

    // LIME
    double zGEM = 19;

    // CYGNO-04
    // double zGEM = 59;


    //-------------------------//
    
    double data[4] = {0.};

    double xTrue = 0.;
    double yTrue = 0.;

};

#endif /* PMT_standard_hpp */