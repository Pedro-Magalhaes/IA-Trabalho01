#include "Solver.h"
#include <random>
#include <algorithm>
#include <iostream>
#include <chrono>
#include "math.h"
#include <climits>
#include <ctime>
void printasolucao(std::vector<int> v)
{
	for (int i = 0; i < v.size(); i++)
	{
		printf("%d ", v[i]);
	}
	printf("\n");
}
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
	std::vector<int> pathA;
	std::vector<int> pathB;
	std::vector<std::pair<int,int>> grau(n); // contabilizando o grau de cada vertice, max == 2
	std::make_heap(arestasOrd.begin(),arestasOrd.end(),[] (matrixRepresentation::edge a,matrixRepresentation::edge b){
		return a.weight > b.weight;
	});
	UnionFind conected(n);
	pathA.reserve(n+1);
	pathB.reserve(n+1);

	std::vector<matrixRepresentation::edge> caminho;
	caminho.reserve(n + 1);

	matrixRepresentation::edge currentEdge;
	int pai_ori, pai_dest, count;
	count = 0; // contador de quantas arestas foram adicionadas
	while ((count < n + 1) && (arestasOrd.size() > 0))
	{
		currentEdge = arestasOrd.front(); // pegando a menor aresta
		std::pop_heap(arestasOrd.begin(), arestasOrd.end(), [](matrixRepresentation::edge a, matrixRepresentation::edge b) {
			return a.weight > b.weight;
		});
		arestasOrd.pop_back();// fazendo o pop
		if (grau[currentEdge.to].second < 1 && grau[currentEdge.from].first < 1) // nao podemos ter nos com grau > 2
		{
			pai_ori = conected.find(currentEdge.from);
			pai_dest = conected.find(currentEdge.to);

			if (count < n - 1) // impedindo a formacao do ciclo
			{
				if (pai_ori != pai_dest) // sao de componetes conexas distintas
				{
					conected.makeUnion(pai_ori, pai_dest);
					caminho.push_back(currentEdge);
					count++;
					grau[currentEdge.from].first++;
					grau[currentEdge.to].second++;
				}
			}
			else // vai ser o ultimo no e vai fechar o caminho (e ciclo)
			{
				conected.makeUnion(pai_ori, pai_dest);
				caminho.push_back(currentEdge);
				count++;
				grau[currentEdge.from].first++;
				grau[currentEdge.to].second++;
			}
		}		
	}	
	
	return kruskalGetBestPath(caminho);
}

//Dado um caminho so com as arestas de ex: 0 1 2 3 0 , retorna se eh melhor fazer 0 1 2 3 0 ou 0 3 2 1 0
std::vector<int> Solver::kruskalGetBestPath(std::vector<matrixRepresentation::edge> pathA)
{
	unsigned int n = this->_node_number;
	std::vector<matrixRepresentation::edge> pathB(pathA);
	std::sort(pathA.begin(), pathA.end(), [](matrixRepresentation::edge a, matrixRepresentation::edge b) {
		if (a.from < b.from)
		{
			return true;
		}
		if (a.from == b.from)
		{
			return a.weight < b.weight;
		}

		return false;
	});

	std::sort(pathB.begin(), pathB.end(), [](matrixRepresentation::edge a, matrixRepresentation::edge b) {
		if (a.to < b.to)
		{
			return true;
		}
		if (a.to == b.to)
		{
			return a.weight < b.weight;
		}

		return false;
	});
	// calculando a rota do pathA
	auto currentEdge = pathA[0];
	std::vector<int> caminhoA;
	caminhoA.push_back(currentEdge.from);
	int count = 1;
	while (count < n + 1)
	{
		caminhoA.push_back(currentEdge.to);
		currentEdge = pathA[currentEdge.to];
		count++;
	}

	std::vector<int> caminhoB;
	currentEdge = pathB[0];
	caminhoB.push_back(currentEdge.to);
	count = 1;
	while (count < n + 1)
	{
		caminhoB.push_back(currentEdge.from);
		currentEdge = pathB[currentEdge.from];
		count++;
	}
	return this->funcaoObjetiva(caminhoB) < funcaoObjetiva(caminhoA) ? caminhoB : caminhoA;
}

// Implementamos mas não estamos utilizando pois deu resultados ruins, pegava o proximo no mais "perto" e colocava no caminho
std::vector<int> Solver::solucaoGulosa()
{
	std::vector<bool> isInSolucao(this->_node_number + 1,false);
	std::vector<int> solucao(this->_node_number + 1);
	int countSolucao = 0; // numero de nos na solucao
	
	// incluindo o deposito na soluçao
	solucao[0] = 0;
	isInSolucao[0] = true;
	countSolucao++;

	unsigned int soma = 0;
	unsigned int latencia = 0;
	std::vector<std::pair<int,int>> sortedNeighbors;
	std::pair<int,int> bestNode;

	sortedNeighbors = this->_m.getSortedNeighbours(0);
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

	return solucao;
}

// Recebe uma solução, calcula e retorna a latencia total
int Solver::funcaoObjetiva(std::vector<int>& sol)
{
	int soma = 0;
	int latencia = 0;
	for (int i = 0; i < sol.size()-1; i++)
	{
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
// s == depot , p == profundidade maxima k == numero de vizinhos a explorar
std::vector<int> Solver::dfs_latencia(int s, int p,int k) 
{
	clock_t inicio = std::clock();
	int latencia;
	
	int latMelhorCandidato = INT_MAX;
	std::vector<Color> inPath(_node_number,Color::WHITE);
	int melhorLatencia = INT_MAX;
	std::vector<int> path;
	path.reserve(_node_number + 1);
	path.emplace_back(s); // depot
	inPath[s] = Color::BLACK;
	int melhorCandidato = 0;
	//percorre todos  os nós
	for(unsigned int i = 1; i<_m.getNodeNumber(); i++)
	{
		melhorLatencia = INT_MAX;
		latMelhorCandidato = INT_MAX;
		//Pegando vizinhos ordenados por ordem de custo do melhor candidato que no inicio eh o deposito
		std::vector<std::pair<int,int>> melhoresVizinhos = _m.getSortedNeighbours(melhorCandidato);
		//Passando pelos vizinhos
		int somaLatencia = 0;
		int maxExploration = 0;
		for (unsigned int j = 1; j < melhoresVizinhos.size(); j++)
		{
			//Se vizinho ainda não foi visitado, visita ele com a dfs_visit
			if (inPath[melhoresVizinhos[j].first] == Color::WHITE)
			{
				//Percorre e calcula a latencia ate uma  profundidade p
				latencia = dfs_soma_latencia_visit(melhoresVizinhos[j].first, p - 1, melhoresVizinhos[j].second, &melhorLatencia, inPath, i,somaLatencia,k);			

				//Se a latencia for melhor que a latencia do melhor candidato até agora, troca a melhor latencia e o melhor candidato
				if (latencia < latMelhorCandidato)
				{
					latMelhorCandidato = latencia;
					melhorCandidato = melhoresVizinhos[j].first;
				}
				if (maxExploration > k)
				{
					maxExploration++;
					break;
				}
			}

		}
		//Depois que passa pelo melhor candidato, marca ele como já visto com a cor black
		inPath[melhorCandidato] = Color::BLACK;
		//Adiciona o melhor candidato ao nosso caminho
		path.emplace_back(melhorCandidato);
	}
	//No final adiciona novamente o depósito 
	path.emplace_back(s); //back depot
	clock_t fim = std::clock();
	float tempo = float(fim - inicio)/ CLOCKS_PER_SEC;
	printf("Tempo da busca gulosa: %lf\n",tempo );
	return path;
}
// vai retornar o id do no com caminho mais barato e a latencia ate p
int Solver::dfs_soma_latencia_visit(int u, int p, int latencia, int * melhorLatencia, std::vector<Color> inPath, int pAtual,int somaLatencia, int k)
{
	somaLatencia += latencia;
	inPath[u] = Color::GRAY;

	if(somaLatencia > *melhorLatencia) //(condicao de parada 1) ja nao eh um caminho viavel (podamos)
	{
		//printf("Poda em no %d , prof %d\n",u,p);
		return INT_MAX;
	}
	if (p == 0) //(condicao de parada 2) Se a profundidade restante a ser explorada for zero
	{
		if(somaLatencia < *melhorLatencia)
		{
			*melhorLatencia = somaLatencia;
		}
		return somaLatencia;
	}
	if(pAtual == _node_number - 1) //(condicao de parada 3) chegou no ultimo no tem que contar o custo do deposito
	{
		somaLatencia = somaLatencia +_m.valueat(u,0);
		if(*melhorLatencia > somaLatencia)
		{
			*melhorLatencia = somaLatencia;
		}
		return somaLatencia;
	}
	int thisLat;
	int bestLat = INT_MAX;
	auto vizinhosOrd = _m.getSortedNeighbours(u);
	unsigned int i = 1; // comecamos a percorrer de 1 porque vizinhoOrd[0] eh o proprio vertice com peso 0
	int latenciaAtual = latencia;
	// Vai percorrer todos os vizinho do vertice corrente que ainda nao estao no caminho
	int maxExploration = 0;
	while(i < vizinhosOrd.size() && maxExploration < k)
	{
		if(inPath[vizinhosOrd[i].first] == Color::WHITE) 
		{
			latenciaAtual = latencia + vizinhosOrd[i].second;
			
			thisLat = dfs_soma_latencia_visit(vizinhosOrd[i].first, p-1,latenciaAtual,melhorLatencia,inPath,pAtual+1,somaLatencia,k);
			maxExploration++;
			if(bestLat > thisLat)
			{
				bestLat = thisLat;
			//	printf("Hey trocando bestLat %d , thisLat %d\n",bestLat,thisLat);
			}
		}
		
		i++;
	}
	inPath[u] = Color::BLACK;
	// if(bestLat < INT_MAX)
	// 	printf("Hey Hey %d\n",bestLat);
	return bestLat;
}



std::vector<std::vector<int>> Solver::CalculaVizinhancaSwap(std::vector<int> &sol, int limit)
{
	if (limit == 0)
	{
		limit = sol.size();
	}
	std::vector<std::vector<int>> vizinhanca;
	for (int i = 1; i < limit; i++)
	{
		// TODO: fazer uma vizinha�a olhando o elemento e ver qual a menor aresta, tentando posiciona-lo ao lado desse vizinho
		for (int j = i+1; j < limit-1; j++)
		{
			std::vector<int >vAux(sol);
			
			int aux = vAux[j];
			vAux[j] = vAux[i];
			vAux[i] = aux;

			vizinhanca.emplace_back(vAux);
		}
	}
		

	/*printf("Numero de vizinhos retornado:%d\n", vizinhanca.size());*/
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

std::vector < int> Solver::TwoOptSwap(const std::vector<int> & currentSol, int i, int k)
{
	//take route[1] to route[i - 1] and add them in order to new_route
	//	2. take route[i] to route[k] and add them in reverse order to new_route
	//	3. take route[k + 1] to end and add them in order to new_route
	std::vector < int > vizinho(currentSol);
	int count = 0;
	for (int j = i; j <= k; j++)
	{
		vizinho[j] = currentSol[ k - count];
		count++;
	}
	return vizinho;
}


std::vector<std::vector<int>> Solver::CalculaVizinhanca2Opt(std::vector<int> &sol)
{
	unsigned int n = sol.size();
	std::vector<std::vector<int>> vizinhanca;

	for (int i = 1; i < n - 2; i++) {
		for (int k = i + 1; k < n-1 ; k++) {
			vizinhanca.emplace_back(TwoOptSwap(sol, i, k));
		}
	}

	return vizinhanca;
}

//Implementa uma Busca Local 
std::vector<int> Solver::BuscaLocal()
{
	clock_t inicio = std::clock();
	std::vector<int> inicialSolution;
	std::vector<int> currentSolution;
	std::vector<int> bestSolution;
	inicialSolution = dfs_latencia(0,7,5);
	int rodadas = 0;
	int count = 0;
	int min, minAnterior,best;
	currentSolution = inicialSolution;
	bestSolution = currentSolution;
	best = funcaoObjetiva(currentSolution);
	for (int j = 0; j < 1; j++)
	{
		while (count < 5)
		{
			min = funcaoObjetiva(currentSolution);
			minAnterior = min;
			std::vector<std::vector<int>> vz = CalculaVizinhanca2Opt(currentSolution); 
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
				vz = CalculaVizinhancaSwap(currentSolution);
				for (int i = 0; i < vz.size(); i++)
				{
					int valor = funcaoObjetiva(vz[i]);
					if (valor < min)
					{
						min = valor;
						currentSolution = vz[i];
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
		currentSolution = generateRandomSolution();
	}
	
	printf("melhor solucao busca local = %d rodadas: %d\n", best,rodadas);
	clock_t fim = std::clock();
	float tempo = float(fim - inicio) / CLOCKS_PER_SEC;
	printf("Tempo da busca local: %lf\n", tempo);



	return bestSolution;
}

//Implementa Simulated Annealing
std::vector<int> Solver::SimAnn(int p , int k)
{
	clock_t inicio = std::clock();
	int indice = ((_node_number )%6) + 6;
	std::vector<int> inicialSolution;
	std::vector<int> currentSolution;
	std::vector<int> bestSolution;
	//printf("Indice: %d\n", indice);
	//printf("NodeNumber: %d\n", _node_number);
	inicialSolution = dfs_latencia(0,7,5);
	int rodadas = 0;
	int count = 0;
	int min, minAnterior, best;
	float coolingRate = 0.002;
	currentSolution = inicialSolution;
	bestSolution = currentSolution;
	best = funcaoObjetiva(currentSolution);
	for (int j = 0; j < 2;j++)
	{
		double T = 1000;
		int cont = 0;
		while (T > 1)
		{
			min = funcaoObjetiva(currentSolution);
			minAnterior = min;
			std::vector<std::vector<int>> vz = CalculaVizinhanca2Opt(currentSolution);

			for (int i = 0; i < vz.size(); i++)
			{
				int valor = funcaoObjetiva(vz[i]);
				if (valor < min)
				{
					min = valor;
					currentSolution = vz[i];					
				}
				else
				{
					float deltaE = valor - min;
					float p;
					float aux = (-deltaE) / T;
					p = exp(aux);
					float num = (float)rand() / (RAND_MAX / 100.0);
					//Se tiver dentro da probabilidade aceita vizinho pior
					if (num <= p * 100)
					{
						min = valor;
						currentSolution = vz[i];
						cont++;

						break;
					}
				}
			}
			if (min == minAnterior)
			{
				vz = CalculaVizinhancaSwap(currentSolution);
				for (int i = 0; i < vz.size(); i++)
				{
					int valor = funcaoObjetiva(vz[i]);
					if (valor < min)
					{
						min = valor;
						currentSolution = vz[i];
					}

				}
			}
			rodadas++;
			if (best > min)
			{
				best = min;
				bestSolution = currentSolution;
			}
			T *= 1 - coolingRate;
		}
	}

	printf("melhor solucao annealing = %d rodadas: %d\n", best, rodadas);

	clock_t fim = std::clock();
	float tempo = float(fim - inicio) / CLOCKS_PER_SEC;
	printf("Tempo da Simulated Annealing: %lf\n", tempo);
	return bestSolution;
}



Solver::~Solver()
{
}
