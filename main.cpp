#include "includes.h"
#include "sala.h"
#include "predio.h"
#include "alocar.h"

int main(){
	
	// Redirecionando o cout para um arquivo de texto.
	/*std::ofstream out("out.txt");
    std::streambuf *coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(out.rdbuf());
    */
    // Declaro meu vetor de predios, leio e aloco os pedidos.
	std::vector < Predio > predios;
	lerPredios(predios);

	// Faço o mesmo com os pedidos
	std::vector < Pedido > pedidos;
	lerPedidos(predios, pedidos);

    ConexaoentrePedidos(pedidos);
	
	ligarPedidos(pedidos);


	// Termino alocando os pedidos nos predios.
	alocar(predios, pedidos);

	

	// Resetando o cout.
   // std::cout.rdbuf(coutbuf);

    for(int i = 0; i < pedidos.size(); i++)
    {
    	std::cout << "Pedido: " << pedidos[i].info << " Cor:" << pedidos[i].cor << "\n";
        std::cout << "Mesmo grupo:\n";
        for(int j = 0; j < pedidos[i].grupo.size(); j++)
        {
            std::cout << "Pedido: " << pedidos[pedidos[i].grupo[j]].info << " Cor:" << pedidos[pedidos[i].grupo[j]].cor << "\n";
        
        }
        std::cout << "\n\n";
    }	

    for(int i = 0; i < pedidos.size(); i++)
    {
    	std::cout << pedidos[i].info << std::endl;
    	std::cout << "LIGACOES:" << std::endl;
    	for(int lerligacoes = 0; lerligacoes < pedidos[i].ligacoes.size(); lerligacoes++)
    	{
    		std::cout << pedidos[pedidos[i].ligacoes[lerligacoes]].info << "\n";
    		
    	}
    	//pedidos[i].ligacoes.erase();
    	std::cout << std::endl << std::endl;
    }

    for(int i = 0; i < pedidos.size(); i++)
    {
    	std::cout << pedidos[i].info << "\n";

    }

    std::queue <Pedido> nao_alocados;
	
    for (int i = 0; i < pedidos.size(); i++)
    {
    	if(pedidos[i].cor == "")
    	{
    		nao_alocados.push(pedidos[i]);
		}
	}	
    			
    std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nNAO ALOCADOS:\n";
    
    std::cout << "\nNão alocados:\n" << nao_alocados.size() << "\n\n";
    while(!nao_alocados.empty())
	{
		std::cout << nao_alocados.front().info << "  " << nao_alocados.front().curso << "\n";
		nao_alocados.pop();
	}

    /*for(int i = 0; i < predios.size(); i++)
    {
    	predios[i].imprimir();
    }*/

}