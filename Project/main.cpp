#include <iostream>
#include <fstream>
#include <Eigen/Eigen>
#include "Fratture.hpp"
#include "Utils.hpp"
#include "UCD_test.hpp"
#include "UCDUtilities.hpp"


using namespace std;
using namespace Eigen;
using namespace FractureLibrary;

int main()
{
    PolygonalMesh mesh;


    FractureStruct fract;
    TracesStruct trac;
    double tol = 10e-10;
    string NomeFile = "FR50_data.txt";


    /// Input dei dati

    if(!ImportData(NomeFile, fract))
    {
        cerr << "Impossibile importare i dati" << endl;

        return 1;
    }
    else
    {
        cout << "Dati importati correttamente" << endl;


        /// Analisi delle fratture
        // vengono chiamate in successione alcune funzioni che controllano
        // se c'è intersezione o meno fra due FRATTURE, eventualmente calcolo la retta di intersezione
        // e la TRACCIA corrispondente. Salvo la traccia aggiornando le due struct Fractures e Traces

        for (unsigned int i =0; i<fract.NumeroFratture; i++)
        {
            for (unsigned int j =i+1; j<fract.NumeroFratture; j++)
            {
                bool result = checkIntersezione(fract,trac,i,j,tol);

                if(result == false)
                    cout << "Main: NON c'e' intersezione tra: " <<i<< " e "<<j << endl;
                else
                    cout << "Main: c'e' intersezione tra: " <<i<< " e " <<j << endl;

            }
        }
    }


    /// File di output
    // vengono prodotti due file output: un elenco di tutte le tracce presenti nel DFN ordinate per ID e
    // un elenco dove per ogni frattura si indicano le tracce che gli appartengono ordinate per tip
    // e per lunghezza

    if(GeneralOutput(trac, fract) == false)
    {
        cerr << "Impossibile stampare i dati" << endl;

        return 2;
    }
    else
    {
        cout << "Dati stampati correttamente" << endl;
    }


    /// Parte 2 : suddivisione di una frattura in una mesh
    // Ogni frattura viene tagliata seguendo le tracce che la attraversano e si memorizzano sulla
    // struct PolygonalMesh i poligoni che derivano dal taglio

    for(unsigned int n = 0; n < fract.NumeroFratture; n++)
    {

        list<MatrixXd> sp = {}; // serve per memorizzare i subPolygons

        Vector3d normale = fract.NormaleFrattura[n];
        list<unsigned int> listaTracce = fract.NumeroTracceP[n];
        vector<Matrix<double,2,3>> coordEstremiTracce;

        if(listaTracce.empty())
        {
            cout << "nessuna traccia P per frattura: " << n << endl;
        }
        else
        {
            while(!listaTracce.empty())
            {
                coordEstremiTracce.push_back(trac.EstremiTracce[listaTracce.front()]);
                listaTracce.pop_front();
            }

            list<Vector3d> verticiPolygons;
            for(unsigned int c = 0; c < fract.CoordinateVertici[n].cols(); c++)
            {
                verticiPolygons.push_back(fract.CoordinateVertici[n].col(c));
            }


            bool taglio = subPolygons(verticiPolygons, coordEstremiTracce,sp, normale, tol);

            if (taglio == true)
                cout << "taglio a buon fine per frattura: "<< n <<endl;
        }

        /// Memorizzo le informazioni nella mesh

        if (!sp.empty())
        {
            unsigned int id = 0;
            unsigned int IdLato = 0;
            mesh.NumberCell2D = sp.size();
            mesh.NumeroDiLati.reserve(sp.size());
            mesh.NumeroDiVertici.reserve(sp.size());

            // troviamo le dimensioni per impostare la mesh
            auto itor2 = sp.begin();
            unsigned int TotCol = 0;
            while(itor2 != sp.end())
            {
                TotCol = TotCol + (*itor2).cols();
                itor2 ++;
            }

            mesh.Cell0DId.reserve(TotCol);
            mesh.Cell1DId.reserve(TotCol);
            mesh.Cell0DCoordinates.reserve(TotCol);
            mesh.Cell1DVertices.reserve(TotCol);
            mesh.Cell2DVertices.reserve(TotCol);
            mesh.Cell2DEdges.reserve(TotCol);



            auto itor3 = sp.begin();
            while(itor3 != sp.end()) // ciclo su ogni sottopoligono
            {
                MatrixXd matrice = (*itor3);
                unsigned int mc = matrice.cols();


                // riempio mesh.Cell0DCoordinates
                // confronto ogni punto con quelli già memorizzati nella struct
                // se non è presente lo inserisco
                for (unsigned int c= 0; c<mc; c++)
                {
                    bool same = false;

                    for(unsigned int col = 0; col < mesh.Cell0DCoordinates.size(); col++)
                    {
                        if (abs(matrice.col(c)[0] - mesh.Cell0DCoordinates[col][0]) < tol &&
                            abs(matrice.col(c)[1] - mesh.Cell0DCoordinates[col][1]) < tol &&
                            abs(matrice.col(c)[2] - mesh.Cell0DCoordinates[col][2]) < tol)
                        {
                            same = true;
                        }
                    }

                    if (same == false)
                    {
                        mesh.Cell0DId.push_back(id);
                        mesh.Cell0DCoordinates.push_back(matrice.col(c));
                        id ++;
                    }
                }

                itor3++;
            }


            auto itor = sp.begin();
            while(itor != sp.end())
            {
                MatrixXd matrice = (*itor);
                unsigned int mc = matrice.cols();
                mesh.NumberCell0D = mesh.NumberCell0D + mc;
                mesh.NumberCell1D = mesh.NumberCell1D +mc;
                mesh.NumeroDiLati.push_back(mc);
                mesh.NumeroDiVertici.push_back(mc);

                VectorXi lati = {};
                VectorXi vertici = {};
                lati.resize(mc);
                vertici.resize(mc);

                unsigned int v1, v2;
                for(unsigned int i = 0; i < matrice.cols(); i++) // ciclo sul numero di lati
                {
                    v1 = i % matrice.cols();
                    v2 = (i+1) % matrice.cols();
                    unsigned int id1, id2;

                    // cerco l'id del punto corrispondente alla colonna matrice.col(v1)
                    for(unsigned int col = 0; col < mesh.Cell0DCoordinates.size(); col++)
                    {
                        if (abs(matrice.col(v1)[0] - mesh.Cell0DCoordinates[col][0]) < tol &&
                            abs(matrice.col(v1)[1] - mesh.Cell0DCoordinates[col][1]) < tol &&
                            abs(matrice.col(v1)[2] - mesh.Cell0DCoordinates[col][2]) < tol)
                        {
                            id1 = col;
                        }
                    }
                    // cerco l'id del punto corrispondente alla colonna matrice.col(v2)
                    for(unsigned int col = 0; col < mesh.Cell0DCoordinates.size(); col++)
                    {
                        if (abs(matrice.col(v2)[0] - mesh.Cell0DCoordinates[col][0]) < tol &&
                            abs(matrice.col(v2)[1] - mesh.Cell0DCoordinates[col][1]) < tol &&
                            abs(matrice.col(v2)[2] - mesh.Cell0DCoordinates[col][2]) < tol)
                        {
                            id2 = col;
                        }
                    }

                    Vector2i edge(id1, id2);
                    mesh.Cell1DVertices.push_back(edge);
                    mesh.Cell1DId.push_back(IdLato);
                    IdLato ++;

                    vertici[i] = v1;
                    lati[i]= IdLato;

            }


            mesh.Cell2DEdges.push_back(lati);
            mesh.Cell2DVertices.push_back(vertici);





            itor++;
        }
        }
        else
        {
            mesh.Cell0DCoordinates = {};
            mesh.Cell0DId = {};
            mesh.Cell1DId = {};
            mesh.Cell1DVertices = {};
            mesh.Cell2DEdges = {};
            mesh.Cell2DVertices = {};
            mesh.NumberCell0D = {};
            mesh.NumberCell1D = {};
            mesh.NumberCell2D = {};
            mesh.NumeroDiLati = {};
            mesh.NumeroDiVertici = {};
        }
    }





    ///*******************************************************************
    ///                     voglio stampare la mesh
    ///*******************************************************************



    unsigned int c = 0;
    MatrixXd points(3, mesh.Cell0DCoordinates.size());
    for (unsigned int i = 0; i< mesh.Cell0DCoordinates.size(); i++)
    {
        points.col(c) = mesh.Cell0DCoordinates[i];
        c++;
    }


    unsigned int p = 0;
    MatrixXi edges(2, mesh.Cell1DVertices.size());
    for (unsigned int i = 0; i< mesh.Cell1DVertices.size(); i++)
    {
        edges.col(p) = mesh.Cell1DVertices[i];
        p++;
    }


    vector<vector<unsigned int>> indici = {};
    indici.resize(mesh.NumberCell2D);
    for(unsigned int i= 0; i<mesh.NumberCell2D; i++)
    {
        vector<unsigned int> v = {};
        unsigned int count = 0;
        for(unsigned int j =0; j<mesh.NumeroDiVertici.size(); j++)
        {
            v.push_back(count);
            count ++;
        }

        indici.push_back(v);
    }


    std::string exportFolder = "./";
    Gedim::UCDUtilities exporter;
    exporter.ExportPoints(exportFolder + "/Mesh0Ds.inp",points);

    exporter.ExportSegments(exportFolder + "/Mesh1Ds.inp",points,edges);

    //exporter.ExportPolygons(exportFolder + "/Mesh2Ds.inp",points,indici);





    return 0;

} // main


/*

bool extendTracesNP(list<Vector3d> verticiPolygons, const vector<Matrix<double,2,3>>& coordEstremiTracce, vector<Matrix<double,2,3>>& nuoveTracce, const double& tol)
{
    // in input dò le tracce non passanti e le faccio passare tutte
    for(unsigned int t=0; t<coordEstremiTracce.size(); t++)
    {
        //prendo i due estremi della traccia da analizzare
        Vector3d A = coordEstremiTracce[t].row(0);
        Vector3d B = coordEstremiTracce[t].row(1);
        Vector3d direzioneTraccia = B-A;
        const unsigned int num = verticiPolygons.size();
        // Inizializzo la nuova traccia che sarà data dai punti di intersezione tra la retta passante per la traccia e quella passante per i lati che interseca
        // I lati che interseca ovviamente saranno due per cui con questo while cerco l'intersezione per ognuno dei quattro lati

        Matrix<double,2,3> nuovaTraccia = {};
        unsigned int e = 0;
        while (e < num )
        {
            // vertici da analizzare
            Vector3d verticeA = verticiPolygons.front();
            verticiPolygons.push_back(verticeA);
            verticiPolygons.pop_front();
            Vector3d verticeB = verticiPolygons.front();
            Vector3d direzioneLato = verticeB-verticeA;
            // chatgpt ha detto che si può fare così a trovare le intersezioni tra due rette di cui siano direzione e punto di applicazione
            Vector3d prodVet = direzioneTraccia.cross(direzioneLato);
            Vector3d vetTraPunti = A-verticeA; // è il vettore tra punti di applicazione
            double determinante = prodVet.dot(prodVet);
            if (abs(determinante) > tol) // se il determinante fosse uguale a zero sarebbero parallele o coincidenti quindi non c'è intersezione
            {
                double t = (vetTraPunti.dot(direzioneLato.cross(prodVet)))/determinante;
                Vector3d estremoNuovo = A + t*direzioneTraccia;
                if (nuovaTraccia.row(0)[0]<tol, nuovaTraccia.row(0)[1]< tol, nuovaTraccia.row(0)[2]<tol)
                {
                    nuovaTraccia.row(0) = estremoNuovo;
                }
                else
                {
                    nuovaTraccia.row(1) = estremoNuovo;
                }

            }
        }
        ///// QUI BISOGNA FARE IN MODO CHE LA NUOVA TRACCIA CHE è UNA MATRICE CON SULLE RIGHE GLI ESTREMI SIA INSERITA NELLA LISTA DELLE TRACCE PASSANTI
        nuoveTracce.push_back((nuovaTraccia));
    }
}





*/
