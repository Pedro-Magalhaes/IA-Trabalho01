#include "Solver.h"
#include <random>
#include <algorithm>
#include <iostream>
#include <chrono>
#include "math.h"
#include <climits>
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
	std::vector<matrixRepresentation::edge> aux(caminho);

	std::sort(caminho.begin(), caminho.end(), [](matrixRepresentation::edge a, matrixRepresentation::edge b) {
		if (a.from < b.from)
		{
			return true;
		}
		if (a.from == b.from)
		{
			return a.weight < b.weight;
		}
		
		return false ;
	} );

	std::sort( aux.begin(),aux.end(),[] (matrixRepresentation::edge a,matrixRepresentation::edge b){
		if(a.to < b.to)
		{
			return true;	
		}
		if (a.to == b.to)
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
	pathA.push_back(currentEdge.from);
	count = 1;
	while (count < n + 1)
	{
		pathA.push_back(currentEdge.to);
		currentEdge = caminho[currentEdge.to];
		count++;
	}

	// for(int i = 0; i < path.size(); i++)
	// {
	// 	printf(" %d >",path[i]);
	// }
	// printf("\n");
	// printf("peso caminho ord por from: %d\n",this->funcaoObjetiva(pathA));

	currentEdge = aux[0];
	pathB.push_back(currentEdge.to);
	count = 1;
	while(count < n+1)
	{
		pathB.push_back(currentEdge.from);
		currentEdge = aux[currentEdge.from];
		count++;
	}
	// printf("peso caminho ord por to: %d\n",this->funcaoObjetiva(pathB));
	return this->funcaoObjetiva(pathB) < funcaoObjetiva(pathA)? pathB : pathA;
}


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
// s == depot , p == profundidade maxima
std::vector<int> Solver::dfs(int s, int p) //busca por profundidade => acho que t
{
	int latencia;
	
	int latMelhorCandidato = INT_MAX;
	std::vector<Color> inPath(_node_number,Color::WHITE);
	int melhorLatencia = INT_MAX;
	std::vector<int> path;
	path.reserve(_node_number + 1);
	path.emplace_back(s); // depot
	inPath[s] = Color::BLACK;
	int melhorCandidato = 0;
	for(unsigned int i = 1; i<_m.getNodeNumber(); i++)
	{
		printf("0\n");
		melhorLatencia = INT_MAX;
		latMelhorCandidato = INT_MAX;
		printf("1\n");
		std::vector<std::pair<int,int>> melhoresVizinhos = _m.getSortedNeighbours(melhorCandidato);
		printf("2\n");
		for(unsigned int j = 1; j < melhoresVizinhos.size(); j++ )
		{
			if(inPath[melhoresVizinhos[j].first] == Color::WHITE)
			{
				latencia = dfs_visit(melhoresVizinhos[j].first, p-1,melhoresVizinhos[j].second,&melhorLatencia,inPath,i);
			//printf("testando para o vizinho: %d, peso da aresta: %d, latencia retornada: %d\n ",melhoresVizinhos[j].first,melhoresVizinhos[j].second,latencia);
			if(latencia < latMelhorCandidato)
			{
				latMelhorCandidato = latencia;
				melhorCandidato = melhoresVizinhos[j].first;
			}
			//printf("melhor C: %d \n",melhorCandidato);
			}
			
		}
		inPath[melhorCandidato] = Color::BLACK;
		path.emplace_back(melhorCandidato);
	}
	path.emplace_back(s); //back depot
	printf("Latencia do path dfs: %d melhor latencia %d, size path %u\n",funcaoObjetiva(path),melhorLatencia,path.size());
	// for(auto i = 0; i < path.size(); i++)
	// {
	// 	printf(" %d > ",path[i]);
	// }
	// printf(" \n ");
	return path;
}
// vai retornar o id do no com caminho mais barato e a latencia ate p
int Solver::dfs_visit(int u, int p, int latencia, int * melhorLatencia, std::vector<Color> inPath, int pAtual)
{
	inPath[u] = Color::GRAY;
	if(latencia > *melhorLatencia) // ja nao eh um caminho viavel (podamos)
	{
		//printf("Poda em no %d , prof %d\n",u,p);
		return INT_MAX;
	}
	if (p == 0)
	{
		if(latencia < *melhorLatencia)
		{
			//printf("settando retornado p==0 %d, u %d\n", latencia,u);
			*melhorLatencia = latencia;
		}
		//printf("Latencia retornado p==0 %d\n", latencia);
		return latencia;
	}
	if(pAtual == _node_number - 1) // chegou no ultimo no tem que contar o custo do deposito
	{
		for(unsigned int i = 0; i< inPath.size( ); i++ ) // Debbug
		{
			if(inPath[i] == Color::WHITE)
			{
				printf("erro alguem ainda costa como nao vizitado: no %d\n",i);
			}
		}
		latencia = latencia +_m.valueat(u,0);
		if(*melhorLatencia > latencia)
		{
			*melhorLatencia = latencia;
		}
		//printf("Latencia retornado pnode number atingido %d\n", latencia);
		return latencia;
	}

	int thisLat;
	int bestLat = INT_MAX;
	auto vizinhosOrd = _m.getSortedNeighbours(u);
	unsigned int i = 1; // 1 porque o 1 eh ele mesmo
	int latenciaAtual = latencia;
	
	// como vai estar ordenado se a latencia atual + peso da aresta de u for maior
	// que a melhor lat. encontrada ate o momento, nao precisamos percorrer o resto
	while(i < vizinhosOrd.size() )
	{
		if(inPath[vizinhosOrd[i].first] == Color::WHITE) 
		{
			latenciaAtual = latencia + vizinhosOrd[i].second;
			
			thisLat = dfs_visit(vizinhosOrd[i].first, p-1,latenciaAtual,melhorLatencia,inPath,pAtual+1);
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

//Implementa uma Busca com aprofundamento iterativo com poda
std::vector<int> Solver::BAHIA()
{
	int i,p;
	p = 3;
	//recursivooooo
	return std::vector<int>();
}
std::vector<std::vector<int>> Solver::CalculaVizinhancaSwap(std::vector<int> &sol, int limit)
{
	if (limit == 0)
	{
		limit = this->_node_number;
	}
	std::vector<std::vector<int>> vizinhanca;
	for (int i = 1; i < limit+1; i++)
	{
		// TODO: fazer uma vizinha�a olhando o elemento e ver qual a menor aresta, tentando posiciona-lo ao lado desse vizinho
		for (int j = i+1; j < limit; j++)
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


std::vector<std::vector<int>> Solver::CalculaVizinhancakOpt(std::vector<int> &sol,int r)
{
	unsigned int n = sol.size();
	std::vector<std::vector<int>> vizinhanca;
	//for (int i = 1; i < sol.size() - k; i++) 
	//{
	//	std::vector<int > vAux(sol);
	//	int count = 1;
	//	for (int j = i; j < i + k; j++)
	//	{
	//		vAux[j] = sol[i + k - count];
	//			count++;
	//	}
	//	/*for (int t = 0; t < vAux.size(); t++)
	//	{
	//		printf(" %d ", vAux[t]);
	//	}
	//	printf("\n\n");*/
	//}

	for (int i = 1; i < n - 2; i++) {
		for (int k = i + 1; k < n-1 ; k++) {
			vizinhanca.emplace_back(TwoOptSwap(sol, i, k));
		}
	}

	return vizinhanca;
}
void printasolucao(std::vector<int> v)
{
	for (int i = 0; i < v.size(); i++)
	{
		printf("%d ", v[i]);
	}
	printf("\n");
}

//Implementa uma Busca Local (Hill Climbing??)
std::vector<int> Solver::BuscaLocal()
{
	std::vector<int> inicialSolution;
	std::vector<int> currentSolution;
	std::vector<int> bestSolution;
	inicialSolution = dfs(0,4);//solucaoKruskalAdaptada();
	std::vector<int> test = {0,1,2,3,4,5,0};
	//CalculaVizinhancakOpt(test, 2);
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
	for (int j = 0; j < 10; j++)
	{
		while (count < 1)
		{
			min = funcaoObjetiva(currentSolution);
			minAnterior = min;
			std::vector<std::vector<int>> vz = CalculaVizinhancakOpt(currentSolution); //CalculaVizinhancaSwap(currentSolution);
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
		currentSolution = generateRandomSolution();
	}
	

//	std::sort(bestSolution.begin(), bestSolution.end());
	printf("melhor solucao = %d rodadas: %d\n", best,rodadas);
	// printf("Vetor Encontrado:\n");
	// for (auto i = 0; i < bestSolution.size(); i++)
	// {
	// 	printf(" %d -> ",bestSolution[i]);
	// }



	return bestSolution;
}
//Implementa Simulated Annealing
std::vector<int> Solver::SimAnn()
{
	std::vector<int> inicialSolution;
	std::vector<int> currentSolution;
	std::vector<int> bestSolution;
	inicialSolution = dfs(0,4);//solucaoKruskalAdaptada();
	//std::vector<int> test = { 0,1,2,3,4,5,6,0 };
	//std::vector<std::vector<int>> ga = CalculaVizinhancakOpt(test, 2);

	//for (int i = 0; i < ga.size(); i++)
	//{
	//	printasolucao(ga[i]);
	//}

	//return bestSolution;
	//for (int i = 0; i < this->_node_number; i++)
	//{
	//	printf(" %d, ", inicialSolution[i]);
	//}
	//printf("\n");
	//printf("fo: %d\n", funcaoObjetiva(inicialSolution));


	//printf("solucao inicial = %d \n", min);
	int rodadas = 0;
	int count = 0;
	int min, minAnterior, best;
	float coolingRate = 0.002;
	currentSolution = inicialSolution;
	bestSolution = currentSolution;
	best = funcaoObjetiva(currentSolution);
	//for (int j = 0; j < 10; j++)
	//{
		double T = 10000;
		int cont = 0;
		while (T > 1)
		{
			min = funcaoObjetiva(currentSolution);
			minAnterior = min;
			std::vector<std::vector<int>> vz = CalculaVizinhancakOpt(currentSolution, 2);
			
			for (int i = 0; i < vz.size(); i++)
			{
				int valor = funcaoObjetiva(vz[i]);
				if (valor < min)
				{
					min = valor;
					currentSolution = vz[i];
					if (currentSolution.back() != 0)
					{
						printf("Erro kopt\n");
					}
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
						if (currentSolution.back() != 0)
						{
							printf("Erro swap\n");
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
							if (currentSolution.back() != 0)
							{
								printf("Erro relocate\n");
							}
						}
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
		//printf("T: %lf\n", T);

	}
	printf("Entrei %d\n", cont);


	//	std::sort(bestSolution.begin(), bestSolution.end());
	printf("melhor solucao = %d rodadas: %d\n", best, rodadas);
	// printf("Vetor Encontrado:\n");
	// for (auto i = 0; i < bestSolution.size(); i++)
	// {
	// 	printf(" %d -> ", bestSolution[i]);
	// }


	return bestSolution;
}



Solver::~Solver()
{
}
