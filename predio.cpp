#include "predio.h"

bool comparar(const Sala &a, const int b){
	return a.capacidade < b;
}

Predio::Predio(std::string nome){
	this->nome = nome;
}

void Predio::addSala(std::string nomeDaSala, int capacidadeDaSala, std::string tipo){
	Sala nova(nomeDaSala, capacidadeDaSala, tipo);						
	salas.push_back(nova);
}

void Predio::ordenar(){
	std::sort(salas.begin(),salas.end(),
		[](const Sala&  a,const Sala& b){							
			if(a.capacidade == b.capacidade) a.nome < b.nome;
			return a.capacidade < b.capacidade;
			});
}

void Predio::imprimir(){
	for(int i = 0;i < salas.size();i++){
		
		std::cout << "Unidade: " << nome << "\nSala: " << salas[i].nome << "\nCapacidade: " << salas[i].capacidade << " " << salas[i].tipo << std::endl;
		salas[i].imprimir();
	}
}

void lerPredios(std::vector < Predio > &predios){
	// Variaveis que serão utilizadas depois
	int i = 0, capacidade = 0;

	// Abro o arquivo Json e coloco ele num documento pra ser utilizado depois
	FILE *fp = fopen("salas.json", "r");
	char readBuffer[1000010];
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	rapidjson::Document d;
	d.ParseStream(is);

	// Declaro uma variavel para iterar pelo meu documento.	
	const rapidjson::Value& predio = d;

	// Começo iterar pelos predios.
	for(rapidjson::Value::ConstMemberIterator iter = predio.MemberBegin();iter != predio.MemberEnd();++iter){
		
		const rapidjson::Value& tipo = iter->value;
		// Crio um novo predio, com o nome dado no arquivo json.
		Predio tmp(iter->name.GetString());
		// Coloco esse predio na minha lista de predios.		
		predios.push_back(tmp);

		// Como cada predio tem 3 tipos de sala, itero pelos tipos.
		for(rapidjson::Value::ConstMemberIterator tipos = tipo.MemberBegin();tipos != tipo.MemberEnd();++tipos){

			// Comeco iterar pelas salas dentro desse predio.
			const rapidjson::Value& sala = tipos->value;


			for(rapidjson::Value::ConstMemberIterator itr = sala.MemberBegin();itr != sala.MemberEnd();++itr){

				// Pego a capacidade das salas, e a lista de salas com aquela capacidade.
				capacidade = std::stoi(itr->name.GetString());
				for(auto& array : itr->value.GetArray()){
					// E coloco essa sala no meu predio.
					predios[i].addSala(array.GetString(),capacidade, tipos->name.GetString());
				}
			}
		}
		// Por fim ordeno as salas do meu prédio por capacidade, pois precisarei disso depois.
		predios[i++].ordenar();

	}

	// Ordeno meus predios por nome, caso não tenham sido ordenados na entrada.
	std::sort(predios.begin(),predios.end(),
		[](const Predio& a,const Predio& b){
			return a.nome < b.nome;
			});
	fclose(fp);
}
