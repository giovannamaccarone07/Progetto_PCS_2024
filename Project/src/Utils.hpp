#pragma once

#include <iostream>
#include "Fratture.hpp"

using namespace std;
namespace FractureLibrary
{

/// ImportaDati
// NomeFile: prende come riferimento costante il path del file
// Fract: prende la struct di tipo FractureStruct come riferimento e può modificarla
// Funzione che importa i dati da file, restituisce TRUE se la lettura è andata a buon fine
//
bool ImportaDati(const string& NomeFile, FractureStruct& fract);


/// PianoPassantePerFrattura
//
//
MatrixXd RettaIntersezione(const FractureStruct& fract, unsigned int n1, unsigned n2);

Vector4d PianoPassantePerFrattura(const FractureStruct& fract, unsigned int n);


}

