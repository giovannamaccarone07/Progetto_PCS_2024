#ifndef __TESTFRATTURE_H
#define __TESTFRATTURE_H

#include <gtest/gtest.h>
#include "Fratture.hpp"
#include "Eigen/Eigen"
#include <iostream>
#include "UCDUtilities.hpp"
#include <math.h>
#include "Utils.hpp"

using namespace Eigen;
using namespace std;

namespace FractureLibrary {
//********************************
/*TEST(TRIANGLETEST, TestComputeArea){

    Matrix3d vertices = Matrix3d::Zero();

    vertices << 0.0, 1.0, 0.0,
        0.0, 0.0, 1.0,
        0.0, 0.0, 0.0;

    Triangle t(vertices);

    double area = t.computeArea();
    EXPECT_EQ(area, 0.5);
}
*///********************************
/*
TEST(FRACTURE_TEST, TestPlotParaviewFractures){

    MatrixXd points = MatrixXd::Zero(3, 4);

    points << 0.0, 1.0, 1.0, 0.0,
        0.0, 0.0, 1.0, 1.0,
        0.0, 0.0, 0.0, 0.0;

    const std::vector<std::vector<unsigned int>> triangles =
        {
            { 0, 1, 3 },
            { 1, 2, 3 }
        };

    Eigen::VectorXi materials(2);
    materials << 0, 1;

    Gedim::UCDUtilities exporter;

    const std::vector<Gedim::UCDProperty<double> > points_properties = {};
    const std::vector<Gedim::UCDProperty<double> > polygons_properties = {};

    exporter.ExportPolygons("./Geometry2Ds.inp",
                            points,
                            triangles,
                            points_properties,
                            polygons_properties,
                            materials);


}
/********************************
TEST(POLYGONTEST, TestPlotParaviewPolygons){

    MatrixXd points = MatrixXd::Zero(3, 4);

    points << 0.0, 1.0, 1.0, 0.0,
        0.0, 0.0, 1.0, 1.0,
        0.0, 0.0, 0.0, 0.0;

    vector<vector<unsigned int>> pol_vertices = { {0, 1, 2, 3}};

    Polygons polygons(points,
                     pol_vertices);
    std::vector<std::vector<unsigned int>> triangles;
    Eigen::VectorXi materials;

    polygons.GedimInterface(triangles, materials);

    Gedim::UCDUtilities exporter;

    exporter.ExportPolygons("./Geometry2Ds.inp",
                            points,
                            triangles,
                            {},
                            {},
                            materials);
}
/********************************
*/


double tol = numeric_limits<double>::epsilon();

TEST(ImportaDatiTest, NumeroVertici)
{
    FractureStruct fract;
    ImportaDati("FR3_data.txt",fract);
    unsigned int vertici = fract.NumeroVertici[0]; // controllo che la prima frattura abbia 4 vertici
    ASSERT_EQ(4,vertici);

}

//TEST funzione RettaIntersezione
/*
TEST(RettaIntersezioneTest, PianiSecanti)
{
    //Valori dati
    Vector4d piano1(1,1,1,1);
    Vector4d piano2(1,1,1,2);

    MatrixXd rettaCalcolata = RettaIntersezione(piano1, piano2);
    MatrixXd rettaCorretta;
    Vector3d appCorretta(1,0,0);
    Vector3d tangCorretta(1,0,-1);
    rettaCorretta.row(0) = appCorretta;
    rettaCorretta.row(1) = tangCorretta;

    unsigned int rows = 2;
    unsigned int columns = 3;

    for(unsigned j = 0; j< columns; j++){
        for(unsigned int i = 0; i< rows; i++)
        {
            EXPECT_NEAR(rettaCalcolata(i,j),rettaCorretta(i,j), tol);
        }
    }
}
*/
TEST(RettaIntersezioneTest, PuntiAppartenentiPiani)
{
    Vector4d piano1(1,1,1,-1); //CONTROLLARE IL SEGNO DI d
    Vector4d piano2(1,1,0,-2);
    MatrixXd retta = RettaIntersezione(piano1, piano2);

    //Calcolo le distanze punto-retta considerando come punti: il punto di applicazione della retta e il punto indicato dalla tangente della retta
    double d1 = (abs(piano1[0]*retta.row(0)[0]+piano1[1]*retta.row(0)[1]+piano1[2]*retta.row(0)[2])+piano1[3])/sqrt(piano1[0]*piano1[0]+piano1[1]*piano1[1]+piano1[2]*piano1[2]);
    //double d2 = (abs(piano1[0]*retta.row(1)[0]+piano1[1]*retta.row(1)[1]+piano1[2]*retta.row(1)[2])+piano1[3])/sqrt(piano1[0]*piano1[0]+piano1[1]*piano1[1]+piano1[2]*piano1[2]);
    double d3 = (abs(piano2[0]*retta.row(0)[0]+piano2[1]*retta.row(0)[1]+piano2[2]*retta.row(0)[2])+piano2[3])/sqrt(piano2[0]*piano2[0]+piano2[1]*piano2[1]+piano2[2]*piano2[2]);
    //double d4 = (abs(piano2[0]*retta.row(1)[0]+piano2[1]*retta.row(1)[1]+piano2[2]*retta.row(1)[2])+piano2[3])/sqrt(piano2[0]*piano2[0]+piano2[1]*piano2[1]+piano2[2]*piano2[2]);

    EXPECT_NEAR(d1,0,tol);
    //EXPECT_NEAR(d2,0,tol);
    EXPECT_NEAR(d3,0,tol);
    //EXPECT_NEAR(d4,0,tol);
 }

TEST(RettaIntersezioneTest, PianiPoligoni01)
{
    Vector4d piano1(0,0,1,0); //CONTROLLARE IL SEGNO DI d
    Vector4d piano2(0.4,0,0,-0.32);
    MatrixXd retta = RettaIntersezione(piano1, piano2);

    //Calcolo le distanze punto-retta considerando come punti: il punto di applicazione della retta e il punto indicato dalla tangente della retta
    double d1 = (abs(piano1[0]*retta.row(0)[0]+piano1[1]*retta.row(0)[1]+piano1[2]*retta.row(0)[2])+piano1[3])/sqrt(piano1[0]*piano1[0]+piano1[1]*piano1[1]+piano1[2]*piano1[2]);
    //double d2 = (abs(piano1[0]*retta.row(1)[0]+piano1[1]*retta.row(1)[1]+piano1[2]*retta.row(1)[2])+piano1[3])/sqrt(piano1[0]*piano1[0]+piano1[1]*piano1[1]+piano1[2]*piano1[2]);
    double d3 = (abs(piano2[0]*retta.row(0)[0]+piano2[1]*retta.row(0)[1]+piano2[2]*retta.row(0)[2])+piano2[3])/sqrt(piano2[0]*piano2[0]+piano2[1]*piano2[1]+piano2[2]*piano2[2]);
    //double d4 = (abs(piano2[0]*retta.row(1)[0]+piano2[1]*retta.row(1)[1]+piano2[2]*retta.row(1)[2])+piano2[3])/sqrt(piano2[0]*piano2[0]+piano2[1]*piano2[1]+piano2[2]*piano2[2]);

    EXPECT_NEAR(d1,0,tol);
    //EXPECT_NEAR(d2,0,tol);
    EXPECT_NEAR(d3,0,tol);
    //EXPECT_NEAR(d4,0,tol);
}

TEST(PianoPassantePerFratturaTest, PianoPassante)
{
    //FractureStruct fract;
    Vector4d pianoCalcolato1 = PianoPassantePerFrattura(fract,0);
    Vector4d pianoCorretto1(0,0,1,0);

    for(unsigned int i=0;i<pianoCorretto1.size();i++)
    {
        EXPECT_NEAR(pianoCalcolato1[i],pianoCorretto1[i],tol);
    }

    Vector4d pianoCalcolato2 = PianoPassantePerFrattura(fract,1);
    Vector4d pianoCorretto2(0.4,0,0,-0.32);

    for(unsigned int i=0;i<pianoCorretto2.size();i++)
    {
        EXPECT_NEAR(pianoCalcolato2[i],pianoCorretto2[i],tol);
    }
}

TEST(ComputeBoundingBoxTest, CorrettezzaBBox)
{
    Matrix<double,2,3> BBoxCalcolata = ComputeBoundingBox(fract,0);
    Matrix<double,2,3> ExpectedBBox;
    Vector3d firstRow(0,0,0);
    Vector3d secondRow(1,1,0);
    ExpectedBBox.row(0) = firstRow;
    ExpectedBBox.row(1) = secondRow;

    for(unsigned int i = 0; i< 2; i++){
        for(unsigned int j = 0; j<3;j++){
            ASSERT_EQ(BBoxCalcolata(i,j),ExpectedBBox(i,j)); // se =
            EXPECT_NEAR(BBoxCalcolata(i,j),ExpectedBBox(i,j),tol);
        }
    }

}

TEST(CheckBoundingBoxTest, CheckBoundingBox){

    ASSERT_TRUE(CheckBoundingBox(fract,0,1));
    ASSERT_FALSE(CheckBoundingBox(fract,1,2));

}

TEST(pianiParalleliTest, PianiParalleli){

    Vector4d piano1(2,3,4,5);
    Vector4d piano2(2,3,4,-7);
    ASSERT_TRUE(pianiParalleli(piano1,piano2));

    Vector4d piano3(1,2,3,-4);
    Vector4d piano4(2,-1,1,1);
    ASSERT_FALSE(pianiParalleli(piano3,piano4));


}













/*/ ***************************************************************************
TEST(SquareRootTest, NegativeNos)
{
    ASSERT_EQ(-1.0, squareRoot(-15.0));
    ASSERT_EQ(-1.0, squareRoot(-0.2));
}
/*/



}



#endif


