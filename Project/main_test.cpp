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
    TracesStruct trac;

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


    unsigned int c = 0;
    MatrixXd points(3, 4*fract.NumeroFratture);
    for (unsigned int i = 0; i<fract.NumeroFratture; i++)
    {
        points.col(c) = fract.CoordinateVertici[i].col(0);
        points.col(c+1) = fract.CoordinateVertici[i].col(1);
        points.col(c+2) = fract.CoordinateVertici[i].col(2);
        points.col(c+3) = fract.CoordinateVertici[i].col(3);
        c = c+4;
    }

    Eigen::VectorXi materials(2);
    materials << 0, 1;

    Gedim::UCDUtilities exporter;

    exporter.ExportPolygons("./Fractures2Ds.inp",
                            points,
                            fract.IndiciVertici,
                            {},
                            {},
                            materials);














    return RUN_ALL_TESTS();
}

