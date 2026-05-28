#pragma once
#include <iostream>
#include <mysqlx/xdevapi.h>
#include <string>

using namespace std;

class ConexionDB {
private:
    mysqlx::Session* session;

public:
    ConexionDB() {
        session = nullptr;
    }

    bool conectar() {
        try {
            session = new mysqlx::Session(
                "mysqlx://root:0819ruanolopez@localhost:33060/supermercado"
            );
            return true;
        }
        catch (const exception& e) {
            cout << "Error de conexion: " << e.what() << endl;
            return false;
        }
    }

    void desconectar() {
        if (session != nullptr) {
            session->close();
            delete session;
            session = nullptr;
        }
    }

    mysqlx::Session* getSession() {
        return session;
    }

    string getSchema() {
        return "supermercado";
    }

    ~ConexionDB() {
        desconectar();
    }
};