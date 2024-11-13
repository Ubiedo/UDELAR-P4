#include "../include/ctrlPromoProd.h"

CtrlPromoProd::CtrlPromoProd() {}
CtrlPromoProd::~CtrlPromoProd() {
    map<string, Producto*>::iterator itPrd;
    for (itPrd = productos.begin(); itPrd != productos.end(); ++itPrd)
        delete itPrd->second;

    map<string, Promocion*>::iterator itPrm;
    for (itPrm = promociones.begin(); itPrm != promociones.end(); ++itPrm)
        delete itPrm->second;
}

CtrlPromoProd* CtrlPromoProd::getInstancia() {
    if (instancia == NULL) {
        instancia = new CtrlPromoProd();
        return instancia;
    } else {
        return instancia;
    }
}

CtrlPromoProd* CtrlPromoProd::instancia = NULL;

//Alta de Producto

void CtrlPromoProd::crearProducto(DataProducto dp) {
    CtrlUsuarios* cUsrs = CtrlUsuarios::getInstancia();
    Vendedor* v = cUsrs->findVendedor(dp.getNickname());

    int id = generadorCodigoProducto;
    generadorCodigoProducto++;
    string strId;
    if (id < 10)
        strId = "0" + to_string(id);
    else
        strId = to_string(id);
    string codigo = "prd" + strId;

    Producto* prod = new Producto(v, codigo, dp.getNombre(), dp.getDescripcion(), dp.getCategoria(), dp.getStock(), dp.getPrecio());
    
    v->agregarProducto(prod);
    productos[codigo] = prod;
}

//Consultar Producto

bool CtrlPromoProd::isEmptyProductos() {
    return productos.empty();
}

vector<DataProdBasica> CtrlPromoProd::listarProductos() {
    vector<DataProdBasica> res;
    map<string, Producto*>::iterator itProd;
    for (itProd = productos.begin(); itProd != productos.end(); ++itProd) {
        DataProdBasica dpb = itProd->second->getDataProdBasica();
        res.push_back(dpb);
    }
    return res;
}

DataProdConsulta CtrlPromoProd::consultarProducto(string key) {
    return productos[key]->getDataProdConsulta();
}

//Dejar Comentario

vector<DataCmtBasica> CtrlPromoProd::comentariosDeProducto(string key) {
    return productos[key]->getComentarios();
}

bool CtrlPromoProd::productoTieneComentarios(string key) {
    return productos[key]->tieneComentarios();
}

//Crear Promocion

bool CtrlPromoProd::existeProductoDisponible(DataFecha fecha) {
    map<string, Producto*>::iterator itPrd = productos.begin();
    while ((itPrd != productos.end()) && (!itPrd->second->disponibleParaPromocion(fecha)))
        ++itPrd;
    return (itPrd != productos.end());
}

vector<string> CtrlPromoProd::nombrePromociones() {
    map<string, Promocion*>::iterator itPrm;
    vector<string> res;
    for (itPrm = promociones.begin(); itPrm != promociones.end(); ++itPrm)
        res.push_back(itPrm->first);
    return res;
}

void CtrlPromoProd::crearPromocion(DataPromocion dPrm) {
    CtrlUsuarios* cUsrs = CtrlUsuarios::getInstancia();
    Vendedor* v = cUsrs->findVendedor(dPrm.getNickname());

    Promocion* promo = new Promocion(v, dPrm.getNombre(), dPrm.getDescripcion(), dPrm.getDescuento(), dPrm.getVencimiento());

    promociones[dPrm.getNombre()] = promo;
    v->agregarPromocion(promo);

    string notificacion = "Nueva promoción de " + v->getNickname() + ": '" + dPrm.getNombre() + "'\n" + "Productos de la promoción: \n\n";

    vector<DataPromoProducto> productosPromocion = dPrm.getProductosPromocion();
    for (unsigned int i = 0; i < productosPromocion.size(); i++) {
        Producto* prod = productos[productosPromocion[i].getCodigoProducto()];
        PromoProducto* pp = new PromoProducto(promo, prod, productosPromocion[i].getCantidadProducto());
        promo->agregarPP(pp, prod->getCodigo());
        prod->setPP(pp);
        notificacion = notificacion + "[" + prod->getCodigo() + "] " + prod->getNombre() + "\n";
    }

    v->notificar(notificacion);
}

//Consultar Promocion

bool CtrlPromoProd::existePromoVigente(DataFecha fecha) {
    map<string, Promocion*>::iterator itPrm = promociones.begin();
    while ((itPrm != promociones.end()) && (!itPrm->second->esVigente(fecha)))
        ++itPrm;
    return (itPrm != promociones.end());
}

vector<DataPromoConsulta> CtrlPromoProd::listarPromoVigentes(DataFecha fecha) {
    vector<DataPromoConsulta> res;
    map<string, Promocion*>::iterator itPrm;
    for (itPrm = promociones.begin(); itPrm != promociones.end(); ++itPrm) 
        if (itPrm->second->esVigente(fecha))
            res.push_back(itPrm->second->getDataPromoConsulta());
    return res;
}

string CtrlPromoProd::infoVendedorPromo(string nombrePromocion) {
    Promocion* promo = promociones[nombrePromocion];
    return (promo->getVendedor()->toString());
}

vector<string> CtrlPromoProd::infoProductosPromo(string nombrePromocion) {
    return promociones[nombrePromocion]->infoProductos();
}

//Realizar Compra

vector<DataProdCompra> CtrlPromoProd::listarProductosDisponibles() {
    vector<DataProdCompra> res;
    map<string, Producto*>::iterator itProd;
    for (itProd = productos.begin(); itProd != productos.end(); ++itProd)
        if (itProd->second->getStock() > 0) {
            DataProdCompra dpc = itProd->second->getDataProdCompra();
            res.push_back(dpc);
        }
    return res;
}

DataCompra CtrlPromoProd::getDetallesCompra(vector<DataPromoProducto> productosCompra, DataFecha fecha) {
    float monto = 0;
    vector<string> infoProductos;
    for (unsigned int i = 0; i < productosCompra.size(); i++) {
        DataPromoProducto dpp = productosCompra[i];
        Producto* prod = productos[dpp.getCodigoProducto()];
        infoProductos.push_back(prod->toString());
        int cantidad = dpp.getCantidadProducto();
        if (!(prod->disponibleParaPromocion(fecha))) { //tiene promocion vigente
            Promocion* promo = promociones[prod->getNombrePromo()];
            if (promo->cumpleRequisitos(productosCompra))
                monto = monto + (prod->getPrecio() * promo->getDescuento() * cantidad);
            else
                monto = monto + (prod->getPrecio() * cantidad);
        } else
            monto = monto + (prod->getPrecio() * cantidad);
    }
    DataCompra res = DataCompra(monto, fecha, infoProductos);
    return res;
}

void CtrlPromoProd::realizarCompra(string nicknameCliente, vector<DataPromoProducto> productosCompra, DataFecha fecha, float monto) {
    CtrlUsuarios* cUsrs = CtrlUsuarios::getInstancia();
    Cliente* cl = cUsrs->findCliente(nicknameCliente);

    int id = generadorCodigoCompra;
    generadorCodigoCompra++;
    string strId;
    if (id < 10)
        strId = "0" + to_string(id);
    else
        strId = to_string(id);
    string codigo = "cmp" + strId;

    Compra* cmp = new Compra(codigo, fecha, monto, cl);
    cl->agregarCompra(cmp);

    for (unsigned int i = 0; i < productosCompra.size(); i++) {
        Producto* prod = productos[productosCompra[i].getCodigoProducto()];
        int cantidadProducto = productosCompra[i].getCantidadProducto();
        CompraProducto* cProd = new CompraProducto(cantidadProducto, cmp, prod);
        cmp->agregarCP(cProd, prod->getCodigo());
        prod->agregarCP(cProd, codigo);
        prod->disminuirStock(cantidadProducto);
    }
}

//Requeridas por CtrlUsuarios

Producto* CtrlPromoProd::findProducto(string key) {
    return productos[key];
}