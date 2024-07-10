#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <limits>
#include <iomanip>
using namespace std;

//#define TEST
namespace FractureLibrary{

bool ImportaDati(const string& NomeFile, FractureStruct& fract)
{
    ifstream file;
    file.open(NomeFile);
    if(file.fail()){
        cerr << "ImportaDati: impossibile aprire il file"<<endl;
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
    fract.NumeroTracceN.resize(fract.NumeroFratture); //aggiunto il 06/06/2024
    fract.NumeroTracceP.resize(fract.NumeroFratture); //aggiunto il 06/06/2024
    fract.NormaleFrattura.resize(fract.NumeroFratture); // aggiunto il 05/07/2024 per ottimizzare parte2


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

Vector4d PianoPassantePerFrattura(FractureStruct& fract, const unsigned int& n) // test n < numero fratture
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
    piano[0] = t[0];
    piano[1] = t[1];
    piano[2] = t[2];
    piano[3] = d;

    fract.NormaleFrattura[n] = t;

    return piano;
}

//****************************************************************

// REtta intersezione chiama PianoPassantePErFRattura due volte
// Calcolo la retta di intersezione tra piani dopo aver controllato che non sono paralleli
Matrix<double,2,3> RettaIntersezione(Vector4d& piano1, Vector4d& piano2) // [coda; testa]
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
    rettaIntersezione.row(0) = - puntoApplicazione; ///questo è il meno che aggiusta i segni deve essere SOLO qui
    rettaIntersezione.row(1) = tangente;

    return rettaIntersezione;
}

//****************************************************************

//VERIFICARE GLI INPUT IN REFERENZA
/// CheckTraccia
// controlla se la retta passa per la frattura che giace nel piano

bool CheckTraccia(FractureStruct& fract, TracesStruct& trac,
                  const MatrixXd& rettaIntersezione, const unsigned int& n1, const unsigned int& n2, const double& tol)

{

    //RIVEDERE DIREZIONE
    Vector3d direzioneRetta = rettaIntersezione.row(1);
    Vector3d app = rettaIntersezione.row(0);
    vector<double> ts; // ts sono i parametri di intersezione tra la retta e le fratture


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
        Vector3d direzioneLato = vertice1 - vertice0;

        //parametro lato
        if((direzioneLato.cross(direzioneRetta)).norm() < tol && ((vertice0-app).cross(direzioneRetta)).norm() < tol) //  caso paralleli e coincidenti
        {
            // sono coincidenti
            ts.push_back((vertice0-app).norm());
            ts.push_back((vertice1-app).norm());
            cout << "CheckTraccia: Sono paralleli. lato: "<<i1<< "frattura: "<<n1<< endl;

        }
        else if((direzioneLato.cross(direzioneRetta)).norm() > tol)
        {
            double k = ((app.cross(direzioneRetta)-vertice0.cross(direzioneRetta)).dot((direzioneLato.cross(direzioneRetta))))/(((direzioneLato.cross(direzioneRetta)).norm())*(direzioneLato.cross(direzioneRetta)).norm());

            if(k >tol && k<=1+tol)
            {
                //parametro retta
                double t = ((vertice0.cross(direzioneLato)-app.cross(direzioneLato)).dot((direzioneRetta.cross(direzioneLato))))/(((direzioneRetta.cross(direzioneLato)).norm())*(direzioneRetta.cross(direzioneLato)).norm());
                ts.push_back(t);

                cout << "CheckTraccia: intersezione: " << i1 <<" frattura: " << n1 << endl;
            }

#ifdef TEST
            double t = ((vertice0.cross(direzioneLato)-app.cross(direzioneLato)).dot((direzioneRetta.cross(direzioneLato))))/(((direzioneRetta.cross(direzioneLato)).norm())*(direzioneRetta.cross(direzioneLato)).norm());
            Vector3d punto = (app+t*direzioneRetta);

            Matrix<double,2,3> BBox = ComputeBoundingBox(fract,n1);
            Vector3d vettoreMax = BBox.row(0);
            Vector3d vettoreMin = BBox.row(1);

            if(vettoreMin[0]-tol<= punto[0] && punto[0] <= vettoreMax[0]+tol &&
                vettoreMin[1]-tol<= punto[1] && punto[1] <= vettoreMax[1]+tol &&
                vettoreMin[2]-tol<= punto[2] && punto[2] <= vettoreMax[2]+tol)
            {
                ts.push_back(t);
            }

#endif

        }
        j1 ++;
    }
    unsigned int j2 = 0;
    unsigned int i2;
    unsigned int p2;
    while(j2<fract.NumeroVertici[n2])
    {

        i2 = j2%(fract.NumeroVertici[n2]);
        p2 = (j2+1)%(fract.NumeroVertici[n2]);

        Vector3d vertice0 = fract.CoordinateVertici[n2].col(i2);
        Vector3d vertice1 = fract.CoordinateVertici[n2].col(p2);
        Vector3d direzioneLato = vertice1 - vertice0;
        //parametro lato
        if((direzioneLato.cross(direzioneRetta)).norm() < tol && ((vertice0-app).cross(direzioneRetta)).norm() < tol) //  caso paralleli e coincidenti
        {
            // sono coincidenti
            ts.push_back((vertice0-app).norm());
            ts.push_back((vertice1-app).norm());
            cout << "CheckTraccia: Sono paralleli. lato: "<<i1<< "frattura: "<<n1<< endl;

        }
        else if((direzioneLato.cross(direzioneRetta)).norm() > tol)
        { //no abs perchè la norma è positiva //da ottimizzare
            /*    double t = ((vertice0.cross(direzioneLato)-app.cross(direzioneLato)).dot((direzioneRetta.cross(direzioneLato))))/(((direzioneRetta.cross(direzioneLato)).norm())*(direzioneRetta.cross(direzioneLato)).norm());
            Vector3d punto = (app+t*direzioneRetta);

            Matrix<double,2,3> BBox = ComputeBoundingBox(fract,n1);
            Vector3d vettoreMax = BBox.row(0);
            Vector3d vettoreMin = BBox.row(1);

            if(vettoreMin[0]-tol<= punto[0] && punto[0] <= vettoreMax[0]+tol &&
                vettoreMin[1]-tol<= punto[1] && punto[1] <= vettoreMax[1]+tol &&
                vettoreMin[2]-tol<= punto[2] && punto[2] <= vettoreMax[2]+tol)
            {
                ts.push_back(t);
            }
*/
            double k = ((app.cross(direzioneRetta)-vertice0.cross(direzioneRetta)).dot((direzioneLato.cross(direzioneRetta))))/(((direzioneLato.cross(direzioneRetta)).norm())*(direzioneLato.cross(direzioneRetta)).norm());

            if(k >tol && k<=1+tol)
            {
                //parametro retta
                double t = ((vertice0.cross(direzioneLato)-app.cross(direzioneLato)).dot((direzioneRetta.cross(direzioneLato))))/(((direzioneRetta.cross(direzioneLato)).norm())*(direzioneRetta.cross(direzioneLato)).norm());
                ts.push_back(t);

                cout << "CheckTraccia: intersezione: " << i1 <<" frattura: " << n1 << endl;
            }

        }
        j2 ++;
    }

    bool intersezione = false;
    bool pass1;
    bool pass2;
    if(ts.size()<4)
    {
        return intersezione;
    }
    else
    {
        if(max(ts[0],ts[1])  <= min(ts[2],ts[3]) + tol  ||
           max(ts[2],ts[3])  <= min(ts[0],ts[1]) + tol ) // caso disgiunti o vertici intermedi coincidenti
        {
            return intersezione;
        }
        else
        {
            if((abs(ts[0]-ts[2])<tol && abs(ts[1]-ts[3])<tol) ||
                (abs(ts[0]-ts[3])<tol && abs(ts[1]-ts[2])<tol)) //caso esattamente coincidenti
            {
                pass1 = true;
                pass2 = true;
            }
            else if((min(ts[2],ts[3]) >= min(ts[1],ts[0]) + tol && max(ts[2],ts[3]) <= max(ts[0],ts[1]))+ tol) // passante per la seconda frattura
            {
                pass1 = false;
                pass2 = true;

            }
            else if((min(ts[0],ts[1]) >= min(ts[2],ts[3]) + tol && max(ts[0],ts[1]) <= max(ts[2],ts[3]))+ tol) // passante per la prima frattura
            {
                pass1 = true;
                pass2 = false;
            }
            else
            {
                pass1 = false;
                pass2 = false;
            }

            //ordino il vettore
            sort(ts.begin(),ts.end()); // costo computazionale o(nlogn)
            intersezione = true;
            //passo i valori centrali che rappresentano gli estremi della traccia


            unsigned int num = trac.ct;
            trac.IdTracce.push_back(num);
            Vector3d p1 = rettaIntersezione.row(0) + rettaIntersezione.row(1)*ts[1];
            Vector3d p2 = rettaIntersezione.row(0) + rettaIntersezione.row(1)*ts[2];
            Matrix<double,2,3> M;
            M.row(0)=p1;
            M.row(1)=p2;
            trac.EstremiTracce.push_back(M);
            double len = (p2-p1).norm();
            trac.LunghezzaTracce.push_back(len);
            Matrix<unsigned int,2,2> Traccia;
            unsigned int passante1 = 0;
            unsigned int passante2 = 0;
            if (pass1 == false)
            {
                passante1 = 1;
                ordineDecrescente(trac, fract.NumeroTracceN[n1], num);
            }
            else
            {
                ordineDecrescente(trac, fract.NumeroTracceP[n1], num);
            }
            if (pass2 == false)
            {
                passante2 = 1;
                ordineDecrescente(trac, fract.NumeroTracceN[n2], num);
            }
            else
            {
                ordineDecrescente(trac, fract.NumeroTracceP[n2], num);
            }
            Vector<unsigned int,2> riga1(n1,passante1);
            Vector<unsigned int,2> riga2(n2,passante2);
            Traccia.row(0) = riga1;
            Traccia.row(1) = riga2;
            trac.PNP.push_back(Traccia);

            trac.ct ++;
        }
    }


    return intersezione;
}

//***************************************************************

void ordineDecrescente(TracesStruct& trac, list<unsigned int>& lista, const unsigned int& num)
{

    double length = trac.LunghezzaTracce[num];

    auto itor = lista.begin();
    while( length < trac.LunghezzaTracce[*itor] && itor != lista.end())
    { 
        itor ++;

    }
    lista.insert(itor,num);

    if (lista.empty())
    {
        lista.push_back(num);

    }

}

//****************************************************************
//SCRIVERE LA FUNZIONE DOVE VIENE CHIAMATA PER EVITARE DI CHIAMARLA
bool pianiParalleli(Vector4d& piano1, Vector4d& piano2, const double& tol)
{

    double dotProduct = piano1[0]*piano2[0]+piano1[1]*piano2[1]+piano1[2]*piano2[2];

    if(abs(dotProduct)>tol){   ///???DISUGUAGLIAnza giusta???
        return true; //sono paralleli
    }
    return false;

}

//****************************************************************

bool checkIntersezione( FractureStruct& fract, TracesStruct& trac, const unsigned int& n1, const unsigned int& n2, const double& tol)
{


    Vector4d piano1 = PianoPassantePerFrattura(fract, n1);
    Vector4d piano2 = PianoPassantePerFrattura(fract, n2);
/*
    if(pianiParalleli(piano1,piano2,tol) == true)
    {
        cout << "CheckIntersezione: I piani sono paralleli" << endl;
        return false; //non c'è intersezione
    }

    else
    {

*/
        if(IntersezioneBoundingBox(fract,n1,n2) == false)
        {
            cout << "Boundingbox: i poligono non si toccano" << endl;
            return false;
        }
        else
        {
            Matrix<double,2,3> rettaIntersezione = RettaIntersezione(piano1,piano2);
            if(CheckTraccia(fract, trac, rettaIntersezione,n1,n2,tol)==false) //modificare una volta che si è modificata checktraccia
            {
                return false;
            }
            else
            {
                return true;
            }

        }
    //}

    //chiamare funzione che salva le info sulle tracce


}

//****************************************************************
Matrix<double,2,3> ComputeBoundingBox(const FractureStruct& fract,  unsigned int n)
{
    Vector3d vettoreMax(numeric_limits<double>::min(), numeric_limits<double>::min(), numeric_limits<double>::min());
    Vector3d vettoreMin(numeric_limits<double>::max(), numeric_limits<double>::max(), numeric_limits<double>::max());
    for(unsigned int i=0;i<fract.NumeroVertici[n];i++)
    {
        Vector3d vertice = fract.CoordinateVertici[n].col(i);
        double x = vertice[0];
        double y = vertice[1];
        double z = vertice[2];
        //check massimi
        if(vettoreMax[0]<x)
            vettoreMax[0] = x;
        if(vettoreMax[1]<y)
            vettoreMax[1] = y;
        if(vettoreMax[2]<z)
            vettoreMax[2] = z;
        //check minimi
        if(vettoreMin[0]>x)
            vettoreMin[0] = x;
        if(vettoreMin[1]>y)
            vettoreMin[1] = y;
        if(vettoreMin[2]>z)
            vettoreMin[2] = z;
    }

    Matrix<double,2,3> BBox;
    BBox.row(0) = vettoreMax;
    BBox.row(1) = vettoreMin;
    return BBox;

}

//****************************************************************

bool IntersezioneBoundingBox(const FractureStruct& fract, const unsigned int& n1, const unsigned int& n2) // METTERE LA TOLLERANZA
{
    Matrix<double,2,3> BBox1 = ComputeBoundingBox(fract,n1);
    Matrix<double,2,3> BBox2 = ComputeBoundingBox(fract,n2);


    Vector3d vettoreMax1 = BBox1.row(0);
    Vector3d vettoreMin1 = BBox1.row(1);
    Vector3d vettoreMax2 = BBox2.row(0);
    Vector3d vettoreMin2 = BBox2.row(1);

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

bool OutputTraces(const TracesStruct& trac)
{
    // Open File
    ofstream file;
    string outputFileName = "./OutputTraces.txt";
    file.open(outputFileName);

    if (file.fail())
    {
        cerr<< "file open failed"<< endl;
        return false;
    }

    unsigned int num = trac.ct;
    file << "# Number of Traces" << endl;
    file << num << endl; //attENZIONE HO AGGIUNTO +1
    file << "# TraceId; FractureId1; FractureId2; X1; Y1; Z1; X2; Y2; Z2" << endl;
    string sep = "; ";
    for (unsigned int i=0; i < num; i++)
    {
        file << i << sep << trac.PNP[i].row(0)[0]
             << sep << trac.PNP[i].row(1)[0]
             << sep << fixed << setprecision(16) << trac.EstremiTracce[i].row(0)[0]
             << sep << fixed << setprecision(16) << trac.EstremiTracce[i].row(0)[1]
             << sep << fixed << setprecision(16) << trac.EstremiTracce[i].row(0)[2]
             << sep << fixed << setprecision(16) << trac.EstremiTracce[i].row(1)[0]
             << sep << fixed << setprecision(16) << trac.EstremiTracce[i].row(1)[1]
             << sep << fixed << setprecision(16) << trac.EstremiTracce[i].row(1)[2]
             << endl;
    }

    // Close File
    file.close();


    return true;
}

//****************************************************************

bool OutputFractures(const TracesStruct& trac, const FractureStruct& frac)
{
    unsigned int num_fratture = frac.NumeroFratture;
    string sep = ";\t";
    ofstream file;
    string outputFileName = "./OutputFractures.txt";
    file.open(outputFileName);

    if (file.fail())
    {
        cerr<< "file open failed"<< endl;
        return false;
    }

    for (unsigned int i=0; i < num_fratture; i++)
    {
        // Tracce passanti
        if (!frac.NumeroTracceP[i].empty() || !frac.NumeroTracceN[i].empty())
        {
            file << "# FractureId; NumTraces" << endl;
            file << i << sep << frac.NumeroTracceP[i].size() + frac.NumeroTracceN[i].size() << endl;

            file <<  "# TraceId; Tips; Length" << endl;
            for (auto itor = frac.NumeroTracceP[i].begin() ; itor != frac.NumeroTracceP[i].end(); itor++)
            {
                unsigned int id_traccia = (*itor);
                file << id_traccia << sep << false << sep << fixed << setprecision(16) << trac.LunghezzaTracce[id_traccia] << endl;
            }

            for (auto itor = frac.NumeroTracceN[i].begin() ; itor != frac.NumeroTracceN[i].end(); itor++)
            {
                unsigned int id_traccia = (*itor);
                file << id_traccia << sep << true << sep << fixed << setprecision(16) << trac.LunghezzaTracce[id_traccia] << endl;
            }
            file << endl;
        }

    }
    file.close();

    return true;
}

//****************************************************************

bool Output(const TracesStruct& trac, const FractureStruct& frac)
{
    bool stampaTracce = false;
    bool stampaFratture = false;

    if(OutputTraces(trac) == true)
    {
        stampaTracce = true;
        cout << "Tracce stampate correttamente" << endl;
    }
    else
    {
        cerr << "Impossibile stampare le tracce" << endl;
    }

    if(OutputFractures(trac,frac) == true)
    {
        stampaFratture = true;
        cout << "Fratture stampate correttamente" << endl;
    }
    else
    {
        cerr << "Impossibile stampare le fratture" << endl;
    }

    bool result = stampaTracce && stampaFratture;
    return result;
}

//****************************************************************

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////  PARTE 2 /////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//****************************************************************

bool subPolygons(list<Vector3d> verticiPolygons, const vector<Matrix<double,2,3>>& coordEstremiTracce, const Vector3d& normale, const double& tol)
{
    list<Vector3d> destra = {}; //modificare unsigned int oppure solo int
    list<Vector3d> sinistra = {};
    Vector3d traccia0 = coordEstremiTracce[0].row(0);  //seleziono la matrice PRIMA S nel vettore coordEstremiTracce (che le ha tutte) e poi estraggo le cordinate da wuella matrice
    Vector3d traccia1 = coordEstremiTracce[0].row(1);
    Vector3d dirTraccia = traccia1 - traccia0;

    /// Taglio la frattura separando i vertici rispetto alla prima traccia
    // algoritmo taglio frattura in due subpolygons e ottengo le due liste destra e sinistra
    // per poter richiamare subpolygons bisogna trasformare le liste in due matrixXd (redmi*)
    unsigned int e = 0; // per ciclare sugli archi
    const unsigned int num = verticiPolygons.size();

    while (e < num )
    {
        // vertici da analizzare
        Vector3d verticeA = verticiPolygons.front();
        verticiPolygons.push_back(verticeA);
        verticiPolygons.pop_front();
        Vector3d verticeB = verticiPolygons.front();


        ///CASO ESTREMI TRACCIA
        // se uno dei due estremi della traccia appartiene al lato e lo salvo direttamente in entrambe le liste
        // bisogna verificare se uno dei due estremi appartiene al segmento che sto considerando, in quel caso lo salvo nella lista
        Vector3d u = verticeB - verticeA;
        Vector3d v0 = (traccia0 - verticeA);
        if(((u.cross(v0)).array().abs() <= tol).all())
        {
            sinistra.push_back(traccia0);
            destra.push_back(traccia0);
        }

        Vector3d v1 = (traccia1 - verticeA);
        if(((u.cross(v1)).array().abs() <= tol).all())
        {
            sinistra.push_back(traccia1);
            destra.push_back(traccia1);
        }

        ///VERTICI POLIGONO
        // se il prodotto scalare è positivo sta nella lista di destra altrimenti nella sinistra
        // dobbiamo distinguere i tre casi se è  > tol , < -tol, o compreso fra le due tol (allora è zero)
        Vector3d crossProduct = dirTraccia.cross((verticeB - traccia0));
        double segno = crossProduct.dot(normale);
        if(segno > tol)
        {
            destra.push_back(verticeB);
        }
        else if(segno < -tol)
        {
            sinistra.push_back(verticeB);
        }
        else if (abs(segno)< tol)
        {
            cerr<< "problema nel prodotto misto";
            return false;
        }
        else
        {
            cerr<< "problema casistica non conteplata prodotto misto";
            return false;
        }


        e++;
    }

    /// Distinguo le tracce della lista rispetto a quella di riferimento
    // bisogna distingue le tracce a dx o sx o secanti (da ritagliare) rispetto alla principale
    // ho un vettore vector<Matrix<double, 2, 3>> da dividere in due vettori dx e sx
    // (redmi **)

    vector<Matrix<double,2,3>> tdestra = {};
    vector<Matrix<double,2,3>> tsinistra = {};
    for (unsigned int i = 1; i < coordEstremiTracce.size(); i++)
    {
        //prendo i due estremi della traccia da analizzare
        Vector3d A = coordEstremiTracce[i].row(0);
        Vector3d B = coordEstremiTracce[i].row(1);

        Vector3d crossProductB = dirTraccia.cross((B - traccia0));
        double segnoB = crossProductB.dot(normale);

        Vector3d crossProductA = dirTraccia.cross((A - traccia0));
        double segnoA = crossProductA.dot(normale);


        if(segnoA && segnoB > tol) // entrambi i vertici stanno a destra della retta di riferimento
        {
            tdestra.push_back(coordEstremiTracce[i]);
        }
        else if(segnoA && segnoB < -tol)
        {
            tsinistra.push_back(coordEstremiTracce[i]);
        }
        else if (segnoA < -tol && segnoB > tol)
        {
            Matrix3d Matrice;
            Vector3d zeri(0,0,0);
            Matrice.col(0) = dirTraccia;
            Matrice.row(1) = (B-A);
            Matrice.row(2) = zeri;
            Vector3d b = (B-traccia0);
            Vector3d coeff = Matrice.fullPivLu().solve(b);
            Vector3d punto = traccia0 + coeff[0]*dirTraccia;
            Vector3d checkPunto = A + coeff[1]*(B-A);

            //condizione su punto e checkpunto

            Matrix<double,2,3> Tdx = {};
            Tdx.row(0) = B;
            Tdx.row(1) = punto;

            Matrix<double,2,3> Tsx = {};
            Tsx.row(0) = A;
            Tsx.row(1) = punto;

            tsinistra.push_back(Tsx);
            tdestra.push_back(Tdx);
        }
        else if (segnoA > tol && segnoB < -tol)
        {
            Matrix3d Matrice;
            Vector3d zeri(0,0,0);
            Matrice.col(0) = dirTraccia;
            Matrice.row(1) = (B-A);
            Matrice.row(2) = zeri;
            Vector3d b = (B-traccia0);
            Vector3d coeff = Matrice.fullPivLu().solve(b);
            Vector3d punto = traccia0 + coeff[0]*dirTraccia;
            Vector3d checkPunto = A + coeff[1]*(B-A);

            //condizione su punto e checkpunto

            Matrix<double,2,3> Tdx = {};
            Tdx.row(0) = A;
            Tdx.row(1) = punto;

            Matrix<double,2,3> Tsx = {};
            Tsx.row(0) = B;
            Tsx.row(1) = punto;

            tsinistra.push_back(Tsx);
            tdestra.push_back(Tdx);
        }
        else if (abs(segnoA) || abs(segnoB) < tol)
        {
            cerr<< "problema nel prodotto misto";
            return false;
        }
        else
        {
            cerr<< "problema casistica non conteplata prodotto misto";
            return false;
        }


    }






    return true; // se tutto è andato bene
}





} //namespace




/*
    while (e < verticiPolygons.cols())
    {
        v1 = e%(verticiPolygons.cols());
        v2 = (e+1)%(verticiPolygons.cols());

        //estrapolo le coordinate dei rispettivi vertici
        Vector3d verticeA = verticiPolygons.col(v1);
        Vector3d verticeB = verticiPolygons.col(v2);

        ///CASO ESTREMI TRACCIA
        // se uno dei due estremi della traccia appartiene al lato e lo salvo direttamente in entrambe le liste
        // bisogna verificare se uno dei due estremi appartiene al segmento che sto considerando, in quel caso lo salvo nella lista
        Vector3d u = verticeB - verticeA;
        Vector3d v0 = (traccia0 - verticeA);
        if(((u.cross(v0)).array().abs() <= tol).all())
        {
            sinistra.push_back(traccia0);
            destra.push_back(traccia0);
        }

        Vector3d v1 = (traccia1 - verticeA);
        if(((u.cross(v1)).array().abs() <= tol).all())
        {
            sinistra.push_back(traccia1);
            destra.push_back(traccia1);
        }

        ///VERTICI POLIGONO
        // se il prodotto scalare è positivo sta nella lista di destra altrimenti nella sinistra
        // dobbiamo distinguere i tre casi se è  > tol , < -tol, o compreso fra le due tol (allora è zero)
        Vector3d crossProduct = dirTraccia.cross((verticeB - traccia0));
        double segno = crossProduct.dot(normale);
        if(segno > tol)
        {
            destra.push_back(verticeB);
        }
        else if(segno < -tol)
        {
            sinistra.push_back(verticeB);
        }
        else if (abs(segno)< tol)
        {
            cerr<< "problema nel prodotto misto";
            return false;
        }
        else
        {
            cerr<< "problema casistica non conteplata prodotto misto";
            return false;
        }

        e++;
    }

*/
