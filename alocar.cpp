#include "alocar.h"
#include <stdlib.h>


void ConexaoentrePedidos(std::vector <Pedido> &pedidos)
{
	for(int i = 0; i < pedidos.size(); i++)
	{
		for(int j = 0; j < pedidos.size(); j++)
		{
		
			if(i != j)
			{
				//conjugando materias 
				if(pedidos[i].conjugar != "")
				{
					if(pedidos[i].conjugar == pedidos[j].info)
					{
						pedidos[i].info = pedidos[i].info + "/" + pedidos[j].info;
						pedidos[i].capacidade = pedidos[i].capacidade + pedidos[j].capacidade;
						pedidos.erase(pedidos.begin() + j);
						j--; //tem que olhar novamente o anterior
					}

				}
				// juntando materias do mesmo periodo, predio, capacidade, curso, turma e turno
				if(pedidos[i].curso == pedidos[j].curso && pedidos[i].predio == pedidos[j].predio && pedidos[i].periodo == pedidos[j].periodo && pedidos[i].turma == "A" && pedidos[j].turma == "A" && pedidos[i].capacidade == pedidos[j].capacidade && pedidos[i].turno == pedidos[j].turno)
				{
					pedidos[i].grupo.push_back(j);
					//pedidos.erase(pedidos.begin() + j);
					//j--;
				}

			}	


		}		
	}

}

void lerPedidos(std::vector < Predio > &predios, std::vector < Pedido > &pedidos){

	// Abro o arquivo Json e coloco ele num documento pra ser utilizado depois
	int j = 0, i = 0;
	FILE *fp = fopen("pedidos03.json", "r");
	char readBuffer[1000010];
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	rapidjson::Document d;
	d.ParseStream(is);

	// Coloco ele numa variavel pra ser iterada.
	const rapidjson::Value& pedido = d;

	for(rapidjson::Value::ConstMemberIterator itr = pedido.MemberBegin();itr != pedido.MemberEnd();++itr){
		const rapidjson::Value& membro = itr->value;
		Pedido tmp;
		i = j = 0;

		// Pego as variaveis necessarias do pedido.
		std::string horario = membro["horario"].GetString();
		tmp.info = membro["info"].GetString();
		tmp.predio = membro["predio"].GetString();
		tmp.curso = membro["curso"].GetString();
		tmp.tipo = membro["tipo"].GetString();
		tmp.capacidade = std::stoi(membro["capacidade"].GetString());
		tmp.prioridade = membro["prioridade"].GetInt();//rand() % 200;
		tmp.turma = membro["turma"].GetString();
		tmp.periodo = membro["periodo"].GetInt();//rand() % 200;
		tmp.conjugar = membro["conjugar"].GetString();
		

		// Procuro o predio que o pedido se esta referindo
		bool achei = false;
		for(;i < predios.size();++i){
			if(tmp.predio.compare(predios[i].nome) == 0){
				achei = true;
				break;
			}
		}

		// Se eu não encontrar, passo pro proximo pedido
		if(!achei){
			std::cout << "Predio nao existe\n";
			continue;
		}

		// Se não pego os dias que tem a aula, quando comeca, termina e o turno
		j = 0;
		
		while(horario[j] <= '9' && horario[j] >= '0'){
			tmp.dias.push_back((int)(horario[j]-'0')-2);
			j++;
		}
		
		if(horario[j] == 'M') tmp.turno = 0;						
		else if(horario[j] == 'T') tmp.turno = 1;
		else tmp.turno = 2;

		j++;
		
		tmp.tempoInicial = horario[j] - '0' - 1;
		tmp.tempoFinal = horario[horario.size()-1]-'0' - 1;
		//tmp.npredio = i;
		//adicionei um novo aqui
		tmp.cor = "";

		pedidos.push_back(tmp);
	}
	
	sort(pedidos.begin(),pedidos.end(), customCompare);
	fclose(fp);
}

void alocar(std::vector < Predio > &predios, std::vector < Pedido > &pedidos)
{
	std::vector <std::string> cores_usadas;
	std::vector <Sala> :: iterator percorrer_salas;
	bool salacheia;
	bool nao_coloca;
	std::string novo_nome;
	
	// vou colocar as cores dentro de todos os pedidos
	for (int j = 0; j < pedidos.size(); j++)
	{
		//verifica se já tem cor ou não	
		if(pedidos[j].cor == "")
		{	
			//pego as cores dos vizinhos do meu nó principal
			for(int i = 0; i < pedidos[j].ligacoes.size(); i++)
			{
				if(pedidos[pedidos[j].ligacoes[i]].cor != "")
				{
					nao_coloca = false;
					for(int usadas = 0; usadas < cores_usadas.size(); usadas++)
					{
						if(cores_usadas[usadas] == pedidos[pedidos[j].ligacoes[i]].cor)
						{
							nao_coloca = true;
							break;
						}
					}
					if(nao_coloca == false)
					{
						cores_usadas.push_back(pedidos[pedidos[j].ligacoes[i]].cor);
					}
					
				}

			}
			//pego as cores dos vizinhos do meu grupo
			for(int a = 0; a < pedidos[j].grupo.size(); a++)
			{
				for(int i = 0; i < pedidos[pedidos[j].grupo[a]].ligacoes.size(); i++)
				{
					if(pedidos[pedidos[pedidos[j].grupo[a]].ligacoes[i]].cor != "")
					{
						nao_coloca = false;
						for(int usadas = 0; usadas < cores_usadas.size(); usadas++)
						{
							if(cores_usadas[usadas] == pedidos[pedidos[pedidos[j].grupo[a]].ligacoes[i]].cor)
							{
								nao_coloca = true;
								break;
							}
						}
						if(nao_coloca == false)
						{
							cores_usadas.push_back(pedidos[pedidos[pedidos[j].grupo[a]].ligacoes[i]].cor);
						}
					
					}

				}
			}	

			//imprimo as cores dos vizinhos e imprimo o pedido que está sendo observado
			/*std::cout << "Pedido: " << pedidos[j].info << " Cores usadas:";
			for(int w = 0; w < cores_usadas.size(); w++)
			{	
				std::cout << " " << cores_usadas[w];

			}
			std::cout << "\n";
			*/
			//comparando dentro do seu predio qual cor pode colocar dentro desse pedido, visto quais sao as cores que nao pode colocar
			for(int i = 0; i < predios.size(); i++)
			{
				//procurando o predio
				if(predios[i].nome == pedidos[j].predio)
				{
					//std::cout << "entrou " << pedidos[j].info << " " << predios[i].nome << "\n";		
					percorrer_salas = lower_bound(predios[i].salas.begin(),predios[i].salas.end(), pedidos[j].capacidade, comparar);
					for(; percorrer_salas !=  predios[i].salas.end(); percorrer_salas++)
					{
						//olha o tipo da sala e do pedido
						if((*percorrer_salas).tipo != pedidos[j].tipo) continue;
						//std::cout << (*percorrer_salas).nome << "\n";
					
						//verifica se a sala ta cheia ou nao
						salacheia = false;
						for(int w = 0; w < cores_usadas.size(); w++)
						{
							//alterando o nome para novo = 101 + CAB
							novo_nome = (*percorrer_salas).nome + predios[i].nome;
							if(novo_nome == cores_usadas[w])
							{
								salacheia = true;
								break;
							}

						}
						if(salacheia) continue;

						pedidos[j].cor = (*percorrer_salas).nome + predios[i].nome;
						for(int per = 0; per < pedidos[j].grupo.size(); per++)
						{	
							pedidos[pedidos[j].grupo[per]].cor = (*percorrer_salas).nome + predios[i].nome;

						} 
						// aqui é so pra alocar direitinho e verificar se está certo
						/*for(int abc = pedido[j].tempoInicial;abc <= pedido[j].tempoFinal;abc++)
						{
							(*vetor).setHorario(dia, pedido.turno, abc, pedido.info);
						}*/
						break;	//conseguiu alocar
					}
				
					break; //ja achou o predio	
				}
			
			}


			//nao alocou o pedido no predio, entao ele vai tentar alocar esse pedido em outro predio 
			if(pedidos[j].cor == "" && (pedidos[j].predio == "CAA" || pedidos[j].predio == "CAB" || pedidos[j].predio == "CAC"))
			{
				std::vector<int> ipredios;
				// verificando qual predio que o pedido solicitou e ver quais serao os proximos predios que vamos tentar alocar o pedido
				if(pedidos[j].predio == "CAA")
				{
					for(int i = 0; i < predios.size(); i++)
					{
						if(predios[i].nome == "CAC")
						{
							ipredios.push_back(i);
							break;
						}
				
					}
					for(int i = 0; i < predios.size(); i++)
					{
						if(predios[i].nome == "CAB")
						{
							ipredios.push_back(i);
							break;
						}
				
					}	

				}
				else if(pedidos[j].predio == "CAB")
				{
					for(int i = 0; i < predios.size(); i++)
					{
						if(predios[i].nome == "CAC")
						{
							ipredios.push_back(i);
							break;
						}
				
					}
					for(int i = 0; i < predios.size(); i++)
					{
						if(predios[i].nome == "CAA")
						{
							ipredios.push_back(i);
							break;
						}
				
					}	

				}
				else if(pedidos[j].predio == "CAC")
				{
					for(int i = 0; i < predios.size(); i++)
					{
						if(predios[i].nome == "CAB")
						{
							ipredios.push_back(i);
							break;
						}
				
					}
					for(int i = 0; i < predios.size(); i++)
					{
						if(predios[i].nome == "CAA")
						{
							ipredios.push_back(i);
							break;
						}
				
					}	

				}
				// vai tentar alocar o pedido nos outros predios
				for(int w = 0; w < ipredios.size(); w++)
				{
					int i = ipredios[w];
					//std::cout << "entrou " << pedidos[j].info << " " << predios[i].nome << "\n";		
					percorrer_salas = lower_bound(predios[i].salas.begin(),predios[i].salas.end(), pedidos[j].capacidade, comparar);
					for(; percorrer_salas !=  predios[i].salas.end(); percorrer_salas++)
					{
						if((*percorrer_salas).tipo != pedidos[j].tipo) continue;
						//std::cout << (*percorrer_salas).nome << "\n";
						salacheia = false;
						for(int w = 0; w < cores_usadas.size(); w++)
						{
							//alterando o nome para novo = 101 + CAB
							novo_nome = (*percorrer_salas).nome + predios[i].nome;
							if(novo_nome == cores_usadas[w])
							{
								salacheia = true;
								break;
							}

						}
						if(salacheia) continue;

						// alocando pro pedidos[j] e seu grupo
						pedidos[j].cor = (*percorrer_salas).nome + predios[i].nome; 
						for(int per = 0; per < pedidos[j].grupo.size(); per++)
						{	
							pedidos[pedidos[j].grupo[per]].cor = (*percorrer_salas).nome + predios[i].nome;

						}


						break;	//achou a cor
					}

				
					if(pedidos[j].cor != "")
					{
						break;	//achou a cor
					}
				
					

				}

				// apaga os predios que fizeram rotação
				ipredios.erase(ipredios.begin(), ipredios.begin() + ipredios.size());
			}
			//ele deleta as cores_usadas(vizinhos)
			cores_usadas.erase(cores_usadas.begin(), cores_usadas.begin() + cores_usadas.size());
		

		}

		if(pedidos[j].cor == "" && pedidos[j].grupo.size() > 0)
		{
			alocar_sem_grupo(predios, pedidos, j);
		}
		


	}




}

void alocar_sem_grupo(std::vector < Predio > &predios, std::vector < Pedido > &pedidos, int j)
{
	std::vector <std::string> cores_usadas;
	std::vector <Sala> :: iterator percorrer_salas;
	bool salacheia;
	bool nao_coloca;
	std::string novo_nome;
	int valordogrupo;		

	//deletando o pedido[j] do grupo
	for(int i = 0; i < pedidos[j].grupo.size(); i++)
	{
		valordogrupo = pedidos[j].grupo[i];
		for(int a = 0; a < pedidos[valordogrupo].grupo.size(); a++)
		{ 

			if(pedidos[j].info == pedidos[pedidos[valordogrupo].grupo[a]].info)
			{
				pedidos[valordogrupo].grupo.erase(pedidos[valordogrupo].grupo.begin() + a);	
			}
		}

	}

	pedidos[j].grupo.erase(pedidos[j].grupo.begin(), pedidos[j].grupo.end());



	//pego as cores dos vizinhos do meu nó principal
	for(int i = 0; i < pedidos[j].ligacoes.size(); i++)
	{
		if(pedidos[pedidos[j].ligacoes[i]].cor != "")
		{
			nao_coloca = false;
			for(int usadas = 0; usadas < cores_usadas.size(); usadas++)
			{
				if(cores_usadas[usadas] == pedidos[pedidos[j].ligacoes[i]].cor)
				{
					nao_coloca = true;
					break;
				}
			}
			if(nao_coloca == false)
			{
				cores_usadas.push_back(pedidos[pedidos[j].ligacoes[i]].cor);
			}
					
		}

	}	

	//imprimo as cores dos vizinhos e imprimo o pedido que está sendo observado
	/*std::cout << "Pedido: " << pedidos[j].info << " Cores usadas:";
	for(int w = 0; w < cores_usadas.size(); w++)
	{	
		std::cout << " " << cores_usadas[w];

	}
	std::cout << "\n";
	*/
	//comparando dentro do seu predio qual cor pode colocar dentro desse pedido, visto quais sao as cores que nao pode colocar
	for(int i = 0; i < predios.size(); i++)
	{
		//procurando o predio
		if(predios[i].nome == pedidos[j].predio)
		{
			//std::cout << "entrou " << pedidos[j].info << " " << predios[i].nome << "\n";		
			percorrer_salas = lower_bound(predios[i].salas.begin(),predios[i].salas.end(), pedidos[j].capacidade, comparar);
			for(; percorrer_salas !=  predios[i].salas.end(); percorrer_salas++)
			{
				//olha o tipo da sala e do pedido
				if((*percorrer_salas).tipo != pedidos[j].tipo) continue;
				//std::cout << (*percorrer_salas).nome << "\n";
			
				//verifica se a sala ta cheia ou nao
				salacheia = false;
				for(int w = 0; w < cores_usadas.size(); w++)
				{
					//alterando o nome para novo = 101 + CAB
					novo_nome = (*percorrer_salas).nome + predios[i].nome;
					if(novo_nome == cores_usadas[w])
					{
						salacheia = true;
						break;
					}

				}
				if(salacheia) continue;

				pedidos[j].cor = (*percorrer_salas).nome + predios[i].nome;
	

				// aqui é so pra alocar direitinho e verificar se está certo
				/*for(int abc = pedido[j].tempoInicial;abc <= pedido[j].tempoFinal;abc++)
				{
					(*vetor).setHorario(dia, pedido.turno, abc, pedido.info);
				}*/
				break;	//conseguiu alocar
			}
				
			break; //ja achou o predio	
		}
			
	}


	//nao alocou o pedido no predio, entao ele vai tentar alocar esse pedido em outro predio 
	if(pedidos[j].cor == "" && (pedidos[j].predio == "CAA" || pedidos[j].predio == "CAB" || pedidos[j].predio == "CAC"))
	{
		std::vector<int> ipredios;
		// verificando qual predio que o pedido solicitou e ver quais serao os proximos predios que vamos tentar alocar o pedido
		if(pedidos[j].predio == "CAA")
		{
			for(int i = 0; i < predios.size(); i++)
			{
				if(predios[i].nome == "CAC")
				{
					ipredios.push_back(i);
					break;
				}
		
			}
			for(int i = 0; i < predios.size(); i++)
			{
				if(predios[i].nome == "CAB")
				{
					ipredios.push_back(i);
					break;
				}
		
			}	

		}
		else if(pedidos[j].predio == "CAB")
		{
			for(int i = 0; i < predios.size(); i++)
			{
				if(predios[i].nome == "CAC")
				{
					ipredios.push_back(i);
					break;
				}
				
			}
			for(int i = 0; i < predios.size(); i++)
			{
				if(predios[i].nome == "CAA")
				{
					ipredios.push_back(i);
					break;
				}
				
			}	

		}
		else if(pedidos[j].predio == "CAC")
		{
			for(int i = 0; i < predios.size(); i++)
			{
				if(predios[i].nome == "CAB")
				{
					ipredios.push_back(i);
					break;
				}
				
			}
			for(int i = 0; i < predios.size(); i++)
			{
				if(predios[i].nome == "CAA")
				{
					ipredios.push_back(i);
					break;
				}
				
			}	

		}
		// vai tentar alocar o pedido nos outros predios
		for(int w = 0; w < ipredios.size(); w++)
		{
			int i = ipredios[w];
			//std::cout << "entrou " << pedidos[j].info << " " << predios[i].nome << "\n";		
			percorrer_salas = lower_bound(predios[i].salas.begin(),predios[i].salas.end(), pedidos[j].capacidade, comparar);
			for(; percorrer_salas !=  predios[i].salas.end(); percorrer_salas++)
			{
				if((*percorrer_salas).tipo != pedidos[j].tipo) continue;
				//std::cout << (*percorrer_salas).nome << "\n";
				salacheia = false;
				for(int w = 0; w < cores_usadas.size(); w++)
				{
					//alterando o nome para novo = 101 + CAB
					novo_nome = (*percorrer_salas).nome + predios[i].nome;
					if(novo_nome == cores_usadas[w])
					{
						salacheia = true;
						break;
					}
				}
				if(salacheia) continue;

				// alocando pro pedidos[j]
				pedidos[j].cor = (*percorrer_salas).nome + predios[i].nome; 

				break;	//achou a cor
			}

				
			if(pedidos[j].cor != "")
			{
				break;	//achou a cor
			}
				
					

		}

		// apaga os predios que fizeram rotação
		ipredios.erase(ipredios.begin(), ipredios.begin() + ipredios.size());
	}
	//ele deleta as cores_usadas(vizinhos)
	cores_usadas.erase(cores_usadas.begin(), cores_usadas.begin() + cores_usadas.size());
		

			




}

void ligarPedidos(std::vector < Pedido > &pedidos)
{
	int w, k, diaprimeiro, diasegundo, a;
	bool ligado;

	for(int i = 0; i < pedidos.size(); i++)
	{
		for (int j = i+1; j < pedidos.size(); j++)
		{
			// vendo o pedido[i] se tem conflito ou nao
			if(pedidos[i].tipo == pedidos[j].tipo)
			{
				if((pedidos[i].predio == pedidos[j].predio)
				|| ((pedidos[i].predio == "CAA" || pedidos[i].predio == "CAB" || pedidos[i].predio == "CAC") 
				&& (pedidos[j].predio == "CAA" || pedidos[j].predio == "CAB" || pedidos[j].predio == "CAC")))
				{
					for(w = 0; w < pedidos[i].dias.size(); w++)
					{
						diaprimeiro = pedidos[i].dias[w];
						for(k = 0; k < pedidos[j].dias.size(); k++)
						{
							diasegundo = pedidos[j].dias[k];
							if(diaprimeiro == diasegundo)
							{
								if(pedidos[i].turno == pedidos[j].turno)
								{
									if((pedidos[i].tempoInicial <= pedidos[j].tempoInicial && pedidos[i].tempoFinal >= pedidos[j].tempoInicial) 
									|| (pedidos[i].tempoInicial <= pedidos[j].tempoFinal && pedidos[i].tempoFinal >= pedidos[j].tempoFinal)
									|| (pedidos[j].tempoInicial <= pedidos[i].tempoInicial && pedidos[j].tempoFinal >= pedidos[i].tempoInicial)
									|| (pedidos[j].tempoInicial <= pedidos[i].tempoFinal && pedidos[j].tempoFinal >= pedidos[i].tempoFinal))
									{

										//if(pedidos[i].capacidade == pedidos[j].capacidade)
										//{
											//std:: cout << "Conflito:\n";
											//std:: cout << pedidos[i].info << " com " << pedidos[j].info << std::endl;
											pedidos[i].ligacoes.push_back(j);
											pedidos[j].ligacoes.push_back(i);

											w = pedidos[i].dias.size();
											k = pedidos[j].dias.size();
										//}	
										/*else if(pedidos[i].capacidade > pedidos[j].capacidade)
										{

											pedidos[j].ligacoes.push_back(i);

											w = pedidos[i].dias.size();
											k = pedidos[j].dias.size();
										}
										else if(pedidos[i].capacidade < pedidos[j].capacidade)
										{

											pedidos[i].ligacoes.push_back(j);
											
											w = pedidos[i].dias.size();
											k = pedidos[j].dias.size();
										}*/
										//ligado = true;	

									}		


								}	


							}


						}

					}	

				}

			}
			// vendo se no grupo tem conflito ou nao
/*			if(!ligado)
			{

				for(a = 0; a < pedidos[i].grupo.size() && ligado == false; a++)
				{
					if(pedidos[i].grupo[a].tipo == pedidos[j].tipo)
					{
						if((pedidos[i].grupo[a].predio == pedidos[j].predio)
						|| ((pedidos[i].grupo[a].predio == "CAA" || pedidos[i].grupo[a].predio == "CAB" || pedidos[i].grupo[a].predio == "CAC") 
						&& (pedidos[j].predio == "CAA" || pedidos[j].predio == "CAB" || pedidos[j].predio == "CAC")))
						{
							for(w = 0; w < pedidos[i].grupo[a].dias.size(); w++)
							{
								diaprimeiro = pedidos[i].grupo[a].dias[w];
								for(k = 0; k < pedidos[j].dias.size(); k++)
								{
									diasegundo = pedidos[j].dias[k];
									if(diaprimeiro == diasegundo)
									{
										if(pedidos[i].grupo[a].turno == pedidos[j].turno)
										{
											if((pedidos[i].grupo[a].tempoInicial <= pedidos[j].tempoInicial && pedidos[i].grupo[a].tempoFinal >= pedidos[j].tempoInicial) 
											|| (pedidos[i].grupo[a].tempoInicial <= pedidos[j].tempoFinal && pedidos[i].grupo[a].tempoFinal >= pedidos[j].tempoFinal)
											|| (pedidos[j].tempoInicial <= pedidos[i].grupo[a].tempoInicial && pedidos[j].tempoFinal >= pedidos[i].grupo[a].tempoInicial)
											|| (pedidos[j].tempoInicial <= pedidos[i].grupo[a].tempoFinal && pedidos[j].tempoFinal >= pedidos[i].grupo[a].tempoFinal))
											{

												//if(pedidos[i].capacidade == pedidos[j].capacidade)
												//{
													//std::cout << "Conflito:\n";
													//std:: cout << pedidos[i].grupo[a].info << " com " << pedidos[j].info << std::endl;
											
													pedidos[i].ligacoes.push_back(j);
													pedidos[j].ligacoes.push_back(i);

													w = pedidos[i].dias.size();
													k = pedidos[j].dias.size();
												//}	
												else if(pedidos[i].capacidade > pedidos[j].capacidade)
												{

												pedidos[j].ligacoes.push_back(i);

												w = pedidos[i].dias.size();
												k = pedidos[j].dias.size();
												}
												else if(pedidos[i].capacidade < pedidos[j].capacidade)
												{

													pedidos[i].ligacoes.push_back(j);
											
													w = pedidos[i].dias.size();
													k = pedidos[j].dias.size();
												}
												ligado = true;	

											}		


										}	


									}


								}

							}	

						}

					}

				}

			}

*/
	

		}

	}

}					























