#pragma once
#include <iostream>
#include "Eigen/Eigen"
using namespace std;
using namespace Eigen;

namespace FractureLibrary{

struct FractureStruct
{

    unsigned int NumeroFratture = 0;
    vector<unsigned int> IdFratture = {};
    vector<unsigned int> NumeroVertici = {};
    vector<VectorXi> Vertici = {};
    unsigned int SpazioMemoria = 0;

};


}
