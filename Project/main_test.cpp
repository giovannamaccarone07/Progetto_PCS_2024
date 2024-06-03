#include "TestFratture.hpp"
#include "UCD_test.hpp"
#include <iostream>
#include <Eigen/Eigen>
#include "Fratture.hpp"
#include "Utils.hpp"


using namespace std;
using namespace Eigen;
using namespace FractureLibrary;


int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);

    FractureStruct fract;

    string NomeFile = "FR3_data.txt";

    bool import = ImportaDati(NomeFile, fract);
    if(import == false)
    {

       cerr << "Impossibile importare i dati" << endl;
    }
    else
    {
        cout << "Dati importati correttamente" << endl;
    }

    bool result = checkIntersezione(fract,0,1);
    if(result == false)
        cout << "Main: non c'è intersezione" << endl;
    else
        cout << "Main: c'é intersezione" << endl;



    return RUN_ALL_TESTS();
}

