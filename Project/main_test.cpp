#include "TestFratture.hpp"
#include "UCD_test.hpp"
#include <iostream>
#include <Eigen/Eigen>
#include "Fratture.hpp"
#include "Utils.hpp"
#include "UCDUtilities.hpp"





using namespace std;
using namespace Eigen;
using namespace FractureLibrary;


int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    FractureStruct fract;


    string NomeFile = "FR200_data.txt";

    bool import = ImportData(NomeFile, fract);
    if(import == false)
    {

       cerr << "Impossibile importare i dati" << endl;
    }
    else
    {
        cout << "Dati importati correttamente" << endl;
    }



///*************************************************************************************
/// PARAVIEW OUTPUT


    unsigned int c1 = 0;
    MatrixXd points1(3, 4*fract.NumeroFratture);
    for (unsigned int i = 0; i<fract.NumeroFratture; i++)
    {
        points1.col(c1) = fract.CoordinateVertici[i].col(0);
        points1.col(c1+1) = fract.CoordinateVertici[i].col(1);
        points1.col(c1+2) = fract.CoordinateVertici[i].col(2);
        points1.col(c1+3) = fract.CoordinateVertici[i].col(3);
        c1 = c1+4;
    }

    Eigen::VectorXi materials(2);
    materials << 0, 1;

    Gedim::UCDUtilities exporter1;

    exporter1.ExportPolygons("./Fractures2Ds.inp",
                            points1,
                            fract.IndiciVertici,
                            {},
                            {},
                            materials);

///
///**************************************************************************************




    return RUN_ALL_TESTS();
}

