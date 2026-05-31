#pragma once
#include "ConexionDB.h"
#include "Cliente.h"
#include "Producto.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <regex>
using namespace std;

struct DetalleVenta {
    int id_producto;
    string nombre_producto;
    int cantidad;
    double precio_unitario;
    double subtotal;
};

class Venta {
private:
    int id_venta, no_factura, id_cliente, id_empleado;
    string serie, fecha_factura;
    vector<DetalleVenta> detalles;
    double total;
    string nit_cliente, nom_cliente, ape_cliente;

    string pedirNombre(string mensaje) {
        string valor;
        do {
            cout << mensaje;
            getline(cin, valor);
            if (valor.empty()) {
                cout << "Error: El campo no puede estar vacio." << endl;
                continue;
            }
            if ((int)valor.length() > 60) {
                cout << "Error: Maximo 60 caracteres." << endl;
                valor = "";
                continue;
            }
            bool valido = true;
            for (char c : valor) {
                if (!isalpha(c) && c != ' ') {
                    cout << "Error: Solo puede contener letras y espacios." << endl;
                    valido = false;
                    valor = "";
                    break;
                }
            }
        } while (valor.empty());
        return valor;
    }

    string pedirTelefono(string mensaje) {
        string valor;
        do {
            cout << mensaje;
            getline(cin, valor);
            if (valor.empty()) {
                cout << "Error: El telefono no puede estar vacio." << endl;
                continue;
            }
            if (valor.length() != 8) {
                cout << "Error: El telefono debe tener exactamente 8 digitos." << endl;
                valor = "";
                continue;
            }
            bool valido = true;
            for (char c : valor) {
                if (!isdigit(c)) {
                    cout << "Error: Solo puede contener numeros." << endl;
                    valido = false;
                    valor = "";
                    break;
                }
            }
        } while (valor.empty());
        return valor;
    }

    string pedirCorreo(string mensaje) {
        string valor;
        regex patron("^[\\w.-]+@[\\w.-]+\\.[a-zA-Z]{2,}$");
        do {
            cout << mensaje;
            getline(cin, valor);
            if (valor.empty()) {
                cout << "Error: El correo no puede estar vacio." << endl;
                continue;
            }
            if (!regex_match(valor, patron)) {
                cout << "Error: Correo invalido. Ejemplo: usuario@correo.com" << endl;
                valor = "";
            }
        } while (valor.empty());
        return valor;
    }

    int pedirGenero() {
        int valor;
        do {
            cout << "Genero (0=Femenino, 1=Masculino): ";
            if (!(cin >> valor)) {
                cout << "Error: Ingrese un numero valido." << endl;
                cin.clear();
                cin.ignore(1000, '\n');
                valor = -1;
            }
            else if (valor != 0 && valor != 1) {
                cout << "Error: Solo se permite 0=Femenino o 1=Masculino." << endl;
                cin.ignore();
                valor = -1;
            }
            else {
                cin.ignore();
            }
        } while (valor == -1);
        return valor;
    }

    bool validarFormatoNIT(string nit) {
        if (nit == "C/F" || nit == "c/f") return true;
        regex patron("^[0-9]{7}-[0-9]$");
        return regex_match(nit, patron);
    }

    int pedirCantidad(int existencia) {
        int valor;
        do {
            cout << "Ingrese cantidad: ";
            if (!(cin >> valor)) {
                cout << "Error: Ingrese un numero valido." << endl;
                cin.clear();
                cin.ignore(1000, '\n');
                valor = 0;
            }
            else if (valor <= 0) {
                cout << "Error: La cantidad debe ser mayor a 0." << endl;
                cin.ignore();
                valor = 0;
            }
            else if (valor > existencia) {
                cout << "Error: No hay suficiente existencia. Disponible: " << existencia << endl;
                cin.ignore();
                valor = 0;
            }
            else {
                cin.ignore();
            }
        } while (valor == 0);
        return valor;
    }

    int pedirIdProducto() {
        int valor = 0;
        do {
            cout << "Ingrese ID del producto: ";
            if (!(cin >> valor)) {
                cout << "Error: Ingrese un numero valido." << endl;
                cin.clear();
                cin.ignore(1000, '\n');
                valor = 0;
            }
            else {
                cin.ignore();
                try {
                    ConexionDB db;
                    if (!db.conectar()) continue;
                    auto res = db.getSession()->sql(
                        "SELECT id_producto FROM supermercado.productos WHERE id_producto = ?"
                    ).bind(valor).execute();
                    db.desconectar();
                    if (res.count() == 0) {
                        cout << "Error: Producto no encontrado." << endl;
                        valor = 0;
                    }
                }
                catch (const exception& e) {
                    cout << "Error: " << e.what() << endl;
                    valor = 0;
                }
            }
        } while (valor == 0);
        return valor;
    }

    string generarFecha() {
        time_t t = time(0);
        struct tm now;
        localtime_s(&now, &t);
        char fecha[11];
        strftime(fecha, sizeof(fecha), "%Y-%m-%d", &now);
        return string(fecha);
    }

    int generarNoFactura() {
        try {
            ConexionDB db;
            if (!db.conectar()) return 1;
            auto resultado = db.getSession()->sql(
                "SELECT IFNULL(MAX(no_factura), 0) + 1 FROM supermercado.ventas"
            ).execute();
            db.desconectar();
            auto fila = resultado.fetchOne();
            return (int)fila[0];
        }
        catch (const exception& e) {
            return 1;
        }
    }

    int buscarIdClientePorNit(string nit) {
        try {
            ConexionDB db;
            if (!db.conectar()) return -1;
            auto resultado = db.getSession()->sql(
                "SELECT id_cliente, nombres, apellidos FROM supermercado.clientes WHERE nit = ?"
            ).bind(nit).execute();
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

    int registrarNuevoCliente(string nit) {
        cout << "Ingrese los datos del cliente:" << endl;
        string nom = pedirNombre("Nombres: ");
        string ape = pedirNombre("Apellidos: ");
        string tel = pedirTelefono("Telefono: ");
        string correo = pedirCorreo("Correo: ");
        int gen = pedirGenero();

        try {
            ConexionDB db;
            if (!db.conectar()) return -1;
            db.getSession()->sql(
                "INSERT INTO supermercado.clientes (nombres, apellidos, nit, genero, telefono, correo_electronico) VALUES (?, ?, ?, ?, ?, ?)"
            ).bind(nom, ape, nit, gen, tel, correo).execute();
            auto res = db.getSession()->sql(
                "SELECT id_cliente FROM supermercado.clientes WHERE nit = ? ORDER BY id_cliente DESC LIMIT 1"
            ).bind(nit).execute();
            db.desconectar();
            if (res.count() > 0) {
                auto fila = res.fetchOne();
                cout << "Cliente registrado exitosamente." << endl;
                return (int)fila[0];
            }
            return -1;
        }
        catch (const exception& e) {
            cout << "Error al registrar cliente: " << e.what() << endl;
            return -1;
        }
    }

    void generarFacturaHTML() {
        stringstream ss;
        ss << setfill('0') << setw(6) << no_factura;
        string noFacturaStr = ss.str();

        string filas = "";
        for (DetalleVenta d : detalles) {
            stringstream precio, subtotal;
            precio << fixed << setprecision(2) << d.precio_unitario;
            subtotal << fixed << setprecision(2) << d.subtotal;
            filas += "<tr>";
            filas += "<td>" + to_string(d.id_producto) + "</td>";
            filas += "<td>" + d.nombre_producto + "</td>";
            filas += "<td style='text-align:center'>" + to_string(d.cantidad) + "</td>";
            filas += "<td style='text-align:right'>Q" + precio.str() + "</td>";
            filas += "<td style='text-align:right'>Q" + subtotal.str() + "</td>";
            filas += "</tr>";
        }

        stringstream totalStr;
        totalStr << fixed << setprecision(2) << total;

        string html = R"(<!DOCTYPE html>
<html lang='es'>
<head>
    <meta charset='UTF-8'>
    <title>Factura )" + noFacturaStr + R"(</title>
    <style>
        body { font-family: Arial, sans-serif; max-width: 650px; margin: 40px auto; padding: 20px; }
        .header { text-align: center; border-bottom: 2px solid #000; padding-bottom: 10px; margin-bottom: 20px; }
        .header h1 { margin: 0; font-size: 26px; letter-spacing: 2px; }
        .header p { margin: 4px 0; font-size: 14px; color: #555; }
        .info { margin-bottom: 20px; font-size: 14px; background: #f5f5f5; padding: 10px; border-radius: 5px; }
        .info p { margin: 4px 0; }
        table { width: 100%; border-collapse: collapse; font-size: 14px; }
        th { background-color: #222; color: white; padding: 10px 8px; text-align: left; }
        th:nth-child(3), th:nth-child(4), th:nth-child(5) { text-align: right; }
        td { padding: 8px; border-bottom: 1px solid #ddd; }
        tr:nth-child(even) { background-color: #f9f9f9; }
        .total-section { text-align: right; margin-top: 15px; padding: 10px 0; border-top: 2px solid #000; }
        .total-section span { font-size: 18px; font-weight: bold; }
        .footer { text-align: center; margin-top: 30px; font-size: 13px; color: #555; border-top: 1px solid #ccc; padding-top: 10px; }
    </style>
    <script>
        window.onload = function() { window.print(); }
    </script>
</head>
<body>
    <div class='header'>
        <h1>SUPERMERCADO XYZ</h1>
        <p>Factura de Venta</p>
    </div>
    <div class='info'>
        <p><strong>Factura No:</strong> )" + noFacturaStr + R"( &nbsp;&nbsp;&nbsp; <strong>Serie:</strong> )" + serie + R"(</p>
        <p><strong>Fecha:</strong> )" + fecha_factura + R"(</p>
        <p><strong>NIT:</strong> )" + nit_cliente + R"(</p>
        <p><strong>Cliente:</strong> )" + nom_cliente + " " + ape_cliente + R"(</p>
    </div>
    <table>
        <thead>
            <tr>
                <th>COD</th>
                <th>PRODUCTO</th>
                <th style='text-align:right'>CANT</th>
                <th style='text-align:right'>PRECIO</th>
                <th style='text-align:right'>SUBTOTAL</th>
            </tr>
        </thead>
        <tbody>
            )" + filas + R"(
        </tbody>
    </table>
    <div class='total-section'>
        <span>TOTAL: Q)" + totalStr.str() + R"(</span>
    </div>
    <div class='footer'>
        <p>Gracias por su compra</p>
    </div>
</body>
</html>)";

        string ruta = "factura_" + noFacturaStr + ".html";
        ofstream archivo(ruta);
        archivo << html;
        archivo.close();

        string cmd = "start " + ruta;
        system(cmd.c_str());
        cout << "Factura generada y abierta en el navegador." << endl;
    }

public:
    Venta() { id_venta = 0; no_factura = 0; id_cliente = 0; id_empleado = 0; total = 0; serie = "A"; }

    void setIdEmpleado(int id) { id_empleado = id; }

    void imprimirFacturaConsola() {
        cout << "\n========================================" << endl;
        cout << "         SUPERMERCADO XYZ" << endl;
        cout << "          Factura de venta" << endl;
        cout << "========================================" << endl;
        cout << "Factura No: " << setfill('0') << setw(6) << no_factura
            << setfill(' ') << "  Serie: " << serie << endl;
        cout << "Fecha: " << fecha_factura << endl;
        cout << "NIT: " << nit_cliente << endl;
        cout << "Cliente: " << nom_cliente << " " << ape_cliente << endl;
        cout << "----------------------------------------" << endl;
        printf("%-5s %-20s %-6s %-10s %s\n", "COD", "PRODUCTO", "CANT", "PRECIO", "SUBTOTAL");
        cout << "----------------------------------------" << endl;
        for (DetalleVenta d : detalles) {
            string nombre = d.nombre_producto;
            if ((int)nombre.length() > 20) nombre = nombre.substr(0, 17) + "...";
            printf("%-5d %-20s %-6d Q%-9.2f Q%.2f\n",
                d.id_producto,
                nombre.c_str(),
                d.cantidad,
                d.precio_unitario,
                d.subtotal);
        }
        cout << "----------------------------------------" << endl;
        printf("%38s Q%.2f\n", "TOTAL:", total);
        cout << "========================================" << endl;
        cout << "       Gracias por su compra" << endl;
        cout << "========================================" << endl;
    }

    void registrarVenta() {
        total = 0;
        detalles.clear();

        do {
            cout << "\nIngrese NIT del cliente (formato: 1234567-8 o C/F): ";
            cin >> nit_cliente;
            cin.ignore();
            if (nit_cliente.empty()) {
                cout << "Error: El NIT no puede estar vacio." << endl;
                nit_cliente = "";
                continue;
            }
            if (!validarFormatoNIT(nit_cliente)) {
                cout << "Error: Formato de NIT invalido. Use 1234567-8 o C/F." << endl;
                nit_cliente = "";
            }
        } while (nit_cliente.empty());

        int id_cli = 0;

        if (nit_cliente == "C/F" || nit_cliente == "c/f") {
            cout << "Consumidor Final - Ingrese los datos del cliente:" << endl;
            id_cli = registrarNuevoCliente(nit_cliente);
        }
        else {
            id_cli = buscarIdClientePorNit(nit_cliente);
            if (id_cli == 0) {
                id_cli = registrarNuevoCliente(nit_cliente);
            }
        }

        if (id_cli <= 0) {
            cout << "Error: No se pudo obtener el cliente." << endl;
            return;
        }

        id_cliente = id_cli;

        try {
            ConexionDB db;
            if (!db.conectar()) return;
            auto res = db.getSession()->sql(
                "SELECT nombres, apellidos FROM supermercado.clientes WHERE id_cliente = ?"
            ).bind(id_cliente).execute();
            db.desconectar();
            if (res.count() > 0) {
                auto fila = res.fetchOne();
                nom_cliente = fila[0].get<string>();
                ape_cliente = fila[1].get<string>();
            }
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
            return;
        }

        char continuar = 's';
        while (continuar == 's' || continuar == 'S') {
            int id_prod = pedirIdProducto();
            Producto p;
            if (p.buscarPorId(id_prod)) {
                int cantidad = pedirCantidad(p.getExistencia());
                DetalleVenta d;
                d.id_producto = p.getId();
                d.nombre_producto = p.getProducto();
                d.cantidad = cantidad;
                d.precio_unitario = p.getPrecioVenta();
                d.subtotal = cantidad * p.getPrecioVenta();
                total += d.subtotal;
                detalles.push_back(d);
                cout << "Producto agregado. Total acumulado: Q" << fixed << setprecision(2) << total << endl;
            }
            cout << "Agregar otro producto? (s/n): ";
            cin >> continuar;
            cin.ignore();
        }

        if (detalles.empty()) {
            cout << "Error: No se agregaron productos." << endl;
            return;
        }

        no_factura = generarNoFactura();
        fecha_factura = generarFecha();

        try {
            ConexionDB db;
            if (!db.conectar()) return;

            db.getSession()->sql("START TRANSACTION").execute();

            db.getSession()->sql(
                "INSERT INTO supermercado.ventas (no_factura, serie, fecha_factura, id_cliente, id_empleado) VALUES (?, ?, ?, ?, ?)"
            ).bind(no_factura, serie, fecha_factura, id_cliente, id_empleado).execute();

            auto res = db.getSession()->sql("SELECT LAST_INSERT_ID()").execute();
            auto fila = res.fetchOne();
            id_venta = (int)fila[0];

            for (DetalleVenta d : detalles) {
                db.getSession()->sql(
                    "INSERT INTO supermercado.ventas_detalle (id_venta, id_producto, cantidad, precio_unitario) VALUES (?, ?, ?, ?)"
                ).bind(id_venta, d.id_producto, d.cantidad, d.precio_unitario).execute();

                db.getSession()->sql(
                    "UPDATE supermercado.productos SET existencia = existencia - ? WHERE id_producto = ?"
                ).bind(d.cantidad, d.id_producto).execute();
            }

            db.getSession()->sql("COMMIT").execute();
            db.desconectar();

            cout << "\nVenta registrada exitosamente!" << endl;
            imprimirFacturaConsola();

            char imprimir;
            cout << "\nDesea imprimir la factura? (s/n): ";
            cin >> imprimir;
            cin.ignore();
            if (imprimir == 's' || imprimir == 'S') {
                generarFacturaHTML();
            }

        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
            try {
                ConexionDB db;
                db.conectar();
                db.getSession()->sql("ROLLBACK").execute();
                db.desconectar();
            }
            catch (...) {}
        }
    }

    void leer() {
        try {
            ConexionDB db;
            if (!db.conectar()) return;
            auto resultado = db.getSession()->sql(
                "SELECT v.id_venta, v.no_factura, v.serie, v.fecha_factura, "
                "c.nombres, c.apellidos, e.nombres "
                "FROM supermercado.ventas v "
                "INNER JOIN supermercado.clientes c ON v.id_cliente = c.id_cliente "
                "INNER JOIN supermercado.empleados e ON v.id_empleado = e.id_empleado"
            ).execute();
            cout << "\n===== LISTA DE VENTAS =====" << endl;
            cout << "ID\tFACTURA\tSERIE\tFECHA\t\tCLIENTE\t\t\tEMPLEADO" << endl;
            cout << "--------------------------------------------------------------------------------" << endl;
            for (mysqlx::Row fila : resultado.fetchAll()) {
                cout << fila[0] << "\t" << fila[1] << "\t" << fila[2] << "\t"
                    << fila[3] << "\t" << fila[4] << " " << fila[5] << "\t\t" << fila[6] << endl;
            }
            db.desconectar();
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
};