#include "Solver.h"
#include <random>
#include <algorithm>
#include <chrono>
#include "unionfind.h"

// Gera uma solução randomica para o problema
std::vector<int> Solver::generateRandomSolution()
{
	// Criando um vector com tamanho n+1 que vai e volta do deposito (cidade 0)
	std::vector<int> a = std::vector<int>(this->_node_number + 1);
	for (int i = 0; i < this->_node_number; i++)
	{
		a[i] = i;
	}
	//std::random_shuffle(a.begin() + 1, a.end() - 1);
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

	//+1 no inicio para tirar o deposito e -1 ao final com o mesmo proposito
	std::shuffle(a.begin() + 1, a.end() - 1, std::default_random_engine(seed));
	a[this->_node_number] = 0;
	return a;
}

// retorna todos as arestas do grafo ordenadas
std::vector<matrixRepresentation::edge> Solver::getSortedEdges()
{
	std::vector<matrixRepresentation::edge> arestas = this->_m.getAllEdges();
	std::sort(arestas.begin(),arestas.end(),[] (matrixRepresentation::edge a,matrixRepresentation::edge b){
		return a.weight < b.weight;
	} );
	return arestas;
}

// solucao gulasa usando kruskal
std::vector<int> Solver::solucaoKruskalAdaptada()
{
	unsigned int n = this->_node_number;
	std::vector<matrixRepresentation::edge> arestasOrd = this->getSortedEdges();
	std::vector<int> path;
	std::vector<std::pair<int,int>> grau(n); // contabilizando o grau de cada vertice, max == 2
	std::make_heap(arestasOrd.begin(),arestasOrd.end(),[] (matrixRepresentation::edge a,matrixRepresentation::edge b){
		return a.weight > b.weight;
	} );
	UnionFind conected(n);
	path.reserve(n+1);

	std::vector<matrixRepresentation::edge> caminho;
	caminho.reserve(n+1);

	matrixRepresentation::edge currentEdge;
	int pai_ori,pai_dest,count;
	count = 0; // contador de quantas arestas foram adicionadas
	while( (count < n+1) && (arestasOrd.size() > 0) )
	{
		currentEdge = arestasOrd.front(); // pegando a menor aresta
		std::pop_heap(arestasOrd.begin(),arestasOrd.end(),[] (matrixRepresentation::edge a,matrixRepresentation::edge b){
		return a.weight > b.weight;
		});
		arestasOrd.pop_back();// fazendo o pop
		if(grau[ currentEdge.to ].second < 1 &&  grau[currentEdge.from].first < 1) // nao podemos ter nos com grau > 2
		{
			pai_ori = conected.find(currentEdge.from);
			pai_dest = conected.find(currentEdge.to);
			
			if(count < n-1) // impedindo a formacao do ciclo
			{
				if(pai_ori != pai_dest) // sao de componetes conexas distintas
				{
					conected.makeUnion(pai_ori,pai_dest);
					caminho.push_back(currentEdge);
					count++;
					grau[currentEdge.from].first++;
					grau[currentEdge.to].second++;
				}
			}
			else // vai ser o ultimo no e vai fechar o caminho (e ciclo)
			{
				conected.makeUnion(pai_ori,pai_dest);
				caminho.push_back(currentEdge);
				count++;
				grau[currentEdge.from].first++;
				grau[currentEdge.to].second++;
				printf("alo\n");
			}
		}		
	}	

	std::sort( caminho.begin(),caminho.end(),[] (matrixRepresentation::edge a,matrixRepresentation::edge b){
		if(a.from < b.from)
		{
			return true;	
		}
		if (a.from == b.from)
		{
			return a.weight < b.weight;
		}
		
		return false ;
		} );
	
	// for(int i = 0; i < caminho.size(); i++)
	// {
	// 	printf("aresta ( %d , %d ) peso: %d \n",caminho[i].from,caminho[i].to,caminho[i].weight);
	// }
	// printf("numero de arestas: %d, count:%d , sets: %d\n",caminho.size(),count,conected.getNumSets());

	// for(int i = 0; i < grau.size(); i++)
	// {
	// 	if(grau[i].first < 1 || grau[i].second < 1)
	// 	{
	// 		printf("grau errado de %d: grau = %d n=%d, arestas: %d\n",i,grau[i],n,arestasOrd.size());
	// 	}
	// }
	currentEdge = caminho[0];
	path.push_back(currentEdge.from);
	count = 1;
	while(count < n+1)
	{
		path.push_back(currentEdge.to);
		currentEdge = caminho[currentEdge.to];
		count++;
	}

	for(int i = 0; i < path.size(); i++)
	{
		printf(" %d >",path[i]);
	}
	printf("\n");
	printf("peso caminho: %d\n",this->funcaoObjetiva(path));

	return path;
}


std::vector<int> Solver::solucaoGulosa()
{
	std::vector<bool> isInSolucao(this->_node_number + 1,false);
	std::vector<int> solucao(this->_node_number + 1);
	int countSolucao; // numero de nos na solucao
	
	// incluindo o deposito na soluçao
	solucao[0] = 0;
	isInSolucao[0] = true;
	countSolucao++;

	unsigned int soma = 0;
	unsigned int latencia = 0;
	std::vector<std::pair<int,int>> sortedNeighbors;
	std::pair<int,int> bestNode;

	sortedNeighbors = this->_m.getSortedNeighbours(0);
	for(int j = 1; j < this->_node_number; j++)
		{
			printf("f: %d  s:  %d\n ",sortedNeighbors[j].first,sortedNeighbors[j].second);
		}
	// partindo do primeiro no [0] indo até [n-2]
	for(auto i = 0; i < this->_node_number - 1; i++) 
	{
		sortedNeighbors = this->_m.getSortedNeighbours(i);
		
		// vamos pegar o node de menor custo, comecamos de 1 pq o elemento 0 do vetor é o proprio node
		for(int j = 1; j < this->_node_number; j++)
		{
			// se o no nao esta no vetor solucao, selecionamos ele
			if( ! isInSolucao[ sortedNeighbors[j].first ])
			{
				bestNode = sortedNeighbors[j];
				isInSolucao[sortedNeighbors[j].first] = true;
				break;
			}
		}

		// adicionamos o node a solucao
		solucao[i+1]= bestNode.first;	std::vector<int> solucaoKruskalAdaptada();
		isInSolucao[bestNode.first] = true;
		countSolucao++;

		//calculando a latencia
		soma += bestNode.second;
		latencia += soma;
		
	}
	//adicionando a volta para o depot
	solucao[this->_node_number] = 0;
	soma+= soma + this->_m.nodeAt(0,bestNode.first).second;
	for(unsigned int i=0;i<solucao.size();i++)
	{
		printf(" %d >",solucao[i]);
	}
	printf("\n");
	printf("Custo solucao calulado: %u, custo calculado com func atual %d: \n",soma,this->funcaoObjetiva(solucao));
	return solucao;
}

// Recebe uma solução, calcula e retorna a latencia total
int Solver::funcaoObjetiva(std::vector<int>& sol)
{
	int soma = 0;
	int latencia = 0;
	for (int i = 0; i < sol.size()-1; i++)
	{
		//printf("peso from %d to %d: %d\n", sol[i], sol[i + 1], _m.nodeAt(sol[i], sol[i + 1]).second);
		soma += _m.nodeAt(sol[i], sol[i + 1]).second ;
		latencia += soma;
	}
	return latencia;
}

Solver::Solver()
{

}
Solver::Solver(int n, matrixRepresentation & m)
{
	_m = m;
	_node_number = n;
	_finalSolution.resize(n + 1);
}

//Implementa uma Busca com aprofundamento iterativo com poda
std::vector<int> Solver::BAHIA()
{
	return std::vector<int>();
}
std::vector<std::vector<int>> Solver::CalculaVizinhancaSwap(std::vector<int> &sol)
{

	std::vector<std::vector<int>> vizinhanca;
	for (int i = 1; i < sol.size() - 1; i++)
	{
		// TODO: fazer uma vizinha�a olhando o elemento e ver qual a menor aresta, tentando posiciona-lo ao lado desse vizinho
		for (int j = i; j < sol.size() - 1; j++)
		{
			std::vector<int >vAux(sol);
			
			int aux = vAux[j];
			vAux[j] = vAux[i];
			vAux[i] = aux;

			vizinhanca.emplace_back(vAux);
		}
	}
		


	return vizinhanca;
}

std::vector<std::vector<int>> Solver::CalculaVizinhancaRelocate(std::vector<int> &sol)
{

	std::vector<std::vector<int>> vizinhanca;
	for (int i = 1; i < sol.size() - 1; i++)
	{
		// TODO: fazer uma vizinha�a olhando o elemento e ver qual a menor aresta, tentando posiciona-lo ao lado desse vizinho
		for (int j = i; j < sol.size() - 1; j++)
		{
			std::vector<int >vAux(sol);

			for (int k = i; k < j; k++)
			{
				vAux[k] = sol[k + 1];
			}
			vAux[j] = sol[i];
			for (int k = j+1; k < vAux.size(); k++)
			{
				vAux[k] = sol[k];
			}
			vizinhanca.emplace_back(vAux);
		}
	}



	return vizinhanca;
}


std::vector<std::vector<int>> Solver::CalculaVizinhancakOpt(std::vector<int> &sol,int k)
{

	std::vector<std::vector<int>> vizinhanca;
	for (int i = 1; i < sol.size() - k; i++) 
	{
		std::vector<int > vAux(sol);
		int count = 1;
		for (int j = i; j < i + k; j++)
		{
			vAux[j] = sol[i + k - count];
				count++;
		}
		/*for (int t = 0; t < vAux.size(); t++)
		{
			printf(" %d ", vAux[t]);
		}
		printf("\n\n");*/
	}



	return vizinhanca;
}


//Implementa uma Busca Local (Hill Climbing??)
std::vector<int> Solver::BuscaLocal()
{
	std::vector<int> inicialSolution;
	std::vector<int> currentSolution;
	std::vector<int> bestSolution;
	inicialSolution = generateRandomSolution();
	std::vector<int> test = {0,1,2,3,4,5,0};
	CalculaVizinhancakOpt(test, 2);
	//for (int i = 0; i < this->_node_number; i++)
	//{
	//	printf(" %d, ", inicialSolution[i]);
	//}
	//printf("\n");
	//printf("fo: %d\n", funcaoObjetiva(inicialSolution));
	
	
	//printf("solucao inicial = %d \n", min);
	int rodadas = 0;
	int count = 0;
	int min, minAnterior,best;
	currentSolution = inicialSolution;
	bestSolution = currentSolution;
	best = funcaoObjetiva(currentSolution);
	for (int j = 0; j < 50; j++)
	{
		while (count < 5)
		{
			min = funcaoObjetiva(currentSolution);
			minAnterior = min;
			std::vector<std::vector<int>> vz = CalculaVizinhancaSwap(currentSolution);
			for (int i = 0; i < vz.size(); i++)
			{
				int valor = funcaoObjetiva(vz[i]);
				if (valor < min)
				{
					min = valor;
					currentSolution = vz[i];
				}
			}
			if (min == minAnterior)
			{
				vz = CalculaVizinhancaRelocate(currentSolution);
				for (int i = 0; i < vz.size(); i++)
				{
					int valor = funcaoObjetiva(vz[i]);
					if (valor < min)
					{
						min = valor;
						currentSolution = vz[i];
					}
				}
				if (min == minAnterior)
				{
					vz = CalculaVizinhancakOpt(currentSolution);
					for (int i = 0; i < vz.size(); i++)
					{
						int valor = funcaoObjetiva(vz[i]);
						if (valor < min)
						{
							min = valor;
							currentSolution = vz[i];
						}
					}
					count++;
				}
			}
			rodadas++;
		}
		if (best > min) 
		{
			best = min;
			bestSolution = currentSolution;
		}
		count = 0;
		currentSolution = generateRandomSolution();
	}
	

//	std::sort(bestSolution.begin(), bestSolution.end());
	printf("menor vz = %d rodadas: %d\n", best,rodadas);
	printf("Vetor Encontrado:\n");
	for (auto i = 0; i < bestSolution.size(); i++)
	{
		printf(" %d -> ",bestSolution[i]);
	}



	return inicialSolution;
}




Solver::~Solver()
{
}
