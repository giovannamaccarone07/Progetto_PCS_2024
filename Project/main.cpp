#include <iostream>
#include <Eigen/Eigen>
#include "Fratture.hpp"
#include "Utils.hpp"


using namespace std;
using namespace Eigen;
using namespace FractureLibrary;

int main()
{
    FractureStruct fract;

    string NomeFile = "FR3_data.txt";

    // Verifica che l'importo della mesh e tutti i test siano andati a buon fine
    if(!ImportaDati(NomeFile, fract))
    {
        cerr << "Impossibile importare i dati" << endl;

        return 1;
    }
    else
    {
        cout << "Dati importati correttamente" << endl;
    }

    // Provo a stampare le coordinate delle fratture
    cout << "Ci sono " << fract.NumeroFratture << " fratture" << endl;
    cout << endl;
    for (unsigned int numb = 0; numb < fract.NumeroFratture; numb++)
    {
        cout << "La Frattura numero: " << numb << " ha: " << fract.NumeroVertici[numb] << " vertici" << endl;
        //vector<unsigned int> indici_frattura = fract.IndiciVertici[numb];
        for (unsigned int i = 0; i < fract.NumeroVertici[numb]; i++)
        {
            cout << "Id vertice: " << fract.IndiciVertici[numb][i];
            cout << " le coordinate sono: " <<"( "<< fract.CoordinateVertici[fract.IndiciVertici[numb][i]][0];
            cout <<" , "<< fract.CoordinateVertici[fract.IndiciVertici[numb][i]][1];
            cout <<" , "<< fract.CoordinateVertici[fract.IndiciVertici[numb][i]][2] << " )" << endl;;
        }
        cout << endl;
    }


    return 0;

}

