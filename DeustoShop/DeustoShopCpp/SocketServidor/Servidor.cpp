#include <stdio.h>
#include <iostream>
#include <string>
#include <cstring>
#include <winsock2.h>
#include <sstream>
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000
using namespace std;

int main() {

    WSADATA wsaData;
    SOCKET conn_socket;     // socket que escucha
    SOCKET comm_socket;     // socket que recibe
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
	if (listen(conn_socket, 1) == SOCKET_ERROR) {
		cout << "Listen failed with error code: " << to_string(WSAGetLastError()) << endl;
		closesocket(conn_socket);
		WSACleanup();
		return 1;
	}

    //ACCEPT incoming connections (server keeps waiting for them)
    cout << "Esperando conexiones..." << endl;
    int stsize = sizeof(struct sockaddr);
	comm_socket = accept(conn_socket, (struct sockaddr*) &client, &stsize);
	
	// Using comm_socket is able to send/receive data to/from connected client
	if (comm_socket == INVALID_SOCKET) {
		cout << "Accept failed with error code : " << to_string(WSAGetLastError()) << endl;
		closesocket(conn_socket);
		WSACleanup();
		return 1;
	}
	cout << "Conexion aceptada." << endl;
    cout << "Conexion entrante desde: " << inet_ntoa(client.sin_addr) << " (" << ntohs(client.sin_port) << ")" << endl;

	// Closing the listening sockets (is not going to be used anymore)
	closesocket(conn_socket);

    //SEND and RECEIVE data
	cout << "Esperando mensajes entrantes del cliente... " << endl;
	do {
		int bytes = recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
		if (bytes > 0) {
			cout << "Recibiendo mensaje... " << endl;
			cout << "Datos recibidos:  " << recvBuff << endl;

			cout << "Mandando respuesta... " << endl;
			strcpy(sendBuff, "EXITO");
            send(comm_socket, sendBuff, strlen(sendBuff), 0);
			cout << "Datos enviados: " << sendBuff << endl;

			if (strcmp(recvBuff, "ADIOS") == 0)
				break;
		}
	} while (1);

    // CLOSING the sockets and cleaning Winsock...
	closesocket(comm_socket);
	WSACleanup();

    return 0;
}
