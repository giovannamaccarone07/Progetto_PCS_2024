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
//
bool ImportaDati(const string& NomeFile, FractureStruct& Fract);

/// PianoPassantePerFrattura
//
//
MatrixXd RettaIntersezione(Vector4d& piano1, Vector4d& piano2);
///
///
///
///
///
Vector4d PianoPassantePerFrattura(const FractureStruct& fract, unsigned int& n);



/// CheckTraccia
// controlla se la retta passa per la frattura che giace nel piano
bool CheckTraccia(const FractureStruct& fract, TracesStruct trac, const MatrixXd& rettaIntersezione,unsigned int& n);
///CheckIntersezione
// controlla se c'è intersezione
bool checkIntersezione(const FractureStruct& fract, TracesStruct trac, unsigned int n1, unsigned int n2);
bool BoundingBox(const FractureStruct& fract, unsigned int n1, unsigned int n2);

/// ComputeTrace
///
///
bool ComputeTrace(TracesStruct trac,
                  FractureStruct fract,
                  double ts1, double ts2,
                  const MatrixXd& rettaIntersezione,
                  unsigned int n1, unsigned int n2,
                  bool pass1,bool pass2);
<<<<<<< HEAD

/// OutputTraces
///
bool OutputTraces(const TracesStruct& trac);

/// Output
///
bool Output(const TracesStruct& trac, const FractureStruct& frac);

/// OutputFractures
///
bool OutputFractures(const TracesStruct& trac, const FractureStruct& frac );


}
=======
>>>>>>> b62d7883dda0ea372709f7372442dfc96c2223d1


Matrix<double,2,3> ComputeBoundingBox(const FractureStruct& fract, unsigned int n);

bool Output(const TracesStruct& trac, const FractureStruct& frac);
bool pianiParalleli(Vector4d& piano1, Vector4d& piano2);

bool OutputFractures(const TracesStruct& trac, const FractureStruct& frac);

bool OutputTraces(const TracesStruct& trac);





}
