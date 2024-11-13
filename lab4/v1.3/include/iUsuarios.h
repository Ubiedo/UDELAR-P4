#ifndef IUSUARIOS
#define IUSUARIOS

#include "dataFecha.h"
#include "dataCliente.h"
#include "dataVendedor.h"
#include "dataProdBasica.h"
#include "dataComentario.h"
#include "dataCmtBasica.h"

#include <vector>
#include <string>
using namespace std;

class IUsuarios {
    public:
        IUsuarios();
        virtual ~IUsuarios() = 0;

        //Alta de Usuario
        virtual string crearCliente(DataCliente) = 0;
        virtual string crearVendedor(DataVendedor) = 0;

        //Listado de Usuarios
        virtual vector<string> listarUsuarios() = 0;

        //Alta de Producto
        virtual bool isEmptyVendedores() = 0;
        virtual vector<string> nicknameVendedores() = 0;

        //Dejar Comentario
        virtual vector<string> nicknameUsuarios() = 0;
        virtual void crearComentario(DataComentario) = 0;

        //Eliminar Comentario
        virtual bool isEmptyComentarios() = 0;
        virtual vector<string> usuariosConComentarios() = 0;
        virtual vector<DataCmtBasica> comentariosDeUsuario(string) = 0;
        virtual void eliminarComentario(string nickname, string codigo) = 0;

        //Crear Promocion
        virtual vector<string> vendedoresConProductoDisponible(DataFecha) = 0;
        virtual vector<DataProdBasica> productosDisponiblesVendedor(string, DataFecha) = 0;
};

#endif