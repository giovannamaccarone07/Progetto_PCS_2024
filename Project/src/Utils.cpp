#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <limits>
using namespace std;

namespace FractureLibrary{

bool ImportDati(const string& NomeFile, FractureStruct& fract)
{
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
            VectorXi vertici;
            VectorXd coordinate_x, coordinate_y, coordinate_z;
            vertici.resize(n_vertici);
            // Utilizza stringstream per analizzare la riga corrente
            istringstream riga_x(riga);
            double coordinate;
            while (riga_x >> coordinate) {
                coordinate_x.conservativeResize(coordinate_x.size() + 1);
                coordinate_x(coordinate_x.size() - 1) = coordinate;
            }

            // Leggi la prossima riga (coordinate y)
            getline(file, riga);
            istringstream riga_y(riga);
            while (riga_y >> coordinate) {
                coordinate_y.conservativeResize(coordinate_y.size() + 1);
                coordinate_y(coordinate_y.size() - 1) = coordinate;
            }

            // Leggi la prossima riga (coordinate z)
            getline(file, riga);
            istringstream riga_z(riga);
            while (riga_z >> coordinate) {
                coordinate_z.conservativeResize(coordinate_z.size() + 1);
                coordinate_z(coordinate_z.size() - 1) = coordinate;
            }
            // Verifica che il numero di coordinate sia lo stesso per x, y e z
            if (coordinate_x.size() != coordinate_y.size() || coordinate_x.size() != coordinate_z.size()) {
                cerr << "Errore: numero diverso di coordinate x, y e z." << endl;
                return 1;
            }

            // Costruisci i vettori di vertici utilizzando le coordinate
            for (int i = 0; i < coordinate_x.size(); ++i) {
                Vector3d vertice;
                vertice << coordinate_x(i), coordinate_y(i), coordinate_z(i);
                fract.Vertici.push_back(vertice);
            }
            fract.IdFratture.push_back(id_fratture);
            fract.NumeroVertici.push_back(n_vertici);
        }
    }
    file.close();

    return true;
}
}

/*
    // Coordinata x
    getline(file, riga);

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
*/























