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
    // ATTENZIONE
    // nel nuovo aggiornamento CoordinateVertici è un vector che contiene in ogni posizione la matrice dei vertici
    // anzichè avere la matrice unica abbiamo tante piccole matrici frammentate.

    /// SpazioMemoria
    // è una variabile che permette di riservare spazio per la registrazione dei dati nella struttura
    // in base alla lunghezza del file che si sta leggendo

    unsigned int NumeroFratture = 0;
    vector<unsigned int> IdFratture = {};               /// dim: 1 x NumeroFratture
    vector<unsigned int> NumeroVertici = {};            /// dim: 1 x NumeroFratture
    vector<vector<unsigned int>> IndiciVertici = {};    /// dim: vector<unsigned int> x NumeroFratture
    vector<MatrixXd> CoordinateVertici = {};            /// dim: MatrixXd x NumeroVertici
    unsigned int SpazioMemoria = 0;

};


}

