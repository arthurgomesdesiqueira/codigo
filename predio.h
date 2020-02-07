#ifndef PREDIO
#define PREDIO
#include "sala.h"
#include "includes.h"
/*
*	Declaro a classe predio, que tem todo o conjunto de salas naquele predio.
*	A função adicionar sala cria uma nova sala e adiciona aquele predio.
*	A função alocar pedido encontra a primeira sala disponivel naquele horario e coloca o pedido lá.
*/



class Predio{

public:
	std::vector < Sala > salas;
	std::string nome;
	Predio(std::string nome);
	void addSala(std::string nomeDaSala, int capacidadeDaSala, std::string tipo);
	// Ordena as salas por capacidade.
	void ordenar();
	// Imprime toda as salas daquele edificio.
	void imprimir();
	int capacidadedoPredio(int capacidadePedida);
	bool alocarPedido(int capacidadePedida, Pedido pedido);
	
};

// Comparo duas salas por ordem de capacidade. Retornando falso se a é maior que b.
bool comparar(const Sala &a, const int b);


// Função que lê o arquivo JSON que fornece os predios disponiveis pra alocação.
void lerPredios(std::vector < Predio > &predios);

#endif