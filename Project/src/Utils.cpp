#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <limits>
using namespace std;

namespace FractureLibrary{

bool ImportaDati(const string& NomeFile, FractureStruct& fract)
{
    ifstream file;
    file.open(NomeFile);
    if(file.fail()){
        cerr << "impossibile aprire il file"<<endl;
        return false;
    }
    string riga;


    getline(file, riga);   //leggo la prima riga e la scarto
    getline(file,riga);    //leggo la seconda da cui prendo il numero delle fratture
    istringstream convertElemento(riga);
    unsigned int n_fratture;
    convertElemento >> n_fratture;


    fract.NumeroFratture = n_fratture;
    fract.IdFratture.resize(fract.NumeroFratture);
    fract.CoordinateVertici.resize(fract.NumeroFratture);
    fract.NumeroVertici.resize(fract.NumeroFratture);
    fract.IndiciVertici.resize(fract.NumeroFratture);


    unsigned int indice = 0; // Assegno un indice a ogni vertice: contatore di vertici per tutto il file.

    // Leggo le righe in gruppi di 6 poichè a ogni frattura sono dedidicate 6 righe
    /// ogni frattura ha 6 righe quindi all'interno del for devono esserci 6 getline.
    /// per ogni frattura ripeto le 6 getline
    for(unsigned int i = 0; i< n_fratture; i++)
    {
        getline(file, riga);  //NUMERO 1 Salto la prima riga di istruzioni

        // Leggo l'id della frattura e il numero dei vertici
        getline(file, riga); //NUMERO 2
        istringstream converter1(riga);
        char separatore;
        unsigned int id_fratture, n_vertici;
        converter1 >> id_fratture >> separatore >> n_vertici;
        getline(file, riga); //NUMERO 3 Salto la riga

        /// Ora riempio l'oggetto IndiciVertici della struct fract
        // IndiciVertici è un vector<vector<unsigned int>> quindi per
        // ogni frattura devo costruire i rispettivi vector<unsigend int> più interni.
        // Creo il vector di lunghezza indefinita per salvare gli indici dei vertici della frattura
        // Gli cambio la dimensione in base al numero dei vertici ha la frattura.

        vector<unsigned int> indici;
        indici.resize(n_vertici);
        //Ciclo per incrementare il contatore 'indice' tante volte quante sono i vertici della frattura
        for (unsigned int in = 0; in < n_vertici; in++)
        {
          indici[in] = indice;  // Metto indice nel vector indici che contine i vertici della frattura
          indice ++;
        }

        fract.IndiciVertici[i] = indici;
        fract.IdFratture[i] = id_fratture;
        fract.NumeroVertici[i] = n_vertici;


        /// Ora riempio l'oggetto CoordinateVertici è un vector<MatrixXd>
        // initializzo e riempio la mia MatrixXd e poi l'inserisco in coda al vector
        MatrixXd matrice(3,n_vertici);

        getline(file, riga); //NUMERO 4
        istringstream riga_x(riga);

        getline(file, riga);//NUMERO 5
        istringstream riga_y(riga);

        getline(file, riga); //NUMERO 6
        istringstream riga_z(riga);

        double coordinatex, coordinatey, coordinatez;
        char c;

        // travaso il contenuto delle tre righe che ho letto, prima le metto in veriabili coordinatex ,y, z
        // poi inserisco le variabili nei vettori che ho costruito
        for (unsigned int k = 0; k < n_vertici; k++)
        {
            riga_x >> coordinatex >> c;
            riga_y >> coordinatey >> c;
            riga_z >> coordinatez >> c;

            matrice(0,k) = coordinatex;
            matrice(1,k) = coordinatey;
            matrice(2,k) = coordinatez;
        }

        // aggiungo la MatrixXd al vector CoordinateVertici
        fract.CoordinateVertici[i] = matrice;


    }

    file.close();

    return true;
}


} //namespace



/*
 *  vecchia versione
 *
        vector<double> coordinate_x, coordinate_y, coordinate_z;
        coordinate_x.reserve(n_vertici);
        coordinate_y.reserve(n_vertici);
        coordinate_z.reserve(n_vertici);

        getline(file, riga); //NUMERO 4
        istringstream riga_x(riga);

        getline(file, riga);//NUMERO 5
        istringstream riga_y(riga);

        getline(file, riga); //NUMERO 6
        istringstream riga_z(riga);

        double coordinatex, coordinatey, coordinatez;
        char c;


        // travaso il contenuto delle tre righe che ho letto, prima le metto in veriabili coordinatex ,y, z
        // poi inserisco le variabili nei vettori che ho costruito
        for (unsigned int k = 0; k < n_vertici; k++)
        {
            riga_x >> coordinatex >> c;
            riga_y >> coordinatey >> c;
            riga_z >> coordinatez >> c;

            coordinate_x.push_back(coordinatex);
            coordinate_y.push_back(coordinatey);
            coordinate_z.push_back(coordinatez);
        }


        // ora unisco i tre vettori coordinate_x coordinate_y coordinate_z prendendo un elemento per ognuno
        // una volta che ho il vettore coordinate_vertice lo aggiungo alla mia 'tabella' CoordinateVertici
        // ripeto per ogni vertice
        for (unsigned int j = 0; j < n_vertici; j++)
        {
            Vector3d coordinate_vertice;
            coordinate_vertice << coordinate_x[j], coordinate_y[j], coordinate_z[j];
            fract.CoordinateVertici.push_back(coordinate_vertice);
        }



*/

















