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
