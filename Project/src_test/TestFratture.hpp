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
TEST(ImportaDatiTest, NumeroVertici)
{
    FractureStruct fract;
    ImportaDati("FR3_data.txt",fract);
    unsigned int vertici = fract.NumeroVertici[0]; // controllo che la prima frattura abbia 4 vertici
    ASSERT_EQ(4,vertici);

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


