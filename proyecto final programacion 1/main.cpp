#include <iostream>
#include <regex>
#include "Marca.h"
#include "Puesto.h"
#include "Proveedor.h"
#include "Cliente.h"
#include "Empleado.h"
#include "Producto.h"
#include "Venta.h"
#include "Compra.h"
using namespace std;

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

string pedirNombreCatalogo(string mensaje, int maxLen) {
    string valor;
    do {
        cout << mensaje;
        getline(cin, valor);
        if (valor.empty()) {
            cout << "Error: El campo no puede estar vacio." << endl;
            continue;
        }
        if ((int)valor.length() > maxLen) {
            cout << "Error: Maximo " << maxLen << " caracteres." << endl;
            valor = "";
            continue;
        }
        bool tieneLetras = false;
        bool valido = true;
        for (char c : valor) {
            if (isalpha(c)) tieneLetras = true;
            if (!isalpha(c) && !isspace(c) && !isdigit(c)) {
                cout << "Error: Contiene caracteres no permitidos." << endl;
                valido = false;
                valor = "";
                break;
            }
        }
        if (valido && !tieneLetras) {
            cout << "Error: Debe contener al menos una letra." << endl;
            valor = "";
        }
    } while (valor.empty());
    return valor;
}

string pedirFecha(string mensaje) {
    string valor;
    regex patron("\\d{4}-\\d{2}-\\d{2}");
    do {
        cout << mensaje;
        getline(cin, valor);
        if (!regex_match(valor, patron)) {
            cout << "Error: Formato invalido. Use AAAA-MM-DD." << endl;
            valor = "";
        }
        else {
            int mes = stoi(valor.substr(5, 2));
            int dia = stoi(valor.substr(8, 2));
            if (mes < 1 || mes > 12) {
                cout << "Error: Mes invalido." << endl;
                valor = "";
            }
            else if (dia < 1 || dia > 31) {
                cout << "Error: Dia invalido." << endl;
                valor = "";
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

string pedirDireccion(string mensaje) {
    string valor;
    do {
        cout << mensaje;
        getline(cin, valor);
        if (valor.empty()) {
            cout << "Error: La direccion no puede estar vacia." << endl;
            continue;
        }
        if ((int)valor.length() < 7) {
            cout << "Error: La direccion debe tener minimo 7 caracteres." << endl;
            valor = "";
            continue;
        }
        if ((int)valor.length() > 80) {
            cout << "Error: La direccion debe tener maximo 80 caracteres." << endl;
            valor = "";
            continue;
        }
        bool tieneLetras = false;
        for (char c : valor) {
            if (isalpha(c)) tieneLetras = true;
        }
        if (!tieneLetras) {
            cout << "Error: La direccion debe contener letras." << endl;
            valor = "";
        }
    } while (valor.empty());
    return valor;
}

string pedirNIT(string mensaje) {
    string valor;
    regex patronNit("^([1-9][0-9]{6,10}-[0-9k]|CF|cf|C/F|c/f)$");
    do {
        cout << mensaje;
        getline(cin, valor);
        if (valor.empty()) {
            cout << "Error: El NIT no puede estar vacio." << endl;
            continue;
        }
        if (!regex_match(valor, patronNit)) {
            cout << "Error: NIT invalido. Formato: 12345678-9 o CF" << endl;
            valor = "";
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

int pedirEntero(string mensaje) {
    int valor;
    while (true) {
        cout << mensaje;
        if (cin >> valor) {
            cin.ignore();
            return valor;
        }
        else {
            cout << "Error: Ingrese un numero valido." << endl;
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
}

double pedirDouble(string mensaje) {
    double valor;
    while (true) {
        cout << mensaje;
        if (cin >> valor && valor > 0) {
            cin.ignore();
            return valor;
        }
        else {
            cout << "Error: Ingrese un valor numerico mayor a 0." << endl;
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
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

int pedirIdPuesto() {
    int valor = 0;
    do {
        cout << "ID del puesto: ";
        if (!(cin >> valor)) {
            cout << "Error: Ingrese un numero valido." << endl;
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        cin.ignore();
        try {
            ConexionDB db;
            if (!db.conectar()) continue;
            auto res = db.getSession()->sql(
                "SELECT id_puesto, puesto FROM supermercado.puestos WHERE id_puesto = ?"
            ).bind(valor).execute();
            db.desconectar();
            if (res.count() == 0) {
                cout << "Error: El puesto con ID " << valor << " no existe." << endl;
                ConexionDB db2;
                if (db2.conectar()) {
                    auto lista = db2.getSession()->sql(
                        "SELECT id_puesto, puesto FROM supermercado.puestos"
                    ).execute();
                    cout << "Puestos disponibles:" << endl;
                    for (mysqlx::Row fila : lista.fetchAll()) {
                        cout << "  " << fila[0] << " - " << fila[1] << endl;
                    }
                    db2.desconectar();
                }
                valor = 0;
            }
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
            valor = 0;
        }
    } while (valor == 0);
    return valor;
}

int pedirIdMarca() {
    int valor = 0;
    do {
        cout << "ID de la marca: ";
        if (!(cin >> valor)) {
            cout << "Error: Ingrese un numero valido." << endl;
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        cin.ignore();
        try {
            ConexionDB db;
            if (!db.conectar()) continue;
            auto res = db.getSession()->sql(
                "SELECT id_marca, marca FROM supermercado.marcas WHERE id_marca = ?"
            ).bind(valor).execute();
            db.desconectar();
            if (res.count() == 0) {
                cout << "Error: La marca con ID " << valor << " no existe." << endl;
                ConexionDB db2;
                if (db2.conectar()) {
                    auto lista = db2.getSession()->sql(
                        "SELECT id_marca, marca FROM supermercado.marcas"
                    ).execute();
                    cout << "Marcas disponibles:" << endl;
                    for (mysqlx::Row fila : lista.fetchAll()) {
                        cout << "  " << fila[0] << " - " << fila[1] << endl;
                    }
                    db2.desconectar();
                }
                valor = 0;
            }
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
            valor = 0;
        }
    } while (valor == 0);
    return valor;
}

int pedirIdEmpleado() {
    int valor = 0;
    do {
        cout << "ID del empleado: ";
        if (!(cin >> valor)) {
            cout << "Error: Ingrese un numero valido." << endl;
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        cin.ignore();
        try {
            ConexionDB db;
            if (!db.conectar()) continue;
            auto res = db.getSession()->sql(
                "SELECT id_empleado, nombres, apellidos FROM supermercado.empleados WHERE id_empleado = ?"
            ).bind(valor).execute();
            db.desconectar();
            if (res.count() == 0) {
                cout << "Error: El empleado con ID " << valor << " no existe." << endl;
                ConexionDB db2;
                if (db2.conectar()) {
                    auto lista = db2.getSession()->sql(
                        "SELECT id_empleado, nombres, apellidos FROM supermercado.empleados"
                    ).execute();
                    cout << "Empleados disponibles:" << endl;
                    for (mysqlx::Row fila : lista.fetchAll()) {
                        cout << "  " << fila[0] << " - " << fila[1] << " " << fila[2] << endl;
                    }
                    db2.desconectar();
                }
                valor = 0;
            }
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
            valor = 0;
        }
    } while (valor == 0);
    return valor;
}

void menuMarcas() {
    int op;
    do {
        cout << "\n===== MARCAS =====" << endl;
        cout << "1. Crear" << endl;
        cout << "2. Listar" << endl;
        cout << "3. Actualizar" << endl;
        cout << "4. Eliminar" << endl;
        cout << "0. Volver" << endl;
        op = pedirEntero("Opcion: ");

        if (op == 1) {
            string m = pedirNombreCatalogo("Nombre de la marca: ", 50);
            Marca marca(0, m);
            marca.crear();
        }
        else if (op == 2) {
            Marca marca;
            marca.leer();
        }
        else if (op == 3) {
            int id = pedirEntero("ID de la marca a actualizar: ");
            string m = pedirNombreCatalogo("Nuevo nombre: ", 50);
            Marca marca(id, m);
            marca.actualizar();
        }
        else if (op == 4) {
            int id = pedirEntero("ID de la marca a eliminar: ");
            Marca marca(id, "");
            marca.borrar();
        }
    } while (op != 0);
}

void menuPuestos() {
    int op;
    do {
        cout << "\n===== PUESTOS =====" << endl;
        cout << "1. Crear" << endl;
        cout << "2. Listar" << endl;
        cout << "3. Actualizar" << endl;
        cout << "4. Eliminar" << endl;
        cout << "0. Volver" << endl;
        op = pedirEntero("Opcion: ");

        if (op == 1) {
            string p = pedirNombreCatalogo("Nombre del puesto: ", 50);
            Puesto puesto(0, p);
            puesto.crear();
        }
        else if (op == 2) {
            Puesto puesto;
            puesto.leer();
        }
        else if (op == 3) {
            int id = pedirEntero("ID del puesto a actualizar: ");
            string p = pedirNombreCatalogo("Nuevo nombre: ", 50);
            Puesto puesto(id, p);
            puesto.actualizar();
        }
        else if (op == 4) {
            int id = pedirEntero("ID del puesto a eliminar: ");
            Puesto puesto(id, "");
            puesto.borrar();
        }
    } while (op != 0);
}

void menuProveedores() {
    int op;
    do {
        cout << "\n===== PROVEEDORES =====" << endl;
        cout << "1. Crear" << endl;
        cout << "2. Listar" << endl;
        cout << "3. Actualizar" << endl;
        cout << "4. Eliminar" << endl;
        cout << "0. Volver" << endl;
        op = pedirEntero("Opcion: ");

        if (op == 1) {
            string prov = pedirNombreCatalogo("Nombre del proveedor: ", 60);
            string nit = pedirNIT("NIT: ");
            string dir = pedirDireccion("Direccion: ");
            string tel = pedirTelefono("Telefono: ");
            Proveedor p(0, prov, nit, dir, tel);
            p.crear();
        }
        else if (op == 2) {
            Proveedor p;
            p.leer();
        }
        else if (op == 3) {
            int id = pedirEntero("ID del proveedor a actualizar: ");
            string prov = pedirNombreCatalogo("Nombre del proveedor: ", 60);
            string nit = pedirNIT("NIT: ");
            string dir = pedirDireccion("Direccion: ");
            string tel = pedirTelefono("Telefono: ");
            Proveedor p(id, prov, nit, dir, tel);
            p.actualizar();
        }
        else if (op == 4) {
            int id = pedirEntero("ID del proveedor a eliminar: ");
            Proveedor p(id, "", "", "", "");
            p.borrar();
        }
    } while (op != 0);
}

void menuClientes() {
    int op;
    do {
        cout << "\n===== CLIENTES =====" << endl;
        cout << "1. Crear" << endl;
        cout << "2. Listar" << endl;
        cout << "3. Actualizar" << endl;
        cout << "4. Eliminar" << endl;
        cout << "0. Volver" << endl;
        op = pedirEntero("Opcion: ");

        if (op == 1) {
            string nom = pedirNombre("Nombres: ");
            string ape = pedirNombre("Apellidos: ");
            string nit = pedirNIT("NIT (o C/F): ");
            int gen = pedirGenero();
            string tel = pedirTelefono("Telefono: ");
            string correo = pedirCorreo("Correo electronico: ");
            Cliente c(0, nom, ape, nit, gen, tel, correo);
            c.crear();
        }
        else if (op == 2) {
            Cliente c;
            c.leer();
        }
        else if (op == 3) {
            int id = pedirEntero("ID del cliente a actualizar: ");
            string nom = pedirNombre("Nombres: ");
            string ape = pedirNombre("Apellidos: ");
            string nit = pedirNIT("NIT (o C/F): ");
            int gen = pedirGenero();
            string tel = pedirTelefono("Telefono: ");
            string correo = pedirCorreo("Correo electronico: ");
            Cliente c(id, nom, ape, nit, gen, tel, correo);
            c.actualizar();
        }
        else if (op == 4) {
            int id = pedirEntero("ID del cliente a eliminar: ");
            Cliente c(id, "", "", "", 0, "", "");
            c.borrar();
        }
    } while (op != 0);
}

void menuEmpleados() {
    int op;
    do {
        cout << "\n===== EMPLEADOS =====" << endl;
        cout << "1. Crear" << endl;
        cout << "2. Listar" << endl;
        cout << "3. Actualizar" << endl;
        cout << "4. Eliminar" << endl;
        cout << "0. Volver" << endl;
        op = pedirEntero("Opcion: ");

        if (op == 1) {
            string nom = pedirNombre("Nombres: ");
            string ape = pedirNombre("Apellidos: ");
            string dir = pedirDireccion("Direccion: ");
            string tel = pedirTelefono("Telefono: ");
            int gen = pedirGenero();
            string fn = pedirFecha("Fecha nacimiento (AAAA-MM-DD): ");
            int ip = pedirIdPuesto();
            string fil = pedirFecha("Fecha inicio labores (AAAA-MM-DD): ");
            Empleado e(0, nom, ape, dir, tel, gen, fn, ip, fil);
            e.crear();
        }
        else if (op == 2) {
            Empleado e;
            e.leer();
        }
        else if (op == 3) {
            int id = pedirEntero("ID del empleado a actualizar: ");
            string nom = pedirNombre("Nombres: ");
            string ape = pedirNombre("Apellidos: ");
            string dir = pedirDireccion("Direccion: ");
            string tel = pedirTelefono("Telefono: ");
            int gen = pedirGenero();
            string fn = pedirFecha("Fecha nacimiento (AAAA-MM-DD): ");
            int ip = pedirIdPuesto();
            string fil = pedirFecha("Fecha inicio labores (AAAA-MM-DD): ");
            Empleado e(id, nom, ape, dir, tel, gen, fn, ip, fil);
            e.actualizar();
        }
        else if (op == 4) {
            int id = pedirEntero("ID del empleado a eliminar: ");
            Empleado e;
            e.setId(id);
            e.borrar();
        }
    } while (op != 0);
}

void menuProductos() {
    int op;
    do {
        cout << "\n===== PRODUCTOS =====" << endl;
        cout << "1. Crear" << endl;
        cout << "2. Listar" << endl;
        cout << "3. Actualizar" << endl;
        cout << "4. Eliminar" << endl;
        cout << "0. Volver" << endl;
        op = pedirEntero("Opcion: ");

        if (op == 1) {
            string prod = pedirNombreCatalogo("Nombre del producto: ", 50);
            int im = pedirIdMarca();
            string desc = pedirNombreCatalogo("Descripcion: ", 100);
            string img = pedirNombreCatalogo("Imagen (nombre archivo): ", 30);
            double pc = pedirDouble("Precio costo: Q");
            double pv = pedirDouble("Precio venta: Q");
            int ex = pedirEntero("Existencia inicial: ");
            Producto p(0, prod, im, desc, img, pc, pv, ex);
            p.crear();
        }
        else if (op == 2) {
            Producto p;
            p.leer();
        }
        else if (op == 3) {
            int id = pedirEntero("ID del producto a actualizar: ");
            string prod = pedirNombreCatalogo("Nombre del producto: ", 50);
            int im = pedirIdMarca();
            string desc = pedirNombreCatalogo("Descripcion: ", 100);
            string img = pedirNombreCatalogo("Imagen (nombre archivo): ", 30);
            double pc = pedirDouble("Precio costo: Q");
            double pv = pedirDouble("Precio venta: Q");
            int ex = pedirEntero("Existencia: ");
            Producto p(id, prod, im, desc, img, pc, pv, ex);
            p.actualizar();
        }
        else if (op == 4) {
            int id = pedirEntero("ID del producto a eliminar: ");
            Producto p;
            p.setId(id);
            p.borrar();
        }
    } while (op != 0);
}

void menuVentas() {
    int op;
    do {
        cout << "\n===== VENTAS =====" << endl;
        cout << "1. Nueva venta" << endl;
        cout << "2. Listar ventas" << endl;
        cout << "0. Volver" << endl;
        op = pedirEntero("Opcion: ");

        if (op == 1) {
            int id_emp = 0;
            do {
                id_emp = pedirIdEmpleado();
                char confirmar;
                cout << "Es correcto este empleado? (s/n): ";
                cin >> confirmar;
                cin.ignore();
                if (confirmar != 's' && confirmar != 'S') {
                    id_emp = 0;
                }
            } while (id_emp == 0);
            Venta v;
            v.setIdEmpleado(id_emp);
            v.registrarVenta();
        }
        else if (op == 2) {
            Venta v;
            v.leer();
        }
    } while (op != 0);
}

void menuCompras() {
    int op;
    do {
        cout << "\n===== COMPRAS =====" << endl;
        cout << "1. Nueva compra" << endl;
        cout << "2. Listar compras" << endl;
        cout << "0. Volver" << endl;
        op = pedirEntero("Opcion: ");

        if (op == 1) {
            Compra c;
            c.registrarCompra();
        }
        else if (op == 2) {
            Compra c;
            c.leer();
        }
    } while (op != 0);
}

int main() {
    int op;
    do {
        cout << "\n========================================" << endl;
        cout << "      SISTEMA SUPERMERCADO XYZ" << endl;
        cout << "========================================" << endl;
        cout << "1. Marcas" << endl;
        cout << "2. Puestos" << endl;
        cout << "3. Proveedores" << endl;
        cout << "4. Clientes" << endl;
        cout << "5. Empleados" << endl;
        cout << "6. Productos" << endl;
        cout << "7. Ventas" << endl;
        cout << "8. Compras" << endl;
        cout << "0. Salir" << endl;
        cout << "========================================" << endl;
        op = pedirEntero("Opcion: ");

        switch (op) {
        case 1: menuMarcas(); break;
        case 2: menuPuestos(); break;
        case 3: menuProveedores(); break;
        case 4: menuClientes(); break;
        case 5: menuEmpleados(); break;
        case 6: menuProductos(); break;
        case 7: menuVentas(); break;
        case 8: menuCompras(); break;
        case 0: cout << "Saliendo del sistema..." << endl; break;
        default: cout << "Opcion invalida." << endl;
        }
    } while (op != 0);

    return 0;
}