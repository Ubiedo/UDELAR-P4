#include "../include/cliente.h"

Cliente::Cliente(DataCliente dc) {
    this->nickname = dc.getNickname();
    this->contrasena = dc.getContrasena();
    this->fechaNacimiento = dc.getFechaNacimiento();
    this->calle = dc.getCalle();
    this->nroPuerta = dc.getNroPuerta();
    this->ciudad = dc.getCiudad();
}

Cliente::~Cliente() {
    map<string, Comentario*>::iterator itCmt;
    for (itCmt = comentarios.begin(); itCmt != comentarios.end(); ++itCmt)
        delete itCmt->second;
}

string Cliente::getCalle() {
    return this->calle;
}

int Cliente::getNroPuerta() {
    return this->nroPuerta;
}

string Cliente::getCiudad() {
    return this->ciudad;
}

void Cliente::setCalle(string calle) {
    this->calle = calle;
}

void Cliente::setNroPuerta(int nroPuerta) {
    this->nroPuerta = nroPuerta;
}

void Cliente::setCiudad(string ciudad) {
    this->ciudad = ciudad;
}

string Cliente::toString() {
    string res = "Nickname: " + this->nickname + "\n" 
    + "Fecha de nacimiento: " + this->fechaNacimiento.toString() + "\n" 
    + "Calle: " + this->calle + "\n" + "Número de puerta: " + to_string(this->nroPuerta) + "\n" 
    + "Ciudad: " + this->ciudad + "\n";
    return res; 
}