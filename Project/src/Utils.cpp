#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <limits>
using namespace std;

namespace FractureLibrary{
bool ImportaStruttura(const string& NomeFile,
                 FractureStruct& Fract)
{
    if(!ImportaDati(NomeFile + "/FR3_data.txt",      // Non so perchè non apre il file
                        Fract))
    {
        return false;
    }
    /*else
    {
        // Qui dovrebbe esserci una funzione che collega gli indici dei vertici della frattura alle corrispondenti coordinate
    } */
}

bool ImportaDati(const string& NomeFile, FractureStruct& fract)
{
    ifstream file;
    file.open(NomeFile);
    if(file.fail()){
        cerr << "impossibile aprire il file"<<endl;
    }
    string riga;

    //leggo la prima riga
    getline(file, riga);
    //leggo la seconda da cui prendo il numero delle fratture
    getline(file,riga);
    istringstream convertElemento(riga);
    unsigned int n_fratture;
    convertElemento >> n_fratture;
    // Uso il numero delle righe del file per creare spazio nella struttura
    list<string> listaRighe;
    fract.SpazioMemoria = listaRighe.size();
    fract.IdFratture.reserve(fract.SpazioMemoria);
    fract.CoordinateVertici.reserve(fract.SpazioMemoria);
    fract.NumeroVertici.reserve(fract.SpazioMemoria);
    fract.IndiciVertici.reserve(fract.SpazioMemoria);

    // Il while continua fino alla fine del file
    while(!file.eof()){
        // Leggo le righe in gruppi di 6 poichè a ogni frattura sono dedidicate 6 righe
        // Inizializzo una variabile per assagnare un indice a ogni vertice
        unsigned int indice = 0;
        for(unsigned int i = 0; i<6; i++){
            getline(file, riga);  // Salto la riga
            // Leggo l'id della frattura e il numero dei vertici
            getline(file, riga);
            istringstream converter1(riga);
            char separatore;
            unsigned int id_fratture, n_vertici;
            converter1 >> id_fratture >> separatore >> n_vertici;

            getline(file, riga); // Salto la riga

            // Creo un vettore di lunghezza indefinita per salvare gli indici dei vertici della frattura
            VectorXi indici;
            indici.resize(n_vertici);  // Gli cambio la dimensione in base al numero dei vertici che ho letto ha la frattura
            // In questo for partendo da 0 (valore a cui ho inizializzato indice) sommo ad esso il valore di in che va fino al numero di vertici,
            //teoricamente uscendo poi dal for il valore di indice dovrebbe aggiornarsi ma non sono sicura. Così si parte da 0 e si continua ad aumentare indice in modo crescente
            for (int in = 0; in < n_vertici; ++in) {
                indice = indice + in;
                indice << indici[in];  // Metto l'indice nel vettore degli indici dei vertici della frattura
            }

            fract.IdFratture.push_back(id_fratture);     // Metto i valori trovati nel file all'interno della struttura
            fract.NumeroVertici.push_back(n_vertici);
            fract.IndiciVertici.push_back(indici);
            // Creo dei vettori con la riga delle x, delle y e delle z per riuscire in un secondo momento a creare vettori con (x,y,z) prendendo un elemento per volta di ogni vettore
            VectorXd coordinate_x, coordinate_y, coordinate_z;
            getline(file, riga);
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

            for (int j = 0; j < coordinate_x.size(); ++j){

                Vector3d coordinate_vertice;
                coordinate_vertice << coordinate_x(j), coordinate_y(j), coordinate_z(j);
                fract.CoordinateVertici.push_back(coordinate_vertice);
            }
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























