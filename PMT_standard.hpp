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

    // double x1 = 2.3;
    // double y1 = 30.7;

    // double x2 = 30.7;
    // double y2 = 30.7;

    // double x3 = 30.7;
    // double y3 = 2.3;

    // double x4 = 2.3;
    // double y4 = 2.3;

    // double zGEM = 19;


    //--- Parameters for CYGNO04 ---//

    double x1 = 12.5;
    double y1 = 80;

    double x2 = 37.5;
    double y2 = 80;

    double x4 = 37.5;
    double y3 = 53.3;

    double x3 = 12.5;
    double y4 = 53.3;

    double zGEM = 59;


    //-------------------------//
    
    double data[4] = {0.};

    double xTrue = 0.;
    double yTrue = 0.;

};

#endif /* PMT_standard_hpp */