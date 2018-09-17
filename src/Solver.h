#pragma once
#include <vector>
class Solver
{
public:
	Solver();
	Solver(int n);
	std::vector<int> BAHIA();
	std::vector<int> BuscaLocal();
	~Solver();

private:
	std::vector<int> _finalSolution;
	int _node_number;
};

