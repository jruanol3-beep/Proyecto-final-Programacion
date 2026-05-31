#pragma once
#include "ConexionDB.h"
#include <iostream>
#include <string>
using namespace std;

class Puesto {
private:
    int id_puesto;
    string puesto;

    bool validarPuesto(string p) {
        if (p.empty()) {
            cout << "Error: El puesto no puede estar vacio." << endl;
            return false;
        }
        if (p.length() > 50) {
            cout << "Error: El puesto debe tener maximo 50 caracteres." << endl;
            return false;
        }
        return true;
    }

public:
    Puesto() { id_puesto = 0; }
    Puesto(int id, string p) {
        id_puesto = id;
        puesto = p;
    }

    void setId(int id) { id_puesto = id; }
    void setPuesto(string p) { puesto = p; }
    int getId() { return id_puesto; }
    string getPuesto() { return puesto; }

    void crear() {
        if (!validarPuesto(puesto)) return;
        try {
            ConexionDB db;
            if (!db.conectar()) return;
            db.getSession()->sql(
                "INSERT INTO supermercado.puestos (puesto) VALUES (?)"
            ).bind(puesto).execute();
            cout << "Puesto creado exitosamente." << endl;
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
                "SELECT id_puesto, puesto FROM supermercado.puestos"
            ).execute();
            cout << "\n===== LISTA DE PUESTOS =====" << endl;
            cout << "ID\tPUESTO" << endl;
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
        if (!validarPuesto(puesto)) return;
        try {
            ConexionDB db;
            if (!db.conectar()) return;
            db.getSession()->sql(
                "UPDATE supermercado.puestos SET puesto = ? WHERE id_puesto = ?"
            ).bind(puesto, id_puesto).execute();
            cout << "Puesto actualizado exitosamente." << endl;
            db.desconectar();
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    void borrar() {
        if (id_puesto <= 0) {
            cout << "Error: ID invalido." << endl;
            return;
        }
        try {
            ConexionDB db;
            if (!db.conectar()) return;
            db.getSession()->sql(
                "DELETE FROM supermercado.puestos WHERE id_puesto = ?"
            ).bind(id_puesto).execute();
            cout << "Puesto eliminado exitosamente." << endl;
            db.desconectar();
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
};