#pragma once

#include <iostream>
#include "Fratture.hpp"

using namespace std;
namespace FractureLibrary
{
bool ImportaDati(const string& NomeFile, FractureStruct& Fract);         // Funzione che importa i dati da file
bool ImportaStruttura(const string& NomeFile, FractureStruct& Fract);    // Funzione che apre con i dati ottenuti da ImportaDati dovrebbe associare gli indici dei vertici alle corrispondenti coordinate

}

