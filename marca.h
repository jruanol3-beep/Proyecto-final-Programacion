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

    void crear() {
        if (!validarMarca(marca)) return;
        try {
            ConexionDB db;
            if (!db.conectar()) return;
            db.getSession()->sql(
                "INSERT INTO supermercado.marcas (marca) VALUES (?)"
            ).bind(marca).execute();
            cout << "Marca creada exitosamente." << endl;
            db.desconectar();
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    void leer() {
        try {
            ConexionDB db;
            if (!db.conectar()) return;
            auto resultado = db.getSession()->sql(
                "SELECT id_marca, marca FROM supermercado.marcas"
            ).execute();
            cout << "\n===== LISTA DE MARCAS =====" << endl;
            cout << "ID\tMARCA" << endl;
            cout << "------------------------" << endl;
            for (mysqlx::Row fila : resultado.fetchAll()) {
                cout << fila[0] << "\t" << fila[1] << endl;
            }
            db.desconectar();
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    void actualizar() {
        if (!validarMarca(marca)) return;
        try {
            ConexionDB db;
            if (!db.conectar()) return;
            db.getSession()->sql(
                "UPDATE supermercado.marcas SET marca = ? WHERE id_marca = ?"
            ).bind(marca, id_marca).execute();
            cout << "Marca actualizada exitosamente." << endl;
            db.desconectar();
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    void borrar() {
        if (id_marca <= 0) {
            cout << "Error: ID invalido." << endl;
            return;
        }
        try {
            ConexionDB db;
            if (!db.conectar()) return;
            db.getSession()->sql(
                "DELETE FROM supermercado.marcas WHERE id_marca = ?"
            ).bind(id_marca).execute();
            cout << "Marca eliminada exitosamente." << endl;
            db.desconectar();
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
};