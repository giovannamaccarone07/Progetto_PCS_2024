#pragma once
#include <iostream>
#include "Eigen/Eigen"
using namespace std;
using namespace Eigen;

namespace FractureLibrary{


struct FractureStruct
{
    /// NumeroFratture
    unsigned int NumeroFratture = 0;

    /// IdFratture
    // Vector in cui vengono salvate le fratture
    vector<unsigned int> IdFratture = {};               /// dim: 1 x NumeroFratture

    /// NumeroVertici
    // Vector in cui vengono salvati i numeri di vertici presenti in ogni frattura
    vector<unsigned int> NumeroVertici = {};            /// dim: 1 x NumeroFratture

    /// CoordinateVertici
    // Vector che contiene in posizione i-esima la matrice dei vertici della frattura i-esima.
    // Ogni matrice ha dimensione 3xNumeroVertici
    vector<MatrixXd> CoordinateVertici = {};

    ///NumeroTracceN
    // E' un vettore di liste di unsigned int in cui, in posizione i-esima, è salvata la lista contenente gli Id della trcce non passanti per la frattura i-esima.
    vector<list<unsigned int>> NumeroTracceN = {};

    ///NumeroTracceP
    // E' un vettore di liste di unsigned int in cui, in posizione i-esima, è salvata la lista contenente gli Id della trcce passanti per la frattura i-esima .
    vector<list<unsigned int>> NumeroTracceP= {};

    ///Normale Frattura
    // vector di Vector3d in cui, in posizione i-esima, sono salvate le coordinate della giaciutura del piano chhe contiene la i-esima frattura.
    vector<Vector3d> NormaleFrattura = {};

};


struct TracesStruct
{
    ///ContatoreTracce
    unsigned int ct = 0;

    ///VettoreIdTracce
    // Vector che salva gli id delle tracce.
    vector<unsigned int> IdTracce = {};                 /// dim: 1 x ct

    ///CoordinateEstremiTracce
    // Vector che salva le coordinate degli estremi delle tracce: in posizione i-esima si trova la traccia di indice i.
    // Inoltre, la matrice della posizione i-esima è 2x3 dove la prima riga contiene le coordinate del primo estremo e analogamente la seconda.
    vector<Matrix<double,2,3>> EstremiTracce = {};      /// dim: 1 x ct

    ///LunghezzaTracce
    // Vettore che salva la lunghezza di ogni traccia.
    vector<double> LunghezzaTracce = {};                /// dim: 1 x ct

    ///MatriceCaratteristicheTraccia
    // Vettore di matrici quadrate 2x2. La matrice che si trova in posizione i-esima è relativa alla traccia di id i e le due righe indicano le fratture tra cui esiste la traccia.
    // Ogni riga è formata da frattura  e un booleano a cui è associato True se la traccia è passante per quella frattura o False se non lo è.
    vector<Matrix<unsigned int,2,2>> PNP = {};          /// dim: 1 x ct
};


struct PolygonalMesh
{
    ///Celle 0D

    // Memorizzo il numero di celle
    unsigned int NumberCell0D = 0; ///< number of Cell0D
    // Vector di interi in cui sono memorizzati gli Id delle celle.
    vector<unsigned int> Cell0DId = {}; ///< Cell0D id, size 1 x NumberCell0D
    // Vector di vettori a due componenti in cui salvo le coordinate (X,Y) delle celle
    vector<Vector3d> Cell0DCoordinates = {}; ///< Cell0D coordinates, size 2 x NumberCell0D (x,y,z)


    /// Celle 1D

    // Memorizzo il numero di celle
    unsigned int NumberCell1D = 0; ///< number of Cell1D
    // Vector di interi dove memorizzo gli Id delle celle
    vector<unsigned int> Cell1DId = {}; ///< Cell1D id, size 1 x NumberCell1D
    // Vector di vettori a due componenti in cui salvo l'id di origine e fine delle celle0D che formano la cella1D
    vector<Vector2i> Cell1DVertices = {}; ///< Cell1D vertices indices, size 2 x NumberCell1D (fromId,toId)

    /// Celle 2D

    // Memorizzo il numero di celle
    unsigned int NumberCell2D = 0; ///< number of Cell2D
    // Vector di interi dove si memorizzano i numeri di vertici che compongono le celle 2D
    vector<unsigned int> NumeroDiVertici = {};
    // Vector di interi dove si memorizzano i numeri di lati che compongono le celle 2D
    vector<unsigned int> NumeroDiLati = {};
    // Vector in cui sono salvati gli Id dei vertici che formano il poligono
    vector<VectorXi> Cell2DVertices = {}; ///< Cell2D Vertices indices
    // Vector in cui sono salvati gli Id dei lati che formano il poligono
    vector<VectorXi> Cell2DEdges = {}; ///< Cell2D Cell1D indices

};


}
