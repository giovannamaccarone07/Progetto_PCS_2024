#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <limits>
using namespace std;

namespace FractureLibrary{

bool ImportDati(const string& NomeFile, FractureStruct& fract){

    ifstream file;
    file.open(NomeFile);
    if(file.fail()){
        return false;
    }
    string riga;

    //leggo prima riga
    getline(file, riga);
    //leggo la seconda
    getline(file,riga);
    istringstream convertElemento(riga);
    unsigned int n_fratture;
    convertElemento >> n_fratture;

    list<string> listaRighe;
    fract.SpazioMemoria = listaRighe.size();
    fract.IdFratture.reserve(fract.SpazioMemoria);
    fract.Vertici.reserve(fract.SpazioMemoria);
    fract.NumeroVertici.reserve(fract.SpazioMemoria);


    while(!file.eof()){
        //leggo le righe in gruppi di 6
        for(unsigned int i = 0; i<6; i++){
            getline(file, riga);

            getline(file, riga);
            istringstream converter1(riga);
            char separatore;
            unsigned int id_fratture, n_vertici;
            converter1 >> id_fratture >> separatore >> n_vertici;

            getline(file, riga); //# Vertices
            VectorXi<Vector3d> vertici;

            // Coordinata x
            getline(file, riga);
            vertici.resize(n_vertici);
            istringstream converter2(riga);
            converter2 >> vertici(0)(0);    // con questo vorrei prendesse il primo elemento della riga delle x e lo mettesse nel primo vertice
            for(unsigned int i = 1; i < n_vertici; i++)
                converter2 >> separatore >> vertici(i)(0); // con questo ciclo dovrebbe mettere le x ai successivi vertici

            // Coordinata y
            getline(file, riga);
            istringstream converter3(riga);
            converter3 >> vertici(0)(1);
            for(unsigned int i = 1; i < n_vertici; i++)
                converter3 >> separatore >> vertici(i)(1);

            // Coordinata z
            getline(file, riga);
            istringstream converter4(riga);
            converter4 >> vertici(0)(2);
            for(unsigned int i = 1; i < n_vertici; i++)
                converter4 >> separatore >> vertici(i)(2);



            fract.IdFratture.push_back(id_fratture);
            fract.NumeroVertici.push_back(n_vertici);
            fract.Vertici.push_back(vertici);

        }

    }













/*
    while(getline(file,riga)){
        listaRighe.push_back(riga);
    }
    file.close();
    unsigned int NumeroRighe = listaRighe.size();
    if (NumeroRighe == 0)
    {
        cerr << "Il file è vuoto." << endl;
        return false;
    }

    for (const string& riga : listaRighe)
    {
        istringstream converter(riga);
        char cancelletto;
        unsigned int id, n_vertici, n_fratture;
        Vector3d coordinate;

        if(riga[0] == '#'){
            break;
        }
*/


}


























}
