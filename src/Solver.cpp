#include "Solver.h"

Solver::Solver()
{

}
Solver::Solver(int n)
{
	_node_number = n;
	_finalSolution.resize(n + 1);
}

//Implementa uma Busca com aprofundamento iterativo com poda
std::vector<int> Solver::BAHIA()
{
	return std::vector<int>();
}

//Implementa uma Busca Local (Hill Climbing??)
std::vector<int> Solver::BuscaLocal()
{
	return std::vector<int>();
}


Solver::~Solver()
{
}
