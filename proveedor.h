#pragma once
#include "ConexionDB.h"
#include <iostream>
#include <string>
using namespace std;

class Proveedor {
private:
    int id_proveedor;
    string proveedor, nit, direccion, telefono;

    bool validarTexto(string texto, string campo, int maxLen) {
        if (texto.empty()) {
            cout << "Error: " << campo << " no puede estar vacio." << endl;
            return false;
        }
        if ((int)texto.length() > maxLen) {
            cout << "Error: " << campo << " debe tener maximo " << maxLen << " caracteres." << endl;
            return false;
        }
        return true;
    }

    bool validarNIT(string n) {
        if (n.empty()) {
            cout << "Error: El NIT no puede estar vacio." << endl;
            return false;
        }
        if (n == "C/F" || n == "c/f") return true;
        if (n.length() > 12) {
            cout << "Error: El NIT debe tener maximo 12 caracteres." << endl;
            return false;
        }
        for (char c : n) {
            if (!isdigit(c) && c != '-') {
                cout << "Error: El NIT solo puede contener numeros y guion." << endl;
                return false;
            }
        }
        return true;
    }

public:
    Proveedor() { id_proveedor = 0; }
    Proveedor(int id, string prov, string n, string dir, string tel) {
        id_proveedor = id;
        proveedor = prov;
        nit = n;
        direccion = dir;
        telefono = tel;
    }

    void setId(int id) { id_proveedor = id; }
    void setProveedor(string p) { proveedor = p; }
    void setNit(string n) { nit = n; }
    void setDireccion(string d) { direccion = d; }
    void setTelefono(string t) { telefono = t; }
    int getId() { return id_proveedor; }
    string getProveedor() { return proveedor; }
    string getNit() { return nit; }
    string getDireccion() { return direccion; }
    string getTelefono() { return telefono; }

    void crear() {
        if (!validarTexto(proveedor, "Proveedor", 60)) return;
        if (!validarNIT(nit)) return;
        if (!validarTexto(direccion, "Direccion", 80)) return;
        if (!validarTexto(telefono, "Telefono", 25)) return;
        try {
            ConexionDB db;
            if (!db.conectar()) return;
            db.getSession()->sql(
                "INSERT INTO supermercado.proveedores (proveedor, nit, direccion, telefono) VALUES (?, ?, ?, ?)"
            ).bind(proveedor, nit, direccion, telefono).execute();
            cout << "Proveedor creado exitosamente." << endl;
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
                "SELECT id_proveedor, proveedor, nit, direccion, telefono FROM supermercado.proveedores"
            ).execute();
            cout << "\n===== LISTA DE PROVEEDORES =====" << endl;
            cout << "ID\tPROVEEDOR\t\tNIT\t\tDIRECCION\t\tTELEFONO" << endl;
            cout << "--------------------------------------------------------------------------------" << endl;
            for (mysqlx::Row fila : resultado.fetchAll()) {
                cout << fila[0] << "\t" << fila[1] << "\t\t" << fila[2] << "\t\t" << fila[3] << "\t\t" << fila[4] << endl;
            }
            db.desconectar();
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    void actualizar() {
        if (!validarTexto(proveedor, "Proveedor", 60)) return;
        if (!validarNIT(nit)) return;
        if (!validarTexto(direccion, "Direccion", 80)) return;
        if (!validarTexto(telefono, "Telefono", 25)) return;
        try {
            ConexionDB db;
            if (!db.conectar()) return;
            db.getSession()->sql(
                "UPDATE supermercado.proveedores SET proveedor=?, nit=?, direccion=?, telefono=? WHERE id_proveedor=?"
            ).bind(proveedor, nit, direccion, telefono, id_proveedor).execute();
            cout << "Proveedor actualizado exitosamente." << endl;
            db.desconectar();
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    void borrar() {
        if (id_proveedor <= 0) {
            cout << "Error: ID invalido." << endl;
            return;
        }
        try {
            ConexionDB db;
            if (!db.conectar()) return;
            db.getSession()->sql(
                "DELETE FROM supermercado.proveedores WHERE id_proveedor=?"
            ).bind(id_proveedor).execute();
            cout << "Proveedor eliminado exitosamente." << endl;
            db.desconectar();
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
};
