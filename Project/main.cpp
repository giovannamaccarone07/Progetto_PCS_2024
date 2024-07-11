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
    TracesStruct trac;
    double tol = 10e-10;
    string NomeFile = "FR200_data.txt";

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

    ///**************************************************************************************
    /*
    // Provo a stampare le coordinate delle fratture
    cout << "Ci sono " << fract.NumeroFratture << " fratture" << endl;
    cout << endl;

    for (unsigned int numb = 0; numb < fract.NumeroFratture; numb++)
    {
        cout << "La Frattura numero: " << numb << " ha: " << fract.NumeroVertici[numb] << " vertici" << endl;
        for (unsigned int i = 0; i < fract.NumeroVertici[numb]; i++)
        {
            cout << "Id vertice: " << fract.IndiciVertici[numb][i]; //come era prima
            //cout << "Id vertice: " << i + ;
            cout << " le coordinate sono: " <<"( "<< fract.CoordinateVertici[numb](0,i);
            cout <<" , "<< fract.CoordinateVertici[numb](1,i);
            cout <<" , "<< fract.CoordinateVertici[numb](2,i) << " )" << endl;;
        }
        cout << endl;
    }
    */
    ///**************************************************************************************
    /*
    bool result = checkIntersezione(fract,trac,0,1,tol);
    if(result == false)
        cout << "Main: non c'è intersezione" << endl;
    else
        cout << "Main: c'é intersezione" << endl;


    if(Output(trac, fract) == false)
    {
        cerr << "Impossibile stampare i dati" << endl;

        return 2;
    }
    else
    {
        cout << "Dati stampati correttamente" << endl;
    }
    */
   ///**************************************************************************************

    for (unsigned int i =0; i<fract.NumeroFratture; i++)
    {
        for (unsigned int j =i+1; j<fract.NumeroFratture; j++)
        {
            bool result = checkIntersezione(fract,trac,i,j,tol);
            if(result == false)
                cout << "Main: NON c'e' intersezione tra: " <<i<< " e "<<j << endl;
            else
                cout << "Main: c'e' intersezione tra: " <<i<< " e " <<j << endl;
        }
    }

    if(Output(trac, fract) == false)
    {
        cerr << "Impossibile stampare i dati" << endl;

        return 2;
    }
    else
    {
        cout << "Dati stampati correttamente" << endl;
    }


    ///*****************************************************************************
    /// Metabolizziamo alcune informazioni prima di passarle a subpolygons in modo da renderla ricorsiva
    unsigned int n = 1;



    Vector3d normale = fract.NormaleFrattura[n];

    list<unsigned int> listaTracce = fract.NumeroTracceP[n];
    vector<Matrix<double,2,3>> coordEstremiTracce;

    if(listaTracce.empty())
    {
        cerr << "nessuna traccia P per frattura: " << n<< endl;
    }
    else
    {
        while(!listaTracce.empty())
        {
            coordEstremiTracce.push_back(trac.EstremiTracce[listaTracce.front()]);
            listaTracce.pop_front();
        }

        list<Vector3d> verticiPolygons;
        for(unsigned int c = 0; c < fract.CoordinateVertici[n].cols(); c++)
        {
            verticiPolygons.push_back(fract.CoordinateVertici[n].col(c));
        }


        bool taglio = subPolygons(verticiPolygons, coordEstremiTracce, normale, tol);

        if (taglio == true)
            cout<< "taglio a buon fine"<<endl;

    }




    return 0;
}


