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


double tol = 10e-10; //CAMBIARE LA TOLLERANZA

//TEST 1: testiamo la funzione ImportaDati verificando che la prima frattura abbiamo quattro vertici.
TEST(ImportaDatiTest, NumeroVertici)
{
    FractureStruct fract;
    ImportData("FR3_data.txt",fract);
    unsigned int vertici = fract.NumeroVertici[0];
    ASSERT_EQ(4,vertici);

}

//****************************************************************

//TEST 2: testiamo la funzione RettaIntersezione verificando che, dati due piani che si intersecano,
//la retta calcolata autonomamente e quella calcolata dalla funzione coincidono.
TEST(RettaIntersezioneTest, PianiSecanti)
{
    Vector4d piano1(1,-1,1,1);
    Vector4d piano2(1,1,1,2);

    Matrix<double,2,3> line = IntersectionLine(piano1, piano2);
    Matrix<double,2,3> expectedLine;
    //Vector3d appCorretta(1.5,0.5,0);
    Vector3d lineDirection(-2,0,2);
    //rettaCorretta.row(0) = appCorretta;
    expectedLine.row(1) = lineDirection;

    for (int i = 0; i < 3; i++)
    {
        EXPECT_NEAR(line(1, i)/expectedLine(1, i), 1.0, tol);
    //VERIFICO CHE SONO UNO IL MULTIPLO DELL'ALTRO, FACCIO IL TEST SOLO SULLE DIREZIONI
    }
}

//****************************************************************

//TEST 3: testiamo la funzione RettaIntersezione verificando che, dati due piani che si intersecano,
//il punto di applicazione della retta calcolata dalla funzione appartenga ad entrambi i piani.
TEST(RettaIntersezioneTest, PuntiAppartenentiPiani)
{
    Vector4d piano1(1,1,1,-1); //CONTROLLARE IL SEGNO DI d
    Vector4d piano2(1,1,0,-2);
    Matrix<double,2,3> retta = IntersectionLine(piano1, piano2);

    //Calcolo le distanze punto-piano

    //Distanza del punto di applicazione dal piano1
    double d1 = (abs(piano1[0]*retta.row(0)[0]+piano1[1]*retta.row(0)[1]+piano1[2]*retta.row(0)[2])+piano1[3])/sqrt(piano1[0]*piano1[0]+piano1[1]*piano1[1]+piano1[2]*piano1[2]);
    //Distanza del punto di applicazione dal piano2
    double d3 = (abs(piano2[0]*retta.row(0)[0]+piano2[1]*retta.row(0)[1]+piano2[2]*retta.row(0)[2])+piano2[3])/sqrt(piano2[0]*piano2[0]+piano2[1]*piano2[1]+piano2[2]*piano2[2]);

    EXPECT_NEAR(d1,0,tol);
    EXPECT_NEAR(d3,0,tol);
}

//****************************************************************

//TEST 4: testiamo la funzione PianoPassantePerFrattura verificando che, importate le fratture di Id 0 e 1
//dal file FR3_data.txt, il piano passante per quest'ultima e quello calcolato dalla funzione coincidano.
TEST(PianoPassantePerFratturaTest, PianoPassante)
{
    FractureStruct fract;
    ImportData("FR3_data.txt",fract);

    Vector4d pianoCalcolato1 = FracturePlane(fract,0);
    Vector4d pianoCorretto1(0,0,1,0);

    for(unsigned int i=0;i<pianoCorretto1.size();i++)
    {
        EXPECT_NEAR(pianoCalcolato1[i],pianoCorretto1[i],tol);
    }

    Vector4d pianoCalcolato2 = FracturePlane(fract,1);
    Vector4d pianoCorretto2(-0.4,0,0,0.32);

    for(unsigned int i=0;i<pianoCorretto2.size();i++)
    {
        EXPECT_NEAR(pianoCalcolato2[i],pianoCorretto2[i],tol);
    }

}

//****************************************************************

//TEST 5: testiamo la funzione ComputeBoundingBox verificando che, importata la frattura 0 dal file FR3_data.txt,
//la bounding box che contiene quest'ultima e quella calcolata dalla funzione coinicdano.
TEST(ComputeBoundingBoxTest, CorrettezzaBBox)
{
    FractureStruct fract;
    ImportData("FR3_data.txt",fract);
    Matrix<double,2,3> BBox = ComputeBoundingBox(fract,0);
    Vector3d expectedMin(0,0,0);
    Vector3d expectedMax(1,1,0);

    for(unsigned int i=0; i<expectedMax.size(); i++)
    {
        EXPECT_NEAR(expectedMax(i),BBox(0,i),tol);
    }

    for(unsigned int i=0; i<expectedMin.size(); i++)
    {
        EXPECT_NEAR(expectedMin(i),BBox(1,i),tol);
    }

}

//****************************************************************

//TEST 6: testiamo la funzione IntersezioneBoundingBox verificando che, importate le fratture dal file FR3_data.txt,
//le bounding box che contengono le fratture di Id 0 e 1 si intersecano, mentre quelle che contengono le fratture  di Id 1 e 2 non si intersecano.

TEST(IntersezioneBoundingBoxTest, IntersezioneBoundingBox)
{
    FractureStruct fract;
    ImportData("FR3_data.txt",fract);
    ASSERT_TRUE(BBoxIntersection(fract,0,1,tol));
    ASSERT_FALSE(BBoxIntersection(fract,1,2,tol));

}

//****************************************************************

//TEST 7: testiamo la funzione piani paralleli verificando che, definiti due piani che si sa intersecarsi e
//due piani che si sa essere paralleli, la funzione, prendendo i inpiut la prima coppia resituisce TRUE,
//mentre prendendo in input la seconda coppia, restituisce FALSE.

TEST(pianiParalleliTest, PianiParalleli){

    //Test con piani paralleli
    Vector4d piano1(2,3,4,5);
    Vector4d piano2(2,3,4,10);
    ASSERT_TRUE(parallelPlanes(piano1,piano2,tol));

    //Test con piani secanti
    Vector4d piano3(1,2,3,-4);
    Vector4d piano4(2,-1,1,1);
    ASSERT_FALSE(parallelPlanes(piano3,piano4,tol));


}
//****************************************************************

//TEST 8: testiamo la funzione ComputeTrace. Caso poligoni disgiunti.
TEST(FindTracesTest, CasoTraccia1){

    FractureStruct fract;
    TracesStruct trac;
    unsigned int n1 = 0, n2 = 1;

    fract.NumeroVertici = {4,4};
    fract.CoordinateVertici = {MatrixXd(3, 4), MatrixXd(3, 4)};
    fract.CoordinateVertici[0] << 0, 1, 1, 0,
                                  0, 0, 1, 1,
                                  0, 0, 0, 0;
    fract.CoordinateVertici[1] << 2, 3, 3, 2,
                                  2, 2, 3, 3,
                                  0, 0, 0, 0;

    Matrix<double,2,3> intersectionLine;
    intersectionLine << 0.5, 0, 0,
                        0, 1, 0;

    bool result = FindTrace(fract, trac, intersectionLine, n1, n2, tol);
    ASSERT_FALSE(result);

}
//****************************************************************

//TEST 9: testiamo la funzione ComputeTrace. Caso traccia passante per la prima frattura ma non per la seconda.
TEST(FindTracesTest, CasoTraccia2){

    FractureStruct fract;
    TracesStruct trac;
    unsigned int n1 = 0, n2 = 1;
    fract.NumeroVertici = {4,4};

    fract.CoordinateVertici = {MatrixXd(3, 4), MatrixXd(3, 4)};
    fract.CoordinateVertici[0] << 0, 1, 1, 0,
                                  0, 0, 2, 2,
                                  0, 0, 0, 0;

    fract.CoordinateVertici[1] <<0.5, 0.5, 0.5, 0.5,
                                 0.5, 0.5, 1.5, 1.5,
                                 -1, 1, 1, -1;

    Matrix<double,2,3> intersectionLine;
    intersectionLine << 0.5, 1.5, 0,
                        0.5, 0.5, 0;

    bool result = FindTrace(fract, trac, intersectionLine, n1, n2, tol);
    ASSERT_TRUE(result);

}
//****************************************************************

//TEST 10: testiamo la funzione ComputeTrace. Caso traccia passante per la seconda frattura ma non per la prima.
TEST(FindTracesTest, CasoTraccia3){

    FractureStruct fract;
    TracesStruct trac;
    unsigned int n1 = 0, n2 = 1;
    fract.NumeroVertici = {4,4};

    fract.CoordinateVertici = {MatrixXd(3, 4), MatrixXd(3, 4)};
    fract.CoordinateVertici[0] << 0, 1, 1, 0,
                                  0, 0, 2, 2,
                                  0, 0, 0, 0;

    fract.CoordinateVertici[1] <<0.5, 0.5, 0.5, 0.5,
                                 -0.5, -0.5, 2.5, 2.5,
                                 -1, 1, 1, -1;

    Matrix<double,2,3> intersectionLine;
    intersectionLine << 0.5, 1.5, 0,
                        0.5, 0.5, 0;

    bool result = FindTrace(fract, trac, intersectionLine, n1, n2, tol);
    ASSERT_TRUE(result);

}
//****************************************************************

//TEST 11: testiamo la funzione ComputeTrace. Caso traccia non passante per entrambe.
TEST(FindTracesTest, CasoTraccia4){

    FractureStruct fract;
    TracesStruct trac;
    unsigned int n1 = 0, n2 = 1;
    fract.NumeroVertici = {4,4};

    fract.CoordinateVertici = {MatrixXd(3, 4), MatrixXd(3, 4)};
    fract.CoordinateVertici[0] << 0, 1, 1, 0,
                                  0, 0, 2, 2,
                                  0, 0, 0, 0;
    fract.CoordinateVertici[1] <<0.5, 0.5, 0.5, 0.5,
                                -0.5, -0.5, 1, 1,
                                -1, 1, 1, -1;

    Matrix<double,2,3> intersectionLine;
    intersectionLine << 0.5, 1.5, 0,
                        0.5, 0.5, 0;

    bool result = FindTrace(fract, trac, intersectionLine, n1, n2, tol);
    ASSERT_TRUE(result);

}

//****************************************************************

//TEST 12: testiamo la funzione ComputeTrace. Caso traccia passante per entrambe.
TEST(FindTracesTest, CasoTraccia5){

    FractureStruct fract;
    TracesStruct trac;
    unsigned int n1 = 0, n2 = 1;
    fract.NumeroVertici = {4,4};

    fract.CoordinateVertici = {MatrixXd(3, 4), MatrixXd(3, 4)};
    fract.CoordinateVertici[0] << 0, 1, 1, 0,
                                  0, 0, 2, 2,
                                  0, 0, 0, 0;
    fract.CoordinateVertici[1] <<0.5, 0.5, 0.5, 0.5,
                                -0.5, -0.5, 1, 1,
                                -1, 1, 1, -1;

    Matrix<double,2,3> intersectionLine;
    intersectionLine << 0.5, 1.5, 0,
                        0.5, 0.5, 0;

    bool result = FindTrace(fract, trac, intersectionLine, n1, n2, tol);
    ASSERT_TRUE(result);

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


