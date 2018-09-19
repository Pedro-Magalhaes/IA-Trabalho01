#include "Solver.h"
#include <random>
#include <algorithm>
#include <chrono>
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
		// TODO: fazer uma vizinhaça olhando o elemento e ver qual a menor aresta, tentando posiciona-lo ao lado desse vizinho
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
		// TODO: fazer uma vizinhaça olhando o elemento e ver qual a menor aresta, tentando posiciona-lo ao lado desse vizinho
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
