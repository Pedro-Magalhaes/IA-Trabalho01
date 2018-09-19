#pragma once
#include <vector>

#include "matrixRepresentation.h"


class Solver
{
private:
	std::vector<int> generateRandomSolution();
	int funcaoObjetiva(std::vector<int> & sol);
public:
	Solver();
	Solver(int n,matrixRepresentation & m);
	std::vector<int> BAHIA();
	std::vector<std::vector<int>> CalculaVizinhanca(std::vector<int>& sol);
	std::vector<int> BuscaLocal();
	~Solver();

private:
	matrixRepresentation _m;
	std::vector<int> _finalSolution;
	int _node_number;
};

