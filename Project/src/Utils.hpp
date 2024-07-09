#pragma once

#include <iostream>
#include "Fratture.hpp"

using namespace std;
namespace FractureLibrary
{

/// ImportaDati
// NomeFile: prende come riferimento costante il path del file
// Fract: prende la struct di tipo FractureStruct come riferimento e può modificarla
// Funzione che importa i dati da file, restituisce TRUE se la lettura è andata a buon fine
/// \brief ImportaDati
/// \param NomeFile
/// \param Fract
/// \return
///
bool ImportaDati(const string& NomeFile, FractureStruct& Fract);


///
/// \brief RettaIntersezione
/// \param piano1
/// \param piano2
/// \return
///
Matrix<double,2,3> RettaIntersezione(Vector4d& piano1, Vector4d& piano2);

///
/// \brief PianoPassantePerFrattura
/// \param fract
/// \param n
/// \return
///
Vector4d PianoPassantePerFrattura(FractureStruct& fract, const unsigned int& n);

/// CheckTraccia
// controlla se la retta passa per la frattura che giace nel piano
///
/// \brief CheckTraccia
/// \param fract
/// \param trac
/// \param rettaIntersezione
/// \param n1
/// \param n2
/// \param tol
/// \return
///
bool CheckTraccia(FractureStruct& fract, TracesStruct& trac, const MatrixXd& rettaIntersezione, const unsigned int& n1, const unsigned int& n2, const double& tol);

///CheckIntersezione
// controlla se c'è intersezione
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
/// \brief IntersezioneBoundingBox
/// \param fract
/// \param n1
/// \param n2
/// \return
///
bool IntersezioneBoundingBox(const FractureStruct& fract, const unsigned int& n1, const unsigned int& n2);

///
/// \brief ComputeTrace
/// \param trac
/// \param fract
/// \param ts1
/// \param ts2
/// \param rettaIntersezione
/// \param n1
/// \param n2
/// \param pass1
/// \param pass2
/// \return
///
bool ComputeTrace(TracesStruct& trac,
                  FractureStruct& fract,
                  double ts1, double ts2,
                  const MatrixXd& rettaIntersezione,
                  const unsigned int& n1, const unsigned int& n2,
                  bool pass1,bool pass2);

///
/// \brief ComputeBoundingBox
/// \param fract
/// \param n
/// \return
///
Matrix<double,2,3> ComputeBoundingBox(const FractureStruct& fract, unsigned int n);

///
/// \brief Output
/// \param trac
/// \param frac
/// \return
///
bool Output(const TracesStruct& trac, const FractureStruct& frac);

///
/// \brief pianiParalleli
/// \param piano1
/// \param piano2
/// \param tol
/// \return
///
bool pianiParalleli(Vector4d& piano1, Vector4d& piano2, const double& tol);

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
void ordineDecrescente(TracesStruct& trac, list<unsigned int>& lista, const unsigned int& num);

///
/// \brief subPolygons
/// \param listaTracce
/// \param verticiPolygons
/// \param coordEstremiTracce
/// \param tol
/// \return
///
bool subPolygons(const MatrixXd& verticiPolygons, const vector<Matrix<double, 2,3>>& coordEstremiTracce, const Vector3d& normale,  const double& tol);


}
