#pragma once
#include "ConexionDB.h"
#include <iostream>
#include <string>
using namespace std;

class Producto {
private:
    int id_producto, id_marca, existencia;
    string producto, descripcion, imagen;
    double precio_costo, precio_venta;

    bool validarTexto(string texto, string campo, int maxLen) {
        if (texto.empty()) {
            cout << "Error: " << campo << " no puede estar vacio." << endl;
            return false;
        }

        if ((int)texto.length() > maxLen) {
            cout << "Error: " << campo << " debe tener maximo "
                 << maxLen << " caracteres." << endl;
            return false;
        }

        return true;
    }

    bool validarPrecio(double precio, string campo) {

        if (precio <= 0) {
            cout << "Error: " << campo
                 << " debe ser mayor a 0." << endl;
            return false;
        }

        return true;
    }

public:

    Producto() {
        id_producto = 0;
        id_marca = 0;
        existencia = 0;
        precio_costo = 0;
        precio_venta = 0;
    }

    Producto(int id, string prod, int im,
        string desc, string img,
        double pc, double pv, int ex) {

        id_producto = id;
        producto = prod;
        id_marca = im;
        descripcion = desc;
        imagen = img;
        precio_costo = pc;
        precio_venta = pv;
        existencia = ex;
    }

    void setId(int id) { id_producto = id; }
    void setProducto(string p) { producto = p; }
    void setIdMarca(int im) { id_marca = im; }
    void setDescripcion(string d) { descripcion = d; }
    void setImagen(string i) { imagen = i; }
    void setPrecioCosto(double pc) { precio_costo = pc; }
    void setPrecioVenta(double pv) { precio_venta = pv; }
    void setExistencia(int e) { existencia = e; }

    int getId() { return id_producto; }
    string getProducto() { return producto; }
    int getIdMarca() { return id_marca; }
    double getPrecioCosto() { return precio_costo; }
    double getPrecioVenta() { return precio_venta; }
    int getExistencia() { return existencia; }
    
void crear() {

    if (!validarTexto(producto, "Producto", 50)) return;

    if (id_marca <= 0) {
        cout << "Error: ID de marca invalido." << endl;
        return;
    }

    if (!validarPrecio(precio_costo, "Precio costo")) return;
    if (!validarPrecio(precio_venta, "Precio venta")) return;

    try {

        ConexionDB db;

        if (!db.conectar()) return;

        db.getSession()->sql(
            "INSERT INTO supermercado.productos "
            "(producto,id_marca,descripcion,imagen,"
            "precio_costo,precio_venta,existencia) "
            "VALUES (?,?,?,?,?,?,?)"
        ).bind(
            producto,
            id_marca,
            descripcion,
            imagen,
            precio_costo,
            precio_venta,
            existencia
        ).execute();

        cout << "Producto creado exitosamente." << endl;

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
            "SELECT p.id_producto,p.producto,m.marca,"
            "p.precio_costo,p.precio_venta,p.existencia "
            "FROM supermercado.productos p "
            "INNER JOIN supermercado.marcas m "
            "ON p.id_marca = m.id_marca"
        ).execute();

        cout << "\n===== PRODUCTOS =====" << endl;

        for (mysqlx::Row fila : resultado.fetchAll()) {

            cout << fila[0] << " | "
                 << fila[1] << " | "
                 << fila[2] << " | "
                 << fila[3] << " | "
                 << fila[4] << " | "
                 << fila[5] << endl;
        }

        db.desconectar();
    }
    catch (const exception& e) {

        cout << "Error: " << e.what() << endl;
    }
}

void actualizar() {

    try {

        ConexionDB db;

        if (!db.conectar()) return;

        db.getSession()->sql(
            "UPDATE supermercado.productos "
            "SET producto=?,id_marca=?,descripcion=?,"
            "imagen=?,precio_costo=?,precio_venta=?,"
            "existencia=? WHERE id_producto=?"
        ).bind(
            producto,
            id_marca,
            descripcion,
            imagen,
            precio_costo,
            precio_venta,
            existencia,
            id_producto
        ).execute();

        cout << "Producto actualizado." << endl;

        db.desconectar();
    }
    catch (const exception& e) {

        cout << "Error: " << e.what() << endl;
    }
}

void borrar() {

    try {

        ConexionDB db;

        if (!db.conectar()) return;

        db.getSession()->sql(
            "DELETE FROM supermercado.productos "
            "WHERE id_producto=?"
        ).bind(id_producto).execute();

        cout << "Producto eliminado." << endl;

        db.desconectar();
    }
    catch (const exception& e) {

        cout << "Error: " << e.what() << endl;
    }
}
bool buscarPorId(int id) {

    try {

        ConexionDB db;

        if (!db.conectar()) return false;

        auto resultado = db.getSession()->sql(
            "SELECT p.id_producto,p.producto,m.marca,"
            "p.precio_venta,p.existencia "
            "FROM supermercado.productos p "
            "INNER JOIN supermercado.marcas m "
            "ON p.id_marca = m.id_marca "
            "WHERE p.id_producto=?"
        ).bind(id).execute();

        db.desconectar();

        if (resultado.count() > 0) {

            auto fila = resultado.fetchOne();

            id_producto = (int)fila[0];
            producto = fila[1].get<string>();
            precio_venta = (double)fila[3];
            existencia = (int)fila[4];

            cout << "Producto: " << fila[1]
                 << " | Marca: " << fila[2]
                 << " | Precio: Q" << fila[3]
                 << " | Existencia: " << fila[4]
                 << endl;

            return true;
        }

        cout << "Producto no encontrado." << endl;

        return false;
    }
    catch (const exception& e) {

        cout << "Error: " << e.what() << endl;

        return false;
    }
}
};
