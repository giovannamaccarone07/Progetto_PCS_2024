#include <iostream>

#include "Fratture.hpp"
#include "Utils.hpp"

using namespace std;
using namespace Eigen;
using namespace FractureLibrary;

int main()
{
    FractureStruct fract;

    string NomeFile = "DFN/FR3_data.txt";

    // Verifica che l'importo della mesh e tutti i test siano andati a buon fine
    if(!ImportaDati(NomeFile,
                     fract))
    {
        cerr << "Impossibile importare i dati" << endl;
        return 1;
    }
    else
    {
        cout << "Dati importati correttamente" << endl;
    }

    // Provo a stampare le coordinate della seconda frattura
    VectorXi indici_frattura_1 = fract.IndiciVertici[1];
    unsigned int numero_vertici = indici_frattura_1.size();
    for (unsigned int i = 0; i < numero_vertici; i++)
    {
        Vector3d coord_primo_vertice = fract.CoordinateVertici[indici_frattura_1(i)];
        double coord_x = coord_primo_vertice(0);
        double coord_y = coord_primo_vertice(1);
        double coord_z = coord_primo_vertice(2);
        cout << "La coordinata x e': " << coord_x << endl;
        cout << "La coordinata y e': " << coord_y << endl;
        cout << "La coordinata z e': " << coord_z << endl;
    }



    return 0;

}
