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

//****************************************************************

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

//****************************************************************

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

//****************************************************************

//VERIFICARE GLI INPUT IN REFERENZA
/// CheckTraccia
// controlla se la retta passa per la frattura che giace nel piano
bool CheckTraccia(const FractureStruct& fract, const MatrixXd& rettaIntersezione,unsigned int& n1, unsigned int& n2)
{

    //RIVEDERE DIREZIONE
    Vector3d direzioneRetta = rettaIntersezione.row(1);
    Vector3d app = rettaIntersezione.row(0);
    vector<double> ts; // ts sono i parametri di intersezione tra la retta e le fratture
    double tol = numeric_limits<double>::epsilon();


    unsigned int j1 = 0;
    unsigned int i1;
    unsigned int p1;
    while(j1<fract.NumeroVertici[n1])
    {
        i1 = j1%(fract.NumeroVertici[n1]);
        p1 = (j1+1)%(fract.NumeroVertici[n1]);
        //calcolo gli estremi di ogni lato per tutti i lati tranne l'ultimo
        //lato i-esimo:
        Vector3d vertice0 = fract.CoordinateVertici[n1].col(i1);
        Vector3d vertice1 = fract.CoordinateVertici[n1].col(p1);
        Vector3d direzioneLato = vertice0 - vertice1;
        //parametro lato
        if((direzioneLato.cross(direzioneRetta)).norm() > tol)
        { //no abs perchè la norma è positiva //da ottimizzare

            double k = ((app.cross(direzioneRetta)-vertice0.cross(direzioneRetta)).dot((direzioneLato.cross(direzioneRetta))))/(((direzioneLato.cross(direzioneRetta)).norm())*(direzioneLato.cross(direzioneRetta)).norm());
            if(k >= -tol && k<=1+tol)
            {
                if(abs(k-1)<tol)
                {
                    cout << "Il lato e la retta sono coincidenti" << endl; //introdurre variabile per dire che i lati sono o meno coincidenti
                }
                else
                {
                    //parametro retta
                    double t = ((vertice0.cross(direzioneLato)-app.cross(direzioneLato)).dot((direzioneRetta.cross(direzioneLato))))/(((direzioneRetta.cross(direzioneLato)).norm())*(direzioneRetta.cross(direzioneLato)).norm());
                    ts.push_back(t);

                }
            cout << "intersezione" << i1 << endl;
            }
        }
        else
        {
            cout << "Sono paralleli" << endl;
        }
    }
    unsigned int j2 = 0;
    unsigned int i2;
    unsigned int p2;
    while(j2<fract.NumeroVertici[n2])
    {
        i2 = j2%(fract.NumeroVertici[n2]);
        p2 = (j2+1)%(fract.NumeroVertici[n2]);
        //calcolo gli estremi di ogni lato per tutti i lati tranne l'ultimo
        //lato i-esimo:
        Vector3d vertice0 = fract.CoordinateVertici[n2].col(i2);
        Vector3d vertice1 = fract.CoordinateVertici[n2].col(p2);
        Vector3d direzioneLato = vertice0 - vertice1;
        //parametro lato
        if((direzioneLato.cross(direzioneRetta)).norm() > tol)
        { //no abs perchè la norma è positiva //da ottimizzare
            double k = ((app.cross(direzioneRetta)-vertice0.cross(direzioneRetta)).dot((direzioneLato.cross(direzioneRetta))))/(((direzioneLato.cross(direzioneRetta)).norm())*(direzioneLato.cross(direzioneRetta)).norm());
            if(k >= -tol && k<=1+tol)
            {
                if(abs(k-1)<tol)
                {
                    cout << "Il lato e la retta sono coincidenti" << endl; //introdurre variabile per dire che i lati sono o meno coincidenti
                }
                else
                {
                    //parametro retta
                    double t = ((vertice0.cross(direzioneLato)-app.cross(direzioneLato)).dot((direzioneRetta.cross(direzioneLato))))/(((direzioneRetta.cross(direzioneLato)).norm())*(direzioneRetta.cross(direzioneLato)).norm());
                    ts.push_back(t);
                }
                cout << "intersezione" << i2 << endl;
            }
        }
        else
        {
            cout << "Sono paralleli" << endl;
        }


    }
    bool intersezione = false;
    if(ts.size()<4)
    {
        return intersezione;
    }
    else
    {
        if((ts[0]<ts[1] && ts[1]<ts[2] && ts[2]<ts[3]) ||
            (ts[1]<ts[0] && ts[0]<ts[2] && ts[2]<ts[3]) ||
            (ts[0]<ts[1] && ts[1]<ts[3] && ts[3]<ts[2]) ||
            (ts[1]<ts[0] && ts[0]<ts[3] && ts[3]<ts[2]) ||
            (ts[2]<ts[3] && ts[3]<ts[0] && ts[0]<ts[1]) ||
            (ts[3]<ts[2] && ts[2]<ts[0] && ts[0]<ts[1]) ||
            (ts[2]<ts[3] && ts[3]<ts[1] && ts[1]<ts[0]) ||
            (ts[3]<ts[2] && ts[2]<ts[1] && ts[1]<ts[0]))
        {
            return intersezione;
        }
        else
        {
            //ordino il vettore
            sort(ts.begin(),ts.end());
            intersezione = true;
            //passo i valori centrali che rappresentano gli estremi della traccia
            //ComputeTrace(trac,fract,ts[1],ts[2],rettaIntersezione,n1,n2,pass1,pass2);

        }
    }
    return intersezione;
}

//****************************************************************

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

//****************************************************************

bool checkIntersezione(const FractureStruct& fract, unsigned int n1, unsigned int n2)
{
    Vector4d piano1 = PianoPassantePerFrattura(fract, n1);
    Vector4d piano2 = PianoPassantePerFrattura(fract, n2);
    if(pianiParalleli(piano1,piano2) == true)
    {
        cout << "I piani sono paralleli" << endl;
        return false; //non c'è intersezione
    }

    else
    {
        if(BoundingBox(fract,n1,n2) == false)
        {
            return false;
        }
        else
        {
            MatrixXd rettaIntersezione = RettaIntersezione(piano1,piano2);
            if(CheckTraccia(fract,rettaIntersezione,n1,n2)==false) //modificare una volta che si è modificata checktraccia
            {
                return false;
            }
            else
            {
                return true;
            }

        }
    }

    //chiamare funzione che salva le info sulle tracce


}

//****************************************************************

bool BoundingBox(const FractureStruct& fract, unsigned int n1, unsigned int n2)
{
    Vector3d vettoreMax1(-1e100, -1e100, -1e100);
    Vector3d vettoreMax2(-1e100, -1e100, -1e100);
    Vector3d vettoreMin1(1e100, 1e100, 1e100);
    Vector3d vettoreMin2(1e100, 1e100, 1e100);
    for(unsigned int i=0;i<fract.NumeroVertici[n1];i++)
    {
        Vector3d vertice = fract.CoordinateVertici[n1].col(i);
        double x = vertice[0];
        double y = vertice[1];
        double z = vertice[2];
        //check massimi
        if(vettoreMax1[0]<x)
            vettoreMax1[0] = x;
        if(vettoreMax1[1]<y)
            vettoreMax1[1] = y;
        if(vettoreMax1[2]<z)
            vettoreMax1[2] = z;
        //check minimi
        if(vettoreMin1[0]>x)
            vettoreMin1[0] = x;
        if(vettoreMin1[1]>y)
            vettoreMin1[1] = y;
        if(vettoreMin1[2]>z)
            vettoreMin1[2] = z;
    }
    for(unsigned int i=0;i<fract.NumeroVertici[n2];i++)
    {
        Vector3d vertice = fract.CoordinateVertici[n2].col(i);
        double x = vertice[0];
        double y = vertice[1];
        double z = vertice[2];
        //check massimi
        if(vettoreMax2[0]<x)
            vettoreMax2[0] = x;
        if(vettoreMax2[1]<y)
            vettoreMax2[1] = y;
        if(vettoreMax2[2]<z)
            vettoreMax2[2] = z;
        //check minimi
        if(vettoreMin2[0]>x)
            vettoreMin2[0] = x;
        if(vettoreMin2[1]>y)
            vettoreMin2[1] = y;
        if(vettoreMin2[2]>z)
            vettoreMin2[2] = z;
    }
    //check intersezione bbox
    if(vettoreMin1[0]<= vettoreMax2[0] && vettoreMax1[0]>= vettoreMin2[0] &&
        vettoreMin1[1]<= vettoreMax2[1] && vettoreMax1[1]>= vettoreMin2[1] &&
        vettoreMin1[2]<= vettoreMax2[2] && vettoreMax1[2]>= vettoreMin2[2])
    {
        return true;
    }
    else
    {
        return false;
    }
}

//****************************************************************
/*
bool ComputeTraces(){}

*/
} //namespace

















