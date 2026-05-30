#pragma once
#include "ConexionDB.h"
#include <iostream>
#include <string>
#include <regex>
using namespace std;

class Cliente {
private:
    int id_cliente;
    string nombres, apellidos, nit, telefono, correo_electronico;
    int genero;

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

    bool validarNombre(string nombre, string campo) {
        if (nombre.empty()) {
            cout << "Error: " << campo << " no puede estar vacio." << endl;
            return false;
        }
        if ((int)nombre.length() > 60) {
            cout << "Error: " << campo << " debe tener maximo 60 caracteres." << endl;
            return false;
        }
        for (char c : nombre) {
            if (!isalpha(c) && c != ' ') {
                cout << "Error: " << campo << " solo puede contener letras y espacios." << endl;
                return false;
            }
        }
        return true;
    }

    bool validarNIT(string n) {
        if (n.empty()) {
            cout << "Error: El NIT no puede estar vacio." << endl;
            return false;
        }
        regex patronNit("^([1-9][0-9]{6,10}-[0-9K]|CF|cf|C/F|c/f)$");
        if (!regex_match(n, patronNit)) {
            cout << "Error: NIT invalido. Formato: 123456-K o CF" << endl;
            return false;
        }

        return true;
    }

public:
    Cliente() { id_cliente = 0; genero = 0; }
    Cliente(int id, string nom, string ape, string n, int gen, string tel, string correo) {
        id_cliente = id;
        nombres = nom;
        apellidos = ape;
        nit = n;
        genero = gen;
        telefono = tel;
        correo_electronico = correo;
    }

    void setId(int id) { id_cliente = id; }
    void setNombres(string n) { nombres = n; }
    void setApellidos(string a) { apellidos = a; }
    void setNit(string n) { nit = n; }
    void setGenero(int g) { genero = g; }
    void setTelefono(string t) { telefono = t; }
    void setCorreo(string c) { correo_electronico = c; }
    int getId() { return id_cliente; }
    string getNombres() { return nombres; }
    string getApellidos() { return apellidos; }
    string getNit() { return nit; }
    int getGenero() { return genero; }
    string getTelefono() { return telefono; }
    string getCorreo() { return correo_electronico; }

    int buscarPorNit(string n) {
        try {
            ConexionDB db;
            if (!db.conectar()) return -1;
            auto resultado = db.getSession()->sql(
                "SELECT id_cliente, nombres, apellidos FROM supermercado.clientes WHERE nit = ?"
            ).bind(n).execute();
            db.desconectar();
            if (resultado.count() > 0) {
                auto fila = resultado.fetchOne();
                cout << "Cliente encontrado: " << fila[1] << " " << fila[2] << endl;
                return (int)fila[0];
            }
            return 0;
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
            return -1;
        }
    }

    void crear() {
        if (!validarNombre(nombres, "Nombres")) return;
        if (!validarNombre(apellidos, "Apellidos")) return;
        if (!validarNIT(nit)) return;
        if (genero < 0 || genero > 1) {
            cout << "Error: Genero invalido. Use 0=Femenino, 1=Masculino." << endl;
            return;
        }
        try {
            ConexionDB db;
            if (!db.conectar()) return;
            db.getSession()->sql(
                "INSERT INTO supermercado.clientes (nombres, apellidos, nit, genero, telefono, correo_electronico) VALUES (?, ?, ?, ?, ?, ?)"
            ).bind(nombres, apellidos, nit, genero, telefono, correo_electronico).execute();
            cout << "Cliente creado exitosamente." << endl;
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
                "SELECT id_cliente, nombres, apellidos, nit, telefono, correo_electronico FROM supermercado.clientes"
            ).execute();
            cout << "\n===== LISTA DE CLIENTES =====" << endl;
            cout << "ID\tNOMBRES\t\t\tAPELLIDOS\t\tNIT\t\tTELEFONO" << endl;
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
        if (!validarNombre(nombres, "Nombres")) return;
        if (!validarNombre(apellidos, "Apellidos")) return;
        if (!validarNIT(nit)) return;
        try {
            ConexionDB db;
            if (!db.conectar()) return;
            db.getSession()->sql(
                "UPDATE supermercado.clientes SET nombres=?, apellidos=?, nit=?, genero=?, telefono=?, correo_electronico=? WHERE id_cliente=?"
            ).bind(nombres, apellidos, nit, genero, telefono, correo_electronico, id_cliente).execute();
            cout << "Cliente actualizado exitosamente." << endl;
            db.desconectar();
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    void borrar() {
        if (id_cliente <= 0) {
            cout << "Error: ID invalido." << endl;
            return;
        }
        try {
            ConexionDB db;
            if (!db.conectar()) return;
            db.getSession()->sql(
                "DELETE FROM supermercado.clientes WHERE id_cliente=?"
            ).bind(id_cliente).execute();
            cout << "Cliente eliminado exitosamente." << endl;
            db.desconectar();
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
};