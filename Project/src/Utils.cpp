#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <limits>
using namespace std;

namespace FractureLibrary{

bool ImportDati(const string& NomeFile, FractureStruct& fract){

    ifstream file;
    file.open(NomeFile);
    if(file.fail()){
        return false;
    }
    string riga;

    //leggo prima riga
    getline(file, riga);
    //leggo la seconda
    getline(file,riga);
    istringstream convertElemento(riga);
    unsigned int n_fratture;
    convertElemento >> n_fratture;

    list<string> listaRighe;
    fract.SpazioMemoria = listaRighe.size();
    fract.IdFratture.reserve(fract.SpazioMemoria);
    fract.CoordinateVertici.reserve(fract.SpazioMemoria);


    while(!file.eof()){
        //leggo le righe in gruppi di 6
        for(unsigned int i = 0; i<6; i++){
            getline(file, riga);

            getline(file, riga);
            istringstream convertElemento(riga);
            char separatore;
            unsigned int id_fratture, n_vertici;
            convertElemento >> id_fratture >> separatore >> n_vertici;

            getline(file, riga);

            getline(file, riga);
            fract.IdFratture.push_back(id_fratture);




        }

    }














    while(getline(file,riga)){
        listaRighe.push_back(riga);
    }
    file.close();
    unsigned int NumeroRighe = listaRighe.size();
    if (NumeroRighe == 0)
    {
        cerr << "Il file è vuoto." << endl;
        return false;
    }
    /*
    for (const string& riga : listaRighe)
    {
        istringstream converter(riga);
        char cancelletto;
        unsigned int id, n_vertici, n_fratture;
        Vector3d coordinate;

        if(riga[0] == '#'){
            break;
        }
*/


}


























}
