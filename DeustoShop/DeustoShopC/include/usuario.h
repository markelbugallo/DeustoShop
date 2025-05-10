
#ifndef CODIGO_USUARIO_USUARIO_H_
#define CODIGO_USUARIO_USUARIO_H_

typedef struct{
	int id_Usuario;
	char nombre_usuario[50];
	char contrasenya[50];
	int nivel;
	char direccion[50];
	int telefono_usuario;
	int id_suscripcion;
	int codigo_postal;
}Usuario;

int getIdUsuario(Usuario usu);
const char* getNombreUsuario(Usuario usu);
const char* getContrasenyaUsuario(Usuario usu);
int getNivel_usuario(Usuario usu);
const char* getDireccion_usuario(Usuario usu);
int getNumerotlf_usuario(Usuario usu);
int getIdSuscripcion_usuario(Usuario usu);
int getCodigoPostal_usuario(Usuario usu);
#endif /* CODIGO_USUARIO_USUARIO_H_ */
