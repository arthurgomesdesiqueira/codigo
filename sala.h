#ifndef SALA
#include "includes.h"
#define SALA
#define MATUTINO 0
#define TARDE 1
#define NOTURNO 2

/*
*	Defino a struct pedido que define os valores necessários pra alocar uma sala.
*/


struct Pedido{
	std::string info, predio, tipo, curso, turma, conjugar;
	int capacidade, turno, tempoInicial, tempoFinal, prioridade, periodo;
	std::vector < int > dias; // horarios seg, ter
	std::vector <int> ligacoes;
	std::vector <int> grupo;
	std::string cor;
};

// Função que ordena os pedidos por ordem de prioridade, e depois por ordem de capacidade.
bool customCompare(const Pedido &a, const Pedido &b);


/*
*	Declaro a classe sala que representa uma sala de aula.
*	Tem os atributos nome e capacidade, que define uma sala de aula.
*	O atributo capacidade é o mais importante, e é o que define a alocação.
*/

class Sala{
private:
	std::string horario[6][3][6];

public:
	std::string nome, tipo;
	int capacidade;
	Sala(std::string nome, int capacidade, std::string tipo);
	// Retorna o que há no horário.
	std::string getHorario(int dia, int turno, int digito);
	// Coloca no horário pedido a disciplina que foi alocada.
	void setHorario(int dia, int turno, int digito, std::string detalhesDoPedido);
	// Imprime detalhadamente o que há em cada dia da semana, em cada horário naquela sala.
	void imprimir();
};

#endif



