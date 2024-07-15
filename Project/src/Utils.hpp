#pragma once

#include <iostream>
#include "Fratture.hpp"

using namespace std;
namespace FractureLibrary
{

bool ImportData(const string& fileName, FractureStruct& fract);

Matrix<double,2,3> IntersectionLine(Vector4d& plane1, Vector4d& plane2);

Vector4d FracturePlane(FractureStruct& fract, const unsigned int& n);

bool CheckTraccia(FractureStruct& fract, TracesStruct& trac, const MatrixXd& intersectionLine, const unsigned int& n1, const unsigned int& n2, const double& tol);

bool checkIntersezione( FractureStruct& fract, TracesStruct& trac, const unsigned int& n1, const  unsigned int& n2, const double& tol);

bool BBoxIntersection(const FractureStruct& fract, const unsigned int& n1, const unsigned int& n2,const double& tol);

Matrix<double,2,3> ComputeBoundingBox(const FractureStruct& fract, unsigned int n, const double& tol);

bool parallelPlanes(Vector4d& plane1, Vector4d& plane2, const double& tol);

bool OutputFractures(const TracesStruct& trac, const FractureStruct& frac);

bool OutputTraces(const TracesStruct& trac);

void descendingOrder(TracesStruct& trac, list<unsigned int>& list, const unsigned int& num, const double& tol);

bool subPolygons(list<Vector3d> verticiPolygons, const vector<Matrix<double, 2,3>>& coordEstremiTracce, list<MatrixXd>& sp, const Vector3d& normale,  const double& tol);


}
