#include <iostream>
#include <Eigen/Eigen>
#include "Fratture.hpp"
#include "Utils.hpp"


using namespace std;
using namespace Eigen;
using namespace FractureLibrary;

int main()
{
    PolygonalMesh mesh;


    FractureStruct fract;
    TracesStruct trac;
    double tol = 10e-10;
    string NomeFile = "FR200_data.txt";

    // Verifica che l'importo della mesh e tutti i test siano andati a buon fine
    if(!ImportData(NomeFile, fract))
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
    unsigned int n = 3; // era 3

    list<MatrixXd> sp = {};

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


        bool taglio = subPolygons(verticiPolygons, coordEstremiTracce,sp, normale, tol);

        if (taglio == true)
            cout<< "taglio a buon fine"<<endl;
    }




    unsigned int id = 0;
    mesh.NumberCell2D = sp.size();
    mesh.Cell0DId.reserve(sp.size());
    mesh.Cell1DId.reserve(sp.size());
    mesh.Cell0DCoordinates.reserve(sp.size());
    mesh.Cell1DVertices.reserve(sp.size());
    mesh.Cell2DVertices.reserve(sp.size());
    mesh.Cell2DEdges.reserve(sp.size());
    mesh.NumeroDiLati.reserve(sp.size());
    mesh.NumeroDiVertici.reserve(sp.size());


    auto itor = sp.begin();
    while(itor != sp.end())
    {
        MatrixXd matrice= (*itor);
        unsigned int mc =matrice.cols();
        mesh.NumberCell0D = mc;
        mesh.NumberCell1D = mc;
        mesh.NumberCell2D ++;
        mesh.NumeroDiLati.push_back(mc);
        mesh.NumeroDiVertici.push_back(mc);

        VectorXi lati = {};
        VectorXi vertici = {};
        for (unsigned int c= 0; c<mc; c++)
        {
            mesh.Cell0DId.push_back(id);
            mesh.Cell0DCoordinates.push_back(matrice.col(c));
            Vector2i edge(id%(mc), (id+1)%(mc));
            mesh.Cell1DVertices.push_back(edge);
            mesh.Cell1DId.push_back(id);
            vertici[c] = id;
            lati[c]= id;
            id ++;
        }

        mesh.Cell2DEdges.push_back(lati);
        mesh.Cell2DVertices.push_back(vertici);

        itor++;
    }




    return 0;
}


