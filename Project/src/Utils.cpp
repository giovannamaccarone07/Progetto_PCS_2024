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

bool ImportData(const string& fileName, FractureStruct& fract)
{
    ifstream file;
    file.open(fileName);
    if(file.fail()){
        cerr << "ImportData: Errore durante l'apertura del file."<<endl;
        return false;
    }
    string line;

    //Leggo la PRIMA riga: la scarto.
    getline(file, line);

    //Leggo la SECONDA riga: estraggo il numero delle fratture.
    getline(file,line);
    istringstream elementConverter(line);
    unsigned int n_fractures;
    elementConverter >> n_fractures;

    fract.NumeroFratture = n_fractures;
    fract.IdFratture.resize(fract.NumeroFratture);
    fract.CoordinateVertici.resize(fract.NumeroFratture);
    fract.NumeroVertici.resize(fract.NumeroFratture);
    fract.IndiciVertici.resize(fract.NumeroFratture);
    fract.NumeroTracceN.resize(fract.NumeroFratture);
    fract.NumeroTracceP.resize(fract.NumeroFratture);
    fract.NormaleFrattura.resize(fract.NumeroFratture);

    // Assegno un indice a ogni vertice: contatore di vertici per tutto il file.
    unsigned int index = 0;

    // Leggo le righe in gruppi di 6 poichè a ogni frattura sono dedidicate 6 righe (avrò bisogno di 6 getline):
    for(unsigned int i = 0; i< n_fractures; i++)
    {
        //Leggo la PRIMA riga: la scarto.
        getline(file, line);

        // Leggo la SECONDA riga: estraggo l'id della frattura e il numero di vertici.
        getline(file, line);
        istringstream converter1(line);
        char delimiter;
        unsigned int id_fractures, n_vertices;
        converter1 >> id_fractures >> delimiter >> n_vertices;

        //Leggo la TERZA riga: la scarto.
        getline(file, line);

        /// Riempio l'oggetto IndiciVertici della struct fract
        // IndiciVertici è un vector<vector<unsigned int>> quindi per ogni frattura devo costruire i rispettivi vector<unsigend int> più interni.
        // Creo il vector di lunghezza indefinita per salvare gli indici dei vertici della frattura e successivamente effettuo un resize in base al numero di vertici della frattura

        vector<unsigned int> indices;
        indices.resize(n_vertices);
        //Effettuo un ciclo per incrementare il contatore 'index' tante volte quante sono i vertici della frattura.
        for (unsigned int in = 0; in < n_vertices; in++)
        {
            indices[in] = index;
            index ++;
        }

        fract.IndiciVertici[i] = indices;
        fract.IdFratture[i] = id_fractures;
        fract.NumeroVertici[i] = n_vertices;

        /// Riempio l'oggetto CoordinateVertici  della struct fract
        // CoordinateVertici è un vector<MatrixXd>. Riempio ogni MatrixXd più interna e poi la inserisco in coda al vector.
        MatrixXd matrix(3,n_vertices);

        //Leggo la QUARTA riga: contiene le coordinate x dei vertici.
        getline(file, line);
        istringstream line_x(line);
        //Leggo la QUINTA riga: contiene le coordinate y dei vertici.
        getline(file, line);
        istringstream line_y(line);
        //Leggo la SESTA riga: contiene le coordinate z dei vertici.
        getline(file, line);
        istringstream line_z(line);

        double coordX, coordY, coordZ;
        char c;

        //Inserisco il contenuto delle tre righe che ho appena letto: in un primo momento le assegno alle variabili coordX/Y/Z,
        //successimanete le inserisco nei vettori che ho precedentemente costruito.
        for (unsigned int k = 0; k < n_vertices; k++)
        {
            line_x >> coordX >> c;
            line_y >> coordY >> c;
            line_z >> coordZ >> c;

            matrix(0,k) = coordX;
            matrix(1,k) = coordY;
            matrix(2,k) = coordZ;
        }

        // Inserisco la MatrixXd al vector CoordinateVertici
        fract.CoordinateVertici[i] = matrix;
    }

    file.close();
    return true;
}

//****************************************************************

Vector4d FracturePlane(FractureStruct& fract, const unsigned int& n)
{
    //Considero i primi tre vertici della frattura n-esima per trovare il piano su cui giace la stessa.
    Vector3d vertex1 = fract.CoordinateVertici[n].col(0);
    Vector3d vertex2 = fract.CoordinateVertici[n].col(1);
    Vector3d vertex3 = fract.CoordinateVertici[n].col(2);

    //Calcolo i vettori direzionali fissando il vertice1.
    Vector3d n1 = vertex2 - vertex1;
    Vector3d n2 = vertex3 - vertex1;

    //Calcolo la giacitura del piano tramite il prodotto vettoriale t = n1 x n2.
    Vector3d t = n1.cross(n2);
    //Calcolo il termine noto come d = -(ax+by+cz).
    double d = -(t[0]*vertex1[0] + t[1]*vertex1[1] + t[2]*vertex1[2]);
    Vector4d plane;
    plane[0] = t[0];
    plane[1] = t[1];
    plane[2] = t[2];
    plane[3] = d;

    fract.NormaleFrattura[n] = t;

    return plane;
}

//****************************************************************

Matrix<double,2,3> IntersectionLine(Vector4d& plane1, Vector4d& plane2) // [coda; testa]
{
    //Individuo lle giaciture dei piani considerando i coefficienti a,b,c delle equazioni.
    Vector3d p1 = plane1.head(3);
    Vector3d p2 = plane2.head(3);
    //Calcolo la direzione tangente tramite il prodotto vettoriale delle giaciture
    Vector3d tang = p1.cross(p2);
    //Costruisco il sistema Ax=b.
    Matrix3d A;
    A.row(0) = p1;
    A.row(1) = p2;
    A.row(2) = tang;
    Vector3d b;
    b[0] = plane1[3];
    b[1] = plane2[3];
    b[2] = 0;

    //Risolvo il sistema calcolando il punto di applicazione della retta di intersezione x.
    Vector3d point = A.fullPivLu().solve(b);
    Matrix<double,2,3> intersectionLine;
    intersectionLine.row(0) = -point;
    intersectionLine.row(1) = tang;


    return intersectionLine;
}

//****************************************************************

bool FindTrace(FractureStruct& fract, TracesStruct& trac, const MatrixXd& intersectionLine, const unsigned int& n1, const unsigned int& n2, const double& tol)

{
    //Estraggo e salvo la direzione e il punto di applicazione della retta di intersezione.
    Vector3d lineDir = intersectionLine.row(1);
    Vector3d point = intersectionLine.row(0);

    //ts è il vettore di double che contiene i parametri di intersezione tra retta e fratture
    vector<double> ts;

    //Intersezione tra intersectionLine e frattura n1:
    //Inizializzo gli indici per lavorare in modulo "NumeroVertici".
    unsigned int j1 = 0;
    unsigned int i1;
    unsigned int p1;

    while(j1<fract.NumeroVertici[n1])
    {

        i1 = j1%(fract.NumeroVertici[n1]);
        p1 = (j1+1)%(fract.NumeroVertici[n1]);

        //Calcolo gli estremi di ogni lato e la rispettiva direzione per tutti i lati della frattura.
        //Lato i-esimo:
        Vector3d vertex0 = fract.CoordinateVertici[n1].col(i1);
        Vector3d vertex1 = fract.CoordinateVertici[n1].col(p1);
        Vector3d edgeDir = vertex1 - vertex0;

        //CASO 1: la retta di intersezione e la direzione del lato sono parallele e coincidenti.
        if((edgeDir.cross(lineDir)).norm() < tol && ((vertex0-point).cross(lineDir)).norm() < tol)
        {
            //Salvo in ts gli estremi del lato.
            ts.push_back((vertex0-point).norm());
            ts.push_back((vertex1-point).norm());
        }
        //CASO 2: c'è intersezione tra la retta di intersezione e la direzione del lato.
        else if((edgeDir.cross(lineDir)).norm() > tol)
        {
            //Calcolo il parametro k.
            double k = ((point.cross(lineDir)-vertex0.cross(lineDir)).dot((edgeDir.cross(lineDir))))/(((edgeDir.cross(lineDir)).norm())*(edgeDir.cross(lineDir)).norm());

            //Se k ∈ [0,1] allora c'è intersezione tra la retta e il lato.
            if(k >tol && k<1+tol)
            {
                //Una volta appurato che c'è intersezione, calcolo il parametro t che indica l'esatto punto di intersezione e lo salvo in ts.
                double t = ((vertex0.cross(edgeDir)-point.cross(edgeDir)).dot((lineDir.cross(edgeDir))))/(((lineDir.cross(edgeDir)).norm())*(lineDir.cross(edgeDir)).norm());
                ts.push_back(t);
            }
//E' possibile attivare un ulteriore test per verificare l'intersezione tra le due fratture attraverso il metodo delle bounding box.
#ifdef TEST
            double t = ((vertex0.cross(edgeDir)-app.cross(edgeDir)).dot((lineDir.cross(edgeDir))))/(((lineDir.cross(edgeDir)).norm())*(lineDir.cross(edgeDir)).norm());
            Vector3d punto = (point+t*lineDir);

            Matrix<double,2,3> BBox = ComputeBoundingBox(fract,n1);
            Vector3d maxVec = BBox.row(0);
            Vector3d minVec = BBox.row(1);

            if(minVec[0]-tol< punto[0] && punto[0] < maxVec[0]+tol &&
                minVec[1]-tol< punto[1] && punto[1] < maxVec[1]+tol &&
                minVec[2]-tol< punto[2] && punto[2] < maxVec[2]+tol)
            {
                ts.push_back(t);
            }

#endif
        }
        //Incremento l'indice
        j1 ++;
    }

    //Intersezione tra intersectionLine e frattura n2:
    //Inizializzo gli indici per lavorare in modulo "NumeroVertici".
    unsigned int j2 = 0;
    unsigned int i2;
    unsigned int p2;
    while(j2<fract.NumeroVertici[n2])
    {

        i2 = j2%(fract.NumeroVertici[n2]);
        p2 = (j2+1)%(fract.NumeroVertici[n2]);

        //Calcolo gli estremi di ogni lato e la rispettiva direzione per tutti i lati della frattura.
        //Lato i-esimo:
        Vector3d vertex0 = fract.CoordinateVertici[n2].col(i2);
        Vector3d vertex1 = fract.CoordinateVertici[n2].col(p2);
        Vector3d edgeDir = vertex1 - vertex0;

        //CASO 1: la retta di intersezione e la direzione del lato sono parallele e coincidenti.
        if((edgeDir.cross(lineDir)).norm() < tol && ((vertex0-point).cross(lineDir)).norm() < tol)
        {
            //Salvo in ts gli estremi del lato.
            ts.push_back((vertex0-point).norm());
            ts.push_back((vertex1-point).norm());

        }
        //CASO 2: c'è intersezione tra la retta di intersezione e la direzione del lato.
        else if((edgeDir.cross(lineDir)).norm() > tol)
        {
            //Calcolo il parametro k.
            double k = ((point.cross(lineDir)-vertex0.cross(lineDir)).dot((edgeDir.cross(lineDir))))/(((edgeDir.cross(lineDir)).norm())*(edgeDir.cross(lineDir)).norm());

            //Se k ∈ [0,1] allora c'è intersezione tra la retta e il lato.
            if(k >tol && k<1+tol)
            {
                //Una volta appurato che c'è intersezione, calcolo il parametro t che indica l'esatto punto di intersezione e lo salvo in ts.
                double t = ((vertex0.cross(edgeDir)-point.cross(edgeDir)).dot((lineDir.cross(edgeDir))))/(((lineDir.cross(edgeDir)).norm())*(lineDir.cross(edgeDir)).norm());
                ts.push_back(t);
            }
//E' possibile attivare un ulteriore test per verificare l'intersezione tra le due fratture attraverso il metodo delle bounding box.
#ifdef TEST
            double t = ((vertex0.cross(edgeDir)-app.cross(edgeDir)).dot((lineDir.cross(edgeDir))))/(((lineDir.cross(edgeDir)).norm())*(lineDir.cross(edgeDir)).norm());
            Vector3d punto = (point+t*lineDir);

            Matrix<double,2,3> BBox = ComputeBoundingBox(fract,n1);
            Vector3d maxVec = BBox.row(0);
            Vector3d minVec = BBox.row(1);

            if(vettoreMin[0]-tol< punto[0] && punto[0] < maxVec[0]+tol &&
                vettoreMin[1]-tol< punto[1] && punto[1] < maxVec[1]+tol &&
                vettoreMin[2]-tol< punto[2] && punto[2] < maxVec[2]+tol)
            {
                ts.push_back(t);
            }
#endif
        }
        //Incremento l'indice
        j2 ++;
    }

    //Una volta calcolata la traccia, verifico se è passante o non passante e, nel primo caso, rispetto a quale frattura.
    bool intersection = false;
    //Queste due variabili booleane indiano se la traccia è passante per la frattura 1/2:
    //pass1 = true se la traccia è passante per la frattura n1. pass2 = true se la traccia è passante per la frattura n2.
    bool pass1;
    bool pass2;

    //Analizzo i casi possibili di relazione traccia-frattura.
    //CASO LIMITE: i poligono si intersecano solo in un punto.
    if(ts.size()<4)
    {
        return intersection;
    }
    else
    {
        //CASO1: non si ha intersezione tra i poligoni di conseguenza non esiste la traccia.
        if(max(ts[0],ts[1])  <= min(ts[2],ts[3]) + tol  ||
           max(ts[2],ts[3])  <= min(ts[0],ts[1]) + tol )
        {
            return intersection;
        }
        else
        {
            //CASO2: la traccia è passante per entrambe le fratture.
            if((abs(ts[0]-ts[2])<tol && abs(ts[1]-ts[3])<tol) ||
                (abs(ts[0]-ts[3])<tol && abs(ts[1]-ts[2])<tol))
            {
                pass1 = true;
                pass2 = true;
            }
            //CASO3: la traccia è passante per la frattura n2 ma non per la frattura n1.
            else if(min(ts[2],ts[3]) > min(ts[1],ts[0]) + tol && max(ts[2],ts[3]) < max(ts[0],ts[1])+ tol) // passante per la seconda frattura
            {
                pass1 = false; //li ho scambiati
                pass2 = true;

            }
            //CASO4: la traccia è passante per la frattura n1 ma non per la frattura n2.
            else if(min(ts[0],ts[1]) > min(ts[2],ts[3]) + tol && max(ts[0],ts[1]) < max(ts[2],ts[3])+ tol) // passante per la prima frattura
            {
                pass1 = true;
                pass2 = false;
            }
            //CASO5: la traccia è non passante sia per la frattura n1 che per la frattura n2.
            else
            {
                pass1 = false;
                pass2 = false;
            }

            //Ordino il vettore e tengo in considerazione solo i valori centrali (ts[1] e ts[2]) che rappresentano gli estremi della traccia.
            sort(ts.begin(),ts.end()); //l'algoritmo sort ha costo computazionale O(nlogn)
            intersection = true;

            unsigned int num = trac.ct;
            trac.IdTracce.push_back(num);
            Vector3d p1 = intersectionLine.row(0) + intersectionLine.row(1)*ts[1];
            Vector3d p2 = intersectionLine.row(0) + intersectionLine.row(1)*ts[2];
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
                descendingOrder(trac, fract.NumeroTracceN[n1], num);
            }
            else
            {
                descendingOrder(trac, fract.NumeroTracceP[n1], num);
            }
            if (pass2 == false)
            {
                passante2 = 1;
                descendingOrder(trac, fract.NumeroTracceN[n2], num);
            }
            else
            {
                descendingOrder(trac, fract.NumeroTracceP[n2], num);
            }
            Vector<unsigned int,2> riga1(n1,passante1);
            Vector<unsigned int,2> riga2(n2,passante2);
            Traccia.row(0) = riga1;
            Traccia.row(1) = riga2;
            trac.PNP.push_back(Traccia);

            trac.ct ++;
        }
    }


    return intersection;
}

//***************************************************************

void descendingOrder(TracesStruct& trac, list<unsigned int>& list, const unsigned int& num)
{

    double length = trac.LunghezzaTracce[num];

    auto itor = list.begin();
    while( length < trac.LunghezzaTracce[*itor] && itor != list.end())
    { 
        itor ++;

    }
    list.insert(itor,num);

    if (list.empty())
    {
        list.push_back(num);

    }

}

//****************************************************************

bool parallelPlanes(Vector4d& plane1, Vector4d& plane2, const double& tol)
{
    Vector3d p1 = plane1.head(3);
    Vector3d p2 = plane2.head(3);
    //double dotProduct = piano1[0]*piano2[0]+piano1[1]*piano2[1]+piano1[2]*piano2[2];

    if(p1.cross(p2).norm()<tol)
    {
        return true; //sono paralleli
    }
    return false;

}

//****************************************************************

Matrix<double,2,3> ComputeBoundingBox(const FractureStruct& fract,  unsigned int n)
{
    //Inizializzo i vettori Max e Min rispettivamente con i valori minimi e massimi possibili in modo da modificarli.
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
    //Salvo la BoundingBox in una matrice.
    Matrix<double,2,3> BBox;
    BBox.row(0) = vettoreMax;
    BBox.row(1) = vettoreMin;
    return BBox;

}

//****************************************************************

bool BBoxIntersection(const FractureStruct& fract, const unsigned int& n1, const unsigned int& n2,const double& tol)
{
    Matrix<double,2,3> BBox1 = ComputeBoundingBox(fract,n1);
    Matrix<double,2,3> BBox2 = ComputeBoundingBox(fract,n2);

    Vector3d vettoreMax1 = BBox1.row(0);
    Vector3d vettoreMin1 = BBox1.row(1);
    Vector3d vettoreMax2 = BBox2.row(0);
    Vector3d vettoreMin2 = BBox2.row(1);

    //Verifico se le due bounding box si intersecano.
    if(vettoreMin1[0]< vettoreMax2[0]+tol && vettoreMax1[0]> vettoreMin2[0]+tol &&
        vettoreMin1[1]< vettoreMax2[1]+tol && vettoreMax1[1]> vettoreMin2[1]+tol &&
        vettoreMin1[2]< vettoreMax2[2]+tol && vettoreMax1[2]> vettoreMin2[2]+tol)
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
        cerr<< "OutputTraces: Errore durante l'apertura del file."<< endl;
        return false;
    }

    unsigned int num = trac.ct;

    file << "# Number of Traces" << endl;
    file << num << endl;

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
        cerr<< "OutputFractures: Errore durante l'apertura del file."<< endl;
        return false;
    }

    for (unsigned int i=0; i < num_fratture; i++)
    {
        //Stampo nel file le tracce passanti.
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

bool GeneralOutput(const TracesStruct& trac, const FractureStruct& frac)
{
    bool printTraces = false;
    bool printFractures = false;

    if(OutputTraces(trac) == true)
    {
        printTraces = true;
        cout << "Output tracce eseguito correttamente." << endl;
    }
    else
    {
        cerr << "Impossibile stampare le tracce." << endl;
    }

    if(OutputFractures(trac,frac) == true)
    {
        printFractures = true;
        cout << "Output fratture eseguito correttamente." << endl;
    }
    else
    {
        cerr << "Impossibile stampare le fratture." << endl;
    }

    bool result = printTraces && printFractures;
    return result;
}

//****************************************************************

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////  PARTE 2 /////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//****************************************************************

bool subPolygons(list<Vector3d> verticiPolygons, const vector<Matrix<double,2,3>>& coordEstremiTracce, const Vector3d& normale, const double& tol)
{
    bool fermaEsplorazione = false;

    if (verticiPolygons.size() <= 3  || coordEstremiTracce.empty())
    {
        return true;
    }


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

    /// Condizione di Salvataggio
    ///
    if (destra.size() >= 3)
    {    /// salvataggioDati
        cout << "punti di destra: "<< endl;
        auto itor = destra.begin();
        while(itor != destra.end())
        {
            itor ++;
            cout << (*itor)[0] << " "<< (*itor)[1] << " "<< (*itor)[2] << endl;

        }
    }

    if(sinistra.size() >= 3)
    {
        cout << "punti di sinistra: "<< endl;
        auto itor2 = sinistra.begin();
        while(itor2 != sinistra.end())
        {
            itor2 ++;
            cout << (*itor2)[0] << " "<< (*itor2)[1] << " "<< (*itor2)[2] << endl;

        }
    }


    /// Chiamo subpolygons ricorsivamente sulle due liste
    // esplorazione in profondità????? CONTROLLARE
    while (fermaEsplorazione != true)
    {
        bool fermaEsplorazioneDx = subPolygons(destra, tdestra, normale, tol);
        bool fermaEsplorazioneSx = subPolygons(sinistra, tsinistra, normale, tol);

        fermaEsplorazione = fermaEsplorazioneDx && fermaEsplorazioneSx;
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

****
****


        cout << "tracce di destra: " << endl;
        for (unsigned int j = 0; j< tdestra.size(); j++)
        {
            cout << tdestra[j](0,0) << " "<< tdestra[j](0,1) << " " << tdestra[j](0,2) << endl;
            cout << tdestra[j](1,0) << " "<< tdestra[j](1,1) << " " << tdestra[j](1,2) << endl;

        }

        cout << "tracce di sinistra: " << endl;
        for (unsigned int j = 0; j< tsinistra.size(); j++)
        {
            cout << tsinistra[j](0,0) << " "<< tsinistra[j](0,1) << " " << tsinistra[j](0,2) << endl;
            cout << tsinistra[j](1,0) << " "<< tsinistra[j](1,1) << " " << tsinistra[j](1,2) << endl;
        }
    }

*/
