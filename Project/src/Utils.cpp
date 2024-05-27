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

Vector4d PianoPassantePerFrattura(const FractureStruct& fract, unsigned int& n) // test n < numero fratture
{
    //Prendo i primi tre vertici della frattura n-esima per trovare il piano su cui giace il poligono

    Vector3d vertice1 = fract.CoordinateVertici[n].col(0);
    Vector3d vertice2 = fract.CoordinateVertici[n].col(1);
    Vector3d vertice3 = fract.CoordinateVertici[n].col(2);

    //trovo i vettori direzionali: fisso il vertice1 e faccio n1 = (vertice2 - vertice1) e n2 = (vertice3) - (vertice1)
    Vector3d n1 = vertice2 - vertice1;
    Vector3d n2 = vertice3 - vertice1;

    //prodotto vettoriale t = n1 x n2
    Vector3d t = n1.cross(n2);
    double d = -(t[0]*vertice1[0] + t[1]*vertice1[1] + t[2]*vertice1[2]); //d = -(ax+by+cz)
    Vector4d piano;
    piano[0] = t[0];        // questo fa cagare
    piano[1] = t[1];
    piano[2] = t[2];
    piano[3] = d;

    cout << piano[0] << " " << piano[1] << " " << piano[2] << " " << piano[3] << endl;

    return piano;
}

// REtta intersezione chiama PianoPassantePErFRattura due volte
// Calcolo la retta di intersezione tra piani dopo aver controllato che non sono paralleli
MatrixXd RettaIntersezione(Vector4d& piano1, Vector4d& piano2) // [coda; testa]
{
    //vettore normale al primo piano
    Vector3d p1 = piano1.head(3);    // prende le prime 3 componenti
    //vettore normale al secondo piano
    Vector3d p2 = piano2.head(3);
    Vector3d tangente = p1.cross(p2);
    //documento vicini
    Matrix3d A;
    A.row(0) = p1;
    A.row(1) = p2;
    A.row(2) = tangente;
    Vector3d b;
    b[0] = piano1[3];
    b[1] = piano2[3];
    b[2] = 0;
    Vector3d puntoApplicazione = A.fullPivLu().solve(b); //x punto di applicazione
    Matrix<double,2,3> rettaIntersezione;
    rettaIntersezione.row(0) = puntoApplicazione;
    rettaIntersezione.row(1) = tangente;

    return rettaIntersezione;
}

//VERIFICARE GLI INPUT IN REFERENZA
/// CheckTraccia
// controlla se la retta passa per la frattura che giace nel piano
bool CheckTraccia(const FractureStruct& fract, const MatrixXd& rettaIntersezione,unsigned int& n)
{
    //RIVEDERE DIREZIONE
    Vector3d direzioneRetta = (rettaIntersezione.row(0)+rettaIntersezione.row(1));
    Vector3d app = rettaIntersezione.row(1);

    double tol = numeric_limits<double>::epsilon();
    for(unsigned int i=0; i<fract.NumeroVertici[n]-1; i++)
    {
        //calcolo gli estremi di ogni lato per tutti i lati tranne l'ultimo
        //lato i-esimo:
        Vector3d vertice0 = fract.CoordinateVertici[n].col(i);
        Vector3d vertice1 = fract.CoordinateVertici[n].col(i+1);
        Vector3d direzioneLato = vertice1 - vertice0;
        //parametro retta
        //double t = ((vertice1.cross(direzioneLato)-vertice0.cross(direzioneLato)).dot((direzioneLato.cross(direzioneRetta))))/(pow(((vertice0.cross(vertice1)).norm()),2));
        //parametro lato
        double k = ((app.cross(direzioneRetta)-vertice0.cross(direzioneRetta)).dot((direzioneLato.cross(direzioneRetta))))/(pow(((direzioneLato.cross(direzioneRetta)).norm()),2)); //esce segno inverso!!!!!!!!!!
        bool intersezione = false;
        if(abs(k-tol)>=0 && abs(k-tol)<=1){
            intersezione = true;
            return intersezione;
        }
    }
    //l'ultimo lato lo calcolo a mano
    Vector3d verticeFirst = fract.CoordinateVertici[n].col(0);
    Vector3d verticeLast = fract.CoordinateVertici[n].col(fract.NumeroVertici[n]-1);
    Vector3d direzioneLato = verticeLast - verticeFirst;
    double k = ((app.cross(direzioneRetta)-verticeFirst.cross(direzioneRetta)).dot((direzioneLato.cross(direzioneRetta))))/(pow(((direzioneLato.cross(direzioneRetta)).norm()),2));
    bool intersezione = false;
    if(abs(k-tol)>=0 && abs(k-tol)<=1){
        intersezione = true;
    }
    return intersezione;

}

bool pianiParalleli(Vector4d& piano1, Vector4d& piano2)
{
    double tol = numeric_limits<double>::epsilon();
    double dotProduct = piano1[0]*piano2[0]+piano1[1]*piano2[1]+piano1[2]*piano2[2];

    if(abs(dotProduct)>tol){
        return true; //sono paralleli
    }

/*
    double r1=piano1[0]/piano2[0];
    double r2=piano1[1]/piano2[1];
    double r3=piano1[2]/piano2[2];

    if(!(abs(r1-r2)<=tol && abs(r1-r3)<=tol && abs(r2-r3)<=tol))
    {
        return true;
    }*/
    return false;

}

bool checkIntersezione(const FractureStruct& fract, unsigned int n1, unsigned int n2)
{
    Vector4d piano1 = PianoPassantePerFrattura(fract, n1);
    Vector4d piano2 = PianoPassantePerFrattura(fract, n2);
    if(pianiParalleli(piano1,piano2) == true)
    {
        cout << "I piani sono paralleli" << endl;
        return false; //non c'è intersezione
    }
    MatrixXd rettaIntersezione = RettaIntersezione(piano1,piano2);
    if(CheckTraccia(fract,rettaIntersezione,n1)==false && CheckTraccia(fract,rettaIntersezione,n2)==false)
    {
        return false;
    }
    //chiamare funzione che salva le info sulle tracce
    return true;
}



} //namespace

















