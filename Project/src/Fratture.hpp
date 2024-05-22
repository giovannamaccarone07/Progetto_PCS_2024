#pragma once
#include <iostream>
#include "Eigen/Eigen"
using namespace std;
using namespace Eigen;

namespace FractureLibrary{



struct FractureStruct
{
    /// IdFratture
    /// NumeroVertici
    /// IndiciVertici
    // ogni frattura è composta da un numero di vertici. Ad ogni vertice corrisponde un indice.
    // Ad ogni posizione del vector corrisponde un vector<unsigned int> che memorizza gli indici dei vertici di quella frattura
    // Siccome ogni frattura ha un numero variabile di vertici non sappiamo a priori la dimensione del vector<unsigned int> interno.
    // Mentre quello esterno sarà lungo quanto NumeroFratture
    /// CoordinateVertici
    // vector contentente i vettori di 3 coordinate l'uno ordinate in modo che gli indici corrispondando alla posizione
    // delle coordinate del vertice. Questa struttura è di fatto una matrice NumeroVertici x 3
    // (però in questo modo cambia la sintassi per accedere)
    /// SpazioMemoria
    // è una variabile che permette di riservare spazio per la registrazione dei dati nella struttura
    // in base alla lunghezza del file che si sta leggendo

    unsigned int NumeroFratture = 0;
    vector<unsigned int> IdFratture = {};               /// dim: NumeroFratture
    vector<unsigned int> NumeroVertici = {};            /// dim: NumeroFratture
    vector<vector<unsigned int>> IndiciVertici = {};    /// dim: NumeroFratture x  vector<unsigned int>
    vector<Vector3d> CoordinateVertici = {};            /// dim: NumeroVertici x 3
    unsigned int SpazioMemoria = 0;

};


}

