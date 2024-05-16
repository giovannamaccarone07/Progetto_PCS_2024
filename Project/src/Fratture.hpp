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
    vector<VectorXi> IndiciVertici = {};          // array contenente indici assegnati in ordine crescente ai vertici man mano che si legge il file, raggruppati in vettori che comprendono i vertici di una singola frattura
    vector<Vector3d> CoordinateVertici = {};      // array contentente i vettori contenente vettori di 3 coordinate l'uno ordinate in modo che gli indici corridpondando alla posizione delle coordinate del vertice
    unsigned int SpazioMemoria = 0;               // è una variabile che permette di riservare spazio per la registrazione dei dati nella struttura in base alla lunghezza del file che si sta leggendo

};


}
