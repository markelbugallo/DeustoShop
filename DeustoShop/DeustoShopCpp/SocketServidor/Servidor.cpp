#include <stdio.h>
#include <iostream>
#include <string>
#include <cstring>
#include <winsock2.h>
#include <sstream>
#include <vector>
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000
using namespace std;

int main() {

    WSADATA wsaData;
    SOCKET conn_socket;     // socket que escucha
    struct sockaddr_in server;
    struct sockaddr_in client;
    char sendBuff[512], recvBuff[512];

    cout << "Inicializando Winsock..." << endl;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        cout << "Fallo en el WSAStartup: " << to_string(WSAGetLastError()) << endl;
        return 1;
    }
    cout << "Inicializado." << endl;

    // Creacion de socket
    if ((conn_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        cout << "No se pudo crear el socket: " << to_string(WSAGetLastError()) << endl;
        WSACleanup();
        return 1;
    }
    cout << "Socket creado." << endl;
    
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);

    //BIND (the IP/port with socket)
	if (bind(conn_socket, (struct sockaddr*) &server, sizeof(server)) == SOCKET_ERROR) {
		cout << "Bind ha fallado con el codigo de error: " <<  to_string(WSAGetLastError()) << endl;
		closesocket(conn_socket);
		WSACleanup();
		return 1;
	}
    cout << "Bind done." << endl;

    //LISTEN to incoming connections (socket server moves to listening mode)
	if (listen(conn_socket, 5) == SOCKET_ERROR) {
		cout << "Listen failed with error code: " << to_string(WSAGetLastError()) << endl;
		closesocket(conn_socket);
		WSACleanup();
		return 1;
	}

    cout << "Esperando conexiones..." << endl;
    int stsize = sizeof(struct sockaddr);
    while (true) {
        SOCKET comm_socket = accept(conn_socket, (struct sockaddr*) &client, &stsize);
        if (comm_socket == INVALID_SOCKET) {
            cout << "Accept failed with error code : " << to_string(WSAGetLastError()) << endl;
            continue;
        }
        cout << "Conexion aceptada." << endl;
        cout << "Conexion entrante desde: " << inet_ntoa(client.sin_addr) << " (" << ntohs(client.sin_port) << ")" << endl;

        // Cada conexión acepta varios mensajes hasta que el cliente cierre la conexión.
        // Si el cliente quiere "cerrar sesión" o "registrar otro usuario", debe abrir una nueva conexión.
        cout << "Esperando mensajes entrantes del cliente... " << endl;
        while (true) {
            int bytes = recv(comm_socket, recvBuff, sizeof(recvBuff) - 1, 0);
            if (bytes <= 0) break; // Cliente cerró la conexión
            recvBuff[bytes] = '\0';
            cout << "Recibiendo mensaje... " << endl;
            cout << "Datos recibidos:  " << recvBuff << endl;

            string respuestaServidor = "";
            stringstream ss(recvBuff);
            string comando;
            getline(ss, comando, ';');
            bool cerrarConexionTrasRespuesta = false;
            if (comando == "LOGIN") {
                string nombre, contrasena;
                getline(ss, nombre, ';');
                getline(ss, contrasena, ';');
                // Buscar usuario en el CSV
                FILE* f = fopen("../../DeustoShopC/data/usuarios.csv", "r");
                if (!f) {
                    respuestaServidor = "ERROR;No se pudo abrir el archivo de usuarios";
                } else {
                    char linea[512];
                    bool encontrado = false;
                    fgets(linea, sizeof(linea), f); // Saltar cabecera
                    while (fgets(linea, sizeof(linea), f)) {
                        stringstream lss(linea);
                        string id, user, pass, contacto, id_sub, direccion, cp;
                        getline(lss, id, ';');
                        getline(lss, user, ';');
                        getline(lss, pass, ';');
                        getline(lss, contacto, ';');
                        getline(lss, id_sub, ';');
                        getline(lss, direccion, ';');
                        getline(lss, cp, ';');
                        // Limpiar salto de línea de cp
                        if (!cp.empty() && (cp.back() == '\n' || cp.back() == '\r')) cp.pop_back();
                        if (!cp.empty() && (cp.back() == '\n' || cp.back() == '\r')) cp.pop_back();
                        if (user == nombre && pass == contrasena) {
                            respuestaServidor = "OK;" + id + ";" + user + ";" + pass + ";" + contacto + ";" + id_sub + ";" + direccion + ";" + cp;
                            encontrado = true;
                            break;
                        }
                    }
                    fclose(f);
                    if (!encontrado) {
                        respuestaServidor = "ERROR;Usuario no encontrado o contraseña incorrecta";
                    }
                }
                cerrarConexionTrasRespuesta = true;
            } else if (comando == "REGISTRAR_USUARIO") {
                // Aquí deberías implementar la lógica de registro si no está ya
                // respuestaServidor = ...
                cerrarConexionTrasRespuesta = true;
            } else if (comando == "EDITAR_USUARIO") {
                // Formato esperado: EDITAR_USUARIO;id;nombre;contrasena;contacto;id_subscripcion;direccion;codigo_postal
                string id, nombre, contrasena, contacto, id_sub, direccion, cp;
                getline(ss, id, ';');
                getline(ss, nombre, ';');
                getline(ss, contrasena, ';');
                getline(ss, contacto, ';');
                getline(ss, id_sub, ';');
                getline(ss, direccion, ';');
                getline(ss, cp, ';');
                // Limpiar salto de línea de cp
                if (!cp.empty() && (cp.back() == '\n' || cp.back() == '\r')) cp.pop_back();
                if (!cp.empty() && (cp.back() == '\n' || cp.back() == '\r')) cp.pop_back();
                cout << "[DEBUG] EDITAR_USUARIO: id=" << id << ", nombre=" << nombre << ", contrasena=" << contrasena << ", contacto=" << contacto << ", id_sub=" << id_sub << ", direccion=" << direccion << ", cp=" << cp << endl;
                FILE* f = fopen("../../DeustoShopC/data/usuarios.csv", "r");
                if (!f) {
                    cout << "[ERROR] No se pudo abrir el archivo de usuarios para lectura" << endl;
                    respuestaServidor = "ERROR;No se pudo abrir el archivo de usuarios";
                } else {
                    vector<string> lineas;
                    char linea[512];
                    bool encontrado = false;
                    // Leer cabecera
                    if (fgets(linea, sizeof(linea), f)) {
                        lineas.push_back(linea);
                    }
                    // Leer y modificar usuarios
                    while (fgets(linea, sizeof(linea), f)) {
                        stringstream lss(linea);
                        string fid;
                        getline(lss, fid, ';');
                        if (fid == id) {
                            string nueva = id + ";" + nombre + ";" + contrasena + ";" + contacto + ";" + id_sub + ";" + direccion + ";" + cp + "\n";
                            cout << "[DEBUG] Modificando usuario id=" << id << " con nueva linea: " << nueva;
                            lineas.push_back(nueva);
                            encontrado = true;
                        } else {
                            lineas.push_back(linea);
                        }
                    }
                    fclose(f);
                    if (!encontrado) {
                        cout << "[ERROR] Usuario no encontrado para editar: id=" << id << endl;
                        respuestaServidor = "ERROR;Usuario no encontrado";
                    } else {
                        FILE* fw = fopen("../../DeustoShopC/data/usuarios.csv", "w");
                        if (!fw) {
                            cout << "[ERROR] No se pudo abrir el archivo de usuarios para escritura" << endl;
                            respuestaServidor = "ERROR;No se pudo escribir el archivo de usuarios";
                        } else {
                            for (const auto& l : lineas) fputs(l.c_str(), fw);
                            fclose(fw);
                            cout << "[DEBUG] Usuario actualizado correctamente en el CSV." << endl;
                            respuestaServidor = "OK;" + id + ";" + nombre + ";" + contrasena + ";" + contacto + ";" + id_sub + ";" + direccion + ";" + cp;
                        }
                    }
                }
            } else if (comando == "ELIMINAR_USUARIO") {
                // Formato esperado: ELIMINAR_USUARIO;id
                string id;
                getline(ss, id, ';');
                // Leer todo el CSV y eliminar la línea correspondiente
                FILE* f = fopen("../../DeustoShopC/data/usuarios.csv", "r");
                if (!f) {
                    respuestaServidor = "ERROR;No se pudo abrir el archivo de usuarios";
                } else {
                    vector<string> lineas;
                    char linea[512];
                    bool eliminado = false;
                    // Leer cabecera
                    if (fgets(linea, sizeof(linea), f)) {
                        lineas.push_back(linea);
                    }
                    // Leer y filtrar usuarios
                    while (fgets(linea, sizeof(linea), f)) {
                        stringstream lss(linea);
                        string fid;
                        getline(lss, fid, ';');
                        if (fid == id) {
                            eliminado = true; // No añadir esta línea
                        } else {
                            lineas.push_back(linea);
                        }
                    }
                    fclose(f);
                    if (!eliminado) {
                        respuestaServidor = "ERROR;Usuario no encontrado";
                    } else {
                        // Escribir el CSV actualizado
                        FILE* fw = fopen("../../DeustoShopC/data/usuarios.csv", "w");
                        if (!fw) {
                            respuestaServidor = "ERROR;No se pudo escribir el archivo de usuarios";
                        } else {
                            for (const auto& l : lineas) fputs(l.c_str(), fw);
                            fclose(fw);
                            respuestaServidor = "OK";
                        }
                    }
                }
            } else {
                // Respuesta por defecto para otros comandos
                respuestaServidor = "EXITO";
            }
            strcpy(sendBuff, respuestaServidor.c_str());
            send(comm_socket, sendBuff, strlen(sendBuff), 0);
            cout << "Datos enviados: " << sendBuff << endl;
            if (cerrarConexionTrasRespuesta) break;
        }
        // Cerrar la conexión con el cliente actual
        closesocket(comm_socket);
        cout << "Cliente desconectado. Esperando nueva conexion..." << endl;
    }

    // Cerrar el socket de escucha y limpiar Winsock al terminar el programa
    closesocket(conn_socket);
    WSACleanup();

    return 0;
}
