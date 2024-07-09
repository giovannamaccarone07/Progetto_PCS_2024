#pragma once
#include <iostream>
#include "Eigen/Eigen"
using namespace std;
using namespace Eigen;

namespace FractureLibrary{


struct FractureStruct
{
    /// IdFratture
    /// NumeroVertici
    /// IndiciVertici
    // ogni frattura è composta da un numero di vertici. Ad ogni vertice corrisponde un indice.
    // Ad ogni posizione del vector corrisponde un vector<unsigned int> che memorizza gli indici dei vertici di quella frattura
    // Siccome ogni frattura ha un numero variabile di vertici non sappiamo a priori la dimensione del vector<unsigned int> interno.
    // Mentre quello esterno sarà lungo quanto NumeroFratture
    /// CoordinateVertici
    // vector contentente i vettori di 3 coordinate l'uno ordinate in modo che gli indici corrispondando alla posizione
    // delle coordinate del vertice. Questa struttura è di fatto una matrice NumeroVertici x 3
    // (però in questo modo cambia la sintassi per accedere)
    // ATTENZIONE
    // nel nuovo aggiornamento CoordinateVertici è un vector che contiene in ogni posizione la matrice dei vertici
    // anzichè avere la matrice unica abbiamo tante piccole matrici frammentate.
    /// SpazioMemoria
    // E' una variabile che permette di riservare spazio per la registrazione dei dati nella struttura
    // in base alla lunghezza del file che si sta leggendo
    ///NumeroTracce
    // E' un vettore di matrici 2xn (con n = numero di tracce associate alla frattura i-esima).
    // Ogni componente del vettore contiene una matrice che indica tutte le tracce che intersecano la frattura i-esima.
    // Ogni colonna della matrice contiene l'id della traccia (prima riga) e il booleano che indica se la traccia stessa è passante o meno per la frattura.


    /////////////////////////// PER L'AMOR DI DIO TOGLIERE LE VARIABILI CHE NON SI USANO ANCHE NELLA DOCUMENTAZIONE
    /// indici vertici ha senso di esistere?
    ///
    /// per ogni frattura ha senso memorizzare il piano che la contiene così poi nella parte2 quando devo calcolare la normale del piano ce l'ho già?
    /// oppure ingombra solo memoria e conviene piuttosto ricalcolarsi la normale quando serve in subPolygons??
    ///
    vector<Vector3d> NormaleFrattura = {};
    ///
    ///
    unsigned int NumeroFratture = 0;
    vector<unsigned int> IdFratture = {};               /// dim: 1 x NumeroFratture
    vector<unsigned int> NumeroVertici = {};            /// dim: 1 x NumeroFratture
    vector<vector<unsigned int>> IndiciVertici = {};    /// dim: vector<unsigned int> x NumeroFratture
    vector<MatrixXd> CoordinateVertici = {};            /// dim: MatrixXd x NumeroVertici
    unsigned int SpazioMemoria = 0;
    vector<list<unsigned int>> NumeroTracceN = {};
    vector<list<unsigned int>> NumeroTracceP= {};

    // vector<PolygonalMesh> mesh = {};                    /// dim: 1 x NumeroFratture
    // in ogni posizione ho un oggetto di tipo mesh associato alla frattura i-esima
    // da aggiungere alla uml ?
};


struct TracesStruct
{
    ///ContatoreTracce
    unsigned int ct = 0;

    ///VettoreIdTracce
    // Vettore che salva gli id delle tracce
    vector<unsigned int> IdTracce = {};                 /// dim: 1 x ct

    ///CoordinateEstremiTracce
    // Vettore che salva le coordinate degli estremi delle tracce: la i-esima coordinata del vettore corrisponde alla traccia di indice i.
    //Inoltre, la matrice della posizione i-esima è 2x3 dove la prima riga contiene le coordinate del primo estremo e analogamente la seconda.
    vector<Matrix<double,2,3>> EstremiTracce = {};      /// dim: 1 x ct

    ///LunghezzaTracce
    // Vettore che salva la lunghezza di ogni traccia
    vector<double> LunghezzaTracce = {};                /// dim: 1 x ct

    ///MatriceCaratteristicheTraccia
    // Vettore di matrici quadrate 2x2. La matrice che si trova in posizione i-esima è relativa alla traccia di id i e le due righe indicano le fratture tra cui esiste la traccia.
    // Ogni riga è formata da frattura  e un booleano a cui è associato True se la traccia è passante per quella frattura o False se non lo è.
    vector<Matrix<unsigned int,2,2>> PNP = {};                          /// dim: 1 x ct
};


struct PolygonalMesh
{
    ///celle 0D
    // memorizzo il numero di celle
    // creo un vector di interi dove memorizzo gli Id delle celle
    // creo un vector di vettori a due componenti in cui salvo le coordinate (X,Y) delle celle
    // creo un dizionario per salvare i marker, ad ogni marker associo una lista con gli Id delle celle che hanno quel marker
    //
    unsigned int NumberCell0D = 0; ///< number of Cell0D
    vector<unsigned int> Cell0DId = {}; ///< Cell0D id, size 1 x NumberCell0D
    vector<Vector3d> Cell0DCoordinates = {}; ///< Cell0D coordinates, size 2 x NumberCell0D (x,y)

    ///celle 1D
    // memorizzo il numero di celle
    // creo un vector di interi dove memorizzo gli Id delle celle
    // creo un vector di vettori a due componenti in cui salvo l'id di origine e fine delle celle0D che formano la cella1D
    // creo un dizionario per salvare i marker, ad ogni marker associo una lista con gli Id delle celle che hanno quel marker
    //
    unsigned int NumberCell1D = 0; ///< number of Cell1D
    vector<unsigned int> Cell1DId = {}; ///< Cell1D id, size 1 x NumberCell1D
    vector<Vector2i> Cell1DVertices = {}; ///< Cell1D vertices indices, size 2 x NumberCell1D (fromId,toId)




    ////// MA SONO INT OPP UNSIGNED INT ? ho un vector di unsigned int opp Vector2i di interi?



    ///celle 2D
    // memorizzo il numero di celle
    // creo un vector di interi dove memorizzo gli Id delle celle
    // creo un vector di Vector per memorizzare la sequenza dei vertici della cella (i vertici sono gli id delle celle 0D)
    // creo un vector di Vector per memorizzare la sequenza degli archi della cella (gli archi sono gli id delle celle 1D)
    //
    unsigned int NumberCell2D = 0; ///< number of Cell2D
    vector<unsigned int> NumeroDiVertici = {};
    vector<unsigned int> NumeroDiLati = {};
    vector<VectorXi> Cell2DVertices = {}; ///< Cell2D Vertices indices
    vector<VectorXi> Cell2DEdges = {}; ///< Cell2D Cell1D indices



};


}
