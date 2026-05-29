#pragma once
#include "ConexionDB.h"
#include <iostream>
#include <string>
using namespace std;

class Marca {
private:
    int id_marca;
    string marca;

    bool validarMarca(string m) {
        if (m.empty()) {
            cout << "Error: La marca no puede estar vacia." << endl;
            return false;
        }
        if (m.length() > 50) {
            cout << "Error: La marca debe tener maximo 50 caracteres." << endl;
            return false;
        }
        return true;
    }

public:
    Marca() { id_marca = 0; }

    Marca(int id, string m) {
        id_marca = id;
        marca = m;
    }

    void setId(int id) { id_marca = id; }
    void setMarca(string m) { marca = m; }

    int getId() { return id_marca; }
    string getMarca() { return marca; }
};
