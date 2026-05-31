#pragma once
#include "ConexionDB.h"
#include "Producto.h"
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
using namespace std;

struct DetalleCompra {
    int id_producto;
    string nombre_producto;
    int cantidad;
    double precio_costo_unitario;
    double subtotal;
};

class Compra {
private:
    int id_compra, no_orden_compra, id_proveedor;
    string fecha_orden;
    vector<DetalleCompra> detalles;
    double total;

    string generarFecha() {
        time_t t = time(0);
        struct tm now;
        localtime_s(&now, &t);
        char fecha[11];
        strftime(fecha, sizeof(fecha), "%Y-%m-%d", &now);
        return string(fecha);
    }

    int generarNoOrden() {
        try {
            ConexionDB db;
            if (!db.conectar()) return 1;
            auto resultado = db.getSession()->sql(
                "SELECT IFNULL(MAX(no_orden_compra), 0) + 1 FROM supermercado.compras"
            ).execute();
            auto fila = resultado.fetchOne();  // sin db.desconectar() antes
            return (int)fila[0];
        }
        catch (const exception& e) {
            return 1;
        }
    }

public:
    Compra() { id_compra = 0; no_orden_compra = 0; id_proveedor = 0; total = 0; }

    void registrarCompra() {
        total = 0;
        detalles.clear();

        cout << "\nIngrese ID del proveedor: ";
        cin >> id_proveedor;

        if (id_proveedor <= 0) {
            cout << "Error: ID de proveedor invalido." << endl;
            return;
        }

        // Verificar proveedor - CORREGIDO
        try {
            ConexionDB db;
            if (!db.conectar()) return;
            auto res = db.getSession()->sql(
                "SELECT proveedor FROM supermercado.proveedores WHERE id_proveedor = ?"
            ).bind(id_proveedor).execute();

            auto fila = res.fetchOne();  // directo sin count()
            if (fila.isNull()) {
                cout << "Error: Proveedor no encontrado." << endl;
                return;
            }
            cout << "Proveedor: " << fila[0] << endl;
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
            return;
        }

        // Ingresar productos
        char continuar = 's';
        while (continuar == 's' || continuar == 'S') {
            int id_prod, cantidad;
            double precio_costo;

            cout << "\nIngrese ID del producto: ";
            if (!(cin >> id_prod)) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Error: Ingrese un ID valido." << endl;
                continue;
            }

            Producto p;
            if (p.buscarPorId(id_prod)) {
                cout << "Ingrese cantidad: ";
                cin >> cantidad;
                cout << "Ingrese precio de costo unitario: Q";
                cin >> precio_costo;

                if (cantidad <= 0) {
                    cout << "Error: La cantidad debe ser mayor a 0." << endl;
                }
                else if (precio_costo <= 0) {
                    cout << "Error: El precio debe ser mayor a 0." << endl;
                }
                else {
                    DetalleCompra d;
                    d.id_producto = p.getId();
                    d.nombre_producto = p.getProducto();
                    d.cantidad = cantidad;
                    d.precio_costo_unitario = precio_costo;
                    d.subtotal = cantidad * precio_costo;
                    total += d.subtotal;
                    detalles.push_back(d);
                    cout << "Producto agregado. Total acumulado: Q" << fixed << setprecision(2) << total << endl;
                }
            }
            cout << "Agregar otro producto? (s/n): ";
            cin >> continuar;
        }

        if (detalles.empty()) {
            cout << "Error: No se agregaron productos." << endl;
            return;
        }

        no_orden_compra = generarNoOrden();
        fecha_orden = generarFecha();

        try {
            ConexionDB db;
            if (!db.conectar()) return;

            db.getSession()->sql("START TRANSACTION").execute();

            db.getSession()->sql(
                "INSERT INTO supermercado.compras (no_orden_compra, id_proveedor, fecha_orden) VALUES (?, ?, ?)"
            ).bind(no_orden_compra, id_proveedor, fecha_orden).execute();

            auto res = db.getSession()->sql("SELECT LAST_INSERT_ID()").execute();
            auto fila = res.fetchOne();
            id_compra = (int)fila[0];

            for (DetalleCompra d : detalles) {
                db.getSession()->sql(
                    "INSERT INTO supermercado.compras_detalle (id_compra, id_producto, cantidad, precio_costo_unitario) VALUES (?, ?, ?, ?)"
                ).bind(id_compra, d.id_producto, d.cantidad, d.precio_costo_unitario).execute();

                db.getSession()->sql(
                    "UPDATE supermercado.productos SET existencia = existencia + ? WHERE id_producto = ?"
                ).bind(d.cantidad, d.id_producto).execute();
            }

            db.getSession()->sql("COMMIT").execute();

            cout << "\n========================================" << endl;
            cout << "         ORDEN DE COMPRA" << endl;
            cout << "========================================" << endl;
            cout << "Orden No: " << setfill('0') << setw(6) << no_orden_compra << endl;
            cout << "Fecha: " << fecha_orden << endl;
            cout << "----------------------------------------" << endl;
            cout << left << setw(6) << "COD" << setw(25) << "PRODUCTO" << setw(6) << "CANT" << setw(10) << "P.COSTO" << "SUBTOTAL" << endl;
            cout << "----------------------------------------" << endl;
            for (DetalleCompra d : detalles) {
                cout << left << setw(6) << d.id_producto << setw(25) << d.nombre_producto
                    << setw(6) << d.cantidad << "Q" << setw(9) << fixed << setprecision(2)
                    << d.precio_costo_unitario << "Q" << d.subtotal << endl;
            }
            cout << "----------------------------------------" << endl;
            cout << right << setw(40) << "TOTAL: Q" << fixed << setprecision(2) << total << endl;
            cout << "========================================" << endl;
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
            try {
                ConexionDB db;
                db.conectar();
                db.getSession()->sql("ROLLBACK").execute();
            }
            catch (...) {}
        }
    }

    void leer() {
        try {
            ConexionDB db;
            if (!db.conectar()) return;
            auto resultado = db.getSession()->sql(
                "SELECT c.id_compra, c.no_orden_compra, c.fecha_orden, p.proveedor "
                "FROM supermercado.compras c "
                "INNER JOIN supermercado.proveedores p ON c.id_proveedor = p.id_proveedor"
            ).execute();
            cout << "\n===== LISTA DE COMPRAS =====" << endl;
            cout << "ID\tORDEN\t\tFECHA\t\tPROVEEDOR" << endl;
            cout << "--------------------------------------------------------------------------------" << endl;
            for (mysqlx::Row fila : resultado.fetchAll()) {
                cout << fila[0] << "\t" << fila[1] << "\t\t" << fila[2] << "\t" << fila[3] << endl;
            }
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
};