#pragma once
#include <iostream>
#include "Eigen/Eigen"
using namespace std;
using namespace Eigen;

namespace FractureLibrary{

struct FractureStruct{

    unsigned int NumeroFratture = 0;
    vector<unsigned int> IdFratture = {};
    unsigned int NumeroVeritici = 0;
    vector<Vector3d> CoordinateVertici = {};
    unsigned int SpazioMemoria = 0;

};


}
