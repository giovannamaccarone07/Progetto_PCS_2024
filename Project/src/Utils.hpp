#pragma once

#include <iostream>
#include "Fratture.hpp"

using namespace std;
namespace FractureLibrary
{

/// ImportaDati
/// \brief ImportaDati
/// \param NomeFile
/// \param Fract
/// \return
///
bool ImportData(const string& fileName, FractureStruct& fract);


///
/// \brief RettaIntersezione
/// \param piano1
/// \param piano2
/// \return
///
Matrix<double,2,3> IntersectionLine(Vector4d& plane1, Vector4d& plane2);

///
/// \brief PianoPassantePerFrattura
/// \param fract
/// \param n
/// \return
///
Vector4d FracturePlane(FractureStruct& fract, const unsigned int& n);

/// CheckTraccia
/// \brief CheckTraccia
/// \param fract
/// \param trac
/// \param rettaIntersezione
/// \param n1
/// \param n2
/// \param tol
/// \return
///

bool CheckTraccia(FractureStruct& fract, TracesStruct& trac, const MatrixXd& intersectionLine, const unsigned int& n1, const unsigned int& n2, const double& tol);

///CheckIntersezione
/// \brief checkIntersezione
/// \param fract
/// \param trac
/// \param n1
/// \param n2
/// \param tol
/// \return
///
bool checkIntersezione( FractureStruct& fract, TracesStruct& trac, const unsigned int& n1, const  unsigned int& n2, const double& tol);

///
/// \brief BBoxIntersection
/// \param fract
/// \param n1
/// \param n2
/// \return
///
bool BBoxIntersection(const FractureStruct& fract, const unsigned int& n1, const unsigned int& n2,const double& tol);


///
/// \brief ComputeBoundingBox
/// \param fract
/// \param n
/// \return
///
Matrix<double,2,3> ComputeBoundingBox(const FractureStruct& fract, unsigned int n, const double& tol);

///
/// \brief Output
/// \param trac
/// \param frac
/// \return
///
bool GeneralOutput(const TracesStruct& trac, const FractureStruct& frac);

///
/// \brief pianiParalleli
/// \param piano1
/// \param piano2
/// \param tol
/// \return
///
bool parallelPlanes(Vector4d& plane1, Vector4d& plane2, const double& tol);

///
/// \brief OutputFractures
/// \param trac
/// \param frac
/// \return
///
bool OutputFractures(const TracesStruct& trac, const FractureStruct& frac);

///
/// \brief OutputTraces
/// \param trac
/// \return
///
bool OutputTraces(const TracesStruct& trac);

///
/// \brief ordineDecrescente
/// \param trac
/// \param lista
/// \param num
///
void descendingOrder(TracesStruct& trac, list<unsigned int>& list, const unsigned int& num, const double& tol);

///
/// \brief subPolygons
/// \param verticiPolygons
/// \param coordEstremiTracce
/// \param tol
/// \return
///
bool subPolygons(list<Vector3d> verticiPolygons, const vector<Matrix<double, 2,3>>& coordEstremiTracce, list<MatrixXd>& sp, const Vector3d& normale,  const double& tol);


}
