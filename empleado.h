#pragma once
#include "ConexionDB.h"
#include <iostream>
#include <string>
#include <regex>
#include <ctime>
using namespace std;

class Empleado {
private:
    int id_empleado;
    string nombres, apellidos, direccion, telefono, fecha_nacimiento, fecha_inicio_labores;
    int genero, id_puesto;

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

    bool validarFecha(string fecha) {
        if (fecha.empty()) {
            cout << "Error: La fecha no puede estar vacia." << endl;
            return false;
        }
        regex patron("\\d{4}-\\d{2}-\\d{2}");
        if (!regex_match(fecha, patron)) {
            cout << "Error: La fecha debe tener formato AAAA-MM-DD." << endl;
            return false;
        }
        int mes = stoi(fecha.substr(5, 2));
        int dia = stoi(fecha.substr(8, 2));
        if (mes < 1 || mes > 12) {
            cout << "Error: Mes invalido." << endl;
            return false;
        }
        if (dia < 1 || dia > 31) {
            cout << "Error: Dia invalido." << endl;
            return false;
        }
        return true;
    }

public:
    Empleado() { id_empleado = 0; genero = 0; id_puesto = 0; }
    Empleado(int id, string nom, string ape, string dir, string tel, int gen, string fn, int ip, string fil) {
        id_empleado = id;
        nombres = nom;
        apellidos = ape;
        direccion = dir;
        telefono = tel;
        genero = gen;
        fecha_nacimiento = fn;
        id_puesto = ip;
        fecha_inicio_labores = fil;
    }

    void setId(int id) { id_empleado = id; }
    void setNombres(string n) { nombres = n; }
    void setApellidos(string a) { apellidos = a; }
    void setDireccion(string d) { direccion = d; }
    void setTelefono(string t) { telefono = t; }
    void setGenero(int g) { genero = g; }
    void setFechaNacimiento(string f) { fecha_nacimiento = f; }
    void setIdPuesto(int ip) { id_puesto = ip; }
    void setFechaInicioLabores(string f) { fecha_inicio_labores = f; }
    int getId() { return id_empleado; }
    string getNombres() { return nombres; }
    string getApellidos() { return apellidos; }

    void crear() {
        if (!validarNombre(nombres, "Nombres")) return;
        if (!validarNombre(apellidos, "Apellidos")) return;
        if (!validarTexto(direccion, "Direccion", 80)) return;
        if (!validarTexto(telefono, "Telefono", 25)) return;
        if (genero < 0 || genero > 1) {
            cout << "Error: Genero invalido. Use 0=Femenino, 1=Masculino." << endl;
            return;
        }
        if (!validarFecha(fecha_nacimiento)) return;
        if (id_puesto <= 0) {
            cout << "Error: ID de puesto invalido." << endl;
            return;
        }
        if (!validarFecha(fecha_inicio_labores)) return;
        try {
            ConexionDB db;
            if (!db.conectar()) return;
            db.getSession()->sql(
                "INSERT INTO supermercado.empleados (nombres, apellidos, direccion, telefono, genero, fecha_nacimiento, id_puesto, fecha_inicio_labores) VALUES (?, ?, ?, ?, ?, ?, ?, ?)"
            ).bind(nombres, apellidos, direccion, telefono, genero, fecha_nacimiento, id_puesto, fecha_inicio_labores).execute();
            cout << "Empleado creado exitosamente." << endl;
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
                "SELECT e.id_empleado, e.nombres, e.apellidos, e.telefono, p.puesto, e.fecha_inicio_labores "
                "FROM supermercado.empleados e "
                "INNER JOIN supermercado.puestos p ON e.id_puesto = p.id_puesto"
            ).execute();
            cout << "\n===== LISTA DE EMPLEADOS =====" << endl;
            cout << "ID\tNOMBRES\t\t\tAPELLIDOS\t\tTELEFONO\tPUESTO\t\tINICIO LABORES" << endl;
            cout << "--------------------------------------------------------------------------------" << endl;
            for (mysqlx::Row fila : resultado.fetchAll()) {
                cout << fila[0] << "\t" << fila[1] << "\t\t" << fila[2] << "\t\t" << fila[3] << "\t" << fila[4] << "\t" << fila[5] << endl;
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
        if (!validarTexto(direccion, "Direccion", 80)) return;
        if (!validarTexto(telefono, "Telefono", 25)) return;
        if (!validarFecha(fecha_nacimiento)) return;
        if (id_puesto <= 0) {
            cout << "Error: ID de puesto invalido." << endl;
            return;
        }
        if (!validarFecha(fecha_inicio_labores)) return;
        try {
            ConexionDB db;
            if (!db.conectar()) return;
            db.getSession()->sql(
                "UPDATE supermercado.empleados SET nombres=?, apellidos=?, direccion=?, telefono=?, genero=?, fecha_nacimiento=?, id_puesto=?, fecha_inicio_labores=? WHERE id_empleado=?"
            ).bind(nombres, apellidos, direccion, telefono, genero, fecha_nacimiento, id_puesto, fecha_inicio_labores, id_empleado).execute();
            cout << "Empleado actualizado exitosamente." << endl;
            db.desconectar();
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    void borrar() {
        if (id_empleado <= 0) {
            cout << "Error: ID invalido." << endl;
            return;
        }
        try {
            ConexionDB db;
            if (!db.conectar()) return;
            db.getSession()->sql(
                "DELETE FROM supermercado.empleados WHERE id_empleado=?"
            ).bind(id_empleado).execute();
            cout << "Empleado eliminado exitosamente." << endl;
            db.desconectar();
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
};