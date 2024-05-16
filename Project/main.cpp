#include <iostream>

#include "Fratture.hpp"
#include "Utils.hpp"

using namespace std;
using namespace Eigen;
using namespace FractureLibrary;

int main()
{
    FractureStruct fract;

    string NomeFile = "DFN";

    // Verifica che l'importo della mesh e tutti i test siano andati a buon fine
    if(!ImportaStruttura(NomeFile,
                     fract))
    {
        cerr << "Impossibile importare i dati" << endl;
        return 1;
    }
    else
    {
        cout << "Dati importati correttamente" << endl;
    }



    return 0;

}
