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
    fract.CoordinateVertici.reserve(fract.NumeroFratture * 8); //brutto
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


        // Ciclo per incrementare il contatore 'indice' tante volte quante sono i vertici della frattura
        for (unsigned int in = 0; in < n_vertici; in++)
        {
            indici[in] = indice;  // Metto indice nel vector indici che contine i vertici della frattura
            indice ++;
        }

        fract.IdFratture[i] = id_fratture;
        fract.NumeroVertici[i] = n_vertici;
        fract.IndiciVertici[i] = indici;


        /// Ora riempio l'oggetto CoordinateVertici è un vector<Vector3d>
        // Creo dei vettori con la riga delle x, delle y e delle z per riuscire
        // in un secondo momento a creare vettori con (x,y,z) prendendo un elemento per volta di ogni vettore
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
    }

    file.close();

    return true;
}

Vector4d PianoPassantePerFrattura(const FractureStruct& fract, unsigned int n) // test n < numero fratture
{
    //Prendo i primi tre vertici della frattura n-esima per trovare il piano su cui giace il poligono

    Vector3d vertice1 = fract.CoordinateVertici[fract.IndiciVertici[n][0]];
    Vector3d vertice2 = fract.CoordinateVertici[fract.IndiciVertici[n][1]];
    Vector3d vertice3 = fract.CoordinateVertici[fract.IndiciVertici[n][2]];

    //trovo i vettori direzionali: fisso il vertice1 e faccio n1 = (vertice2 - vertice1) e n2 = (vertice3) - (vertice1)
    Vector3d n1 = vertice2 - vertice1;
    Vector3d n2 = vertice3 - vertice1;

    //prodotto vettoriale t = n1 x n2
    Vector3d t = n1.cross(n2);
    double d = -(t[0]*vertice1[0] + t[1]*vertice1[1] + t[2]*vertice1[2]);
    Vector4d piano;
    piano[0] = t[0];        // questo fa cagare
    piano[1] = t[1];
    piano[2] = t[2];
    piano[3] = d;

    cout << piano[0] << " " << piano[1] << " " << piano[2] << " " << piano[3] << endl;

    return piano;
}


// Calcolo la retta di intersezione tra piani dopo aver controllato che non sono paralleli
MatrixXd RettaIntersezione(const FractureStruct& fract, unsigned int n1, unsigned n2) // [coda; testa]
{
    Vector4d piano1 = PianoPassantePerFrattura(fract, n1);
    Vector4d piano2 = PianoPassantePerFrattura(fract, n2);
    double tol = numeric_limits<double>::epsilon();

    double r1=piano1[0]/piano2[0];
    double r2=piano1[1]/piano2[1];
    double r3=piano1[2]/piano2[2];

    Vector3d p1;
    p1[0] = piano1[0];        // questo fa cagare
    p1[1] = piano1[1];
    p1[2] = piano1[2];

    Vector3d p2;
    p2[0] = piano2[0];        // questo fa cagare
    p2[1] = piano2[1];
    p2[2] = piano2[2];


    if(!(abs(r1-r2)<=tol && abs(r1-r3)<=tol && abs(r2-r3)<=tol))
    {
        Vector3d testa = p1.cross(p2);
        MatrixXd A;
        A.row(0)=p1;
        A.row(1)=p2;
        Vector2d b = {piano1[3], piano2[3]}; //ottimizziamo
        Vector3d coda = A.lu().solve(b);
        MatrixXd rettaIntersezione;
        rettaIntersezione.row(0) = coda;
        rettaIntersezione.row(1) = testa;

        return rettaIntersezione;
    }
    else
    {
        MatrixXd nulla = {};
        return nulla;
    }

}

} //namespace




















