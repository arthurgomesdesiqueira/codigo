#ifndef ALOCAR
#define ALOCAR
#include "predio.h"
#include "sala.h"

void ConexaoentrePedidos(std::vector < Pedido > &pedidos);

void lerPedidos(std::vector < Predio > &predios, std::vector < Pedido > &pedidos);

void alocar(std::vector < Predio > &predios, std::vector < Pedido > &pedidos);

void alocar_sem_grupo(std::vector < Predio > &predios, std::vector < Pedido > &pedidos, int j);


void ligarPedidos(std::vector < Pedido > &pedidos);

#endif