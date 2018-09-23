#include <iostream>
#include <regex>
#include <vector>
#include <string>
#include <cerrno>
#include <fstream>
#include <sstream>

#include "Solver.h"
#include "matrixRepresentation.h"

/*
    Abre o arquivo e armazena o conteudo inteiro do texto em uma string,
    mais eficiente para fazer o parsing e gerar a matriz porque tudo vai estar em memoria
    http://insanecoding.blogspot.com/2011/11/how-to-read-in-file-in-c.html
*/
std::string get_file_contents(const char *filename)
{
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (in)
    {
        std::string contents;
        in.seekg(0, std::ios::end);             // leva o cursor do arquivo até final
        contents.resize(in.tellg());            // reserva espaço em memória pra caber todo o arquivo
        in.seekg(0, std::ios::beg);             //retorna o cursor pro inicio do arquivo
        in.read(&contents[0], contents.size()); // le o arquivo inteiro e coloca na memoria
        in.close();
        return (contents);
    }
    throw(errno);
}


/* Vai encontrar a posicao do primeiro numero na string e
    saberemos onde começar a leitura da matriz  */
int getBeginOfMatrix(std::string &filecontent)
{
    std::smatch m; //

    if (filecontent.empty())
    {
        return -1;
    }
    int pos = -1;
    if (std::regex_search(filecontent, m, std::regex("EDGE_WEIGHT_SECTION[.]*[[:cntrl:]][[:blank:]]*")))
    {                                    // encontra a string com "EDGE_WEIGHT_SECTION" incluindo a mudança de linha
        pos = m.position() + m.length(); // retorna a posição do fim da string
    }
    return pos;
}


/*
    Recebe a string com a descricao do problema e extrai o numero de cidades e
    o tipo de matriz do problema
*/

int getProblemData(unsigned int &size, MatrixType &mType, std::string &text)
{
    std::regex numNodeRegex = std::regex("DIMENSION:[[:blank:]]*([[:d:]]+)");
    std::regex matrixTypeRegex = std::regex("EDGE_WEIGHT_FORMAT:[[:blank:]]*([[:alpha:]_]+)");
    std::smatch m;

    if (std::regex_search(text, m, numNodeRegex))
    { // o regex_serch retorna em m[1] o numero
        std::stringstream ss(m[1].str());
        if (ss >> size)
        { // Conseguimos pegar o numero de nodes, vamos pegar o tipo de matriz
            if (std::regex_search(text, m, matrixTypeRegex))
            {
                if (m[1].str().compare("UPPER_ROW") == 0)
                {
                    std::cout << "padrao reconhecido UP_ROW: " << m[1] << std::endl;
                    mType = MatrixType::UPPER_ROW;
                }
                else if (m[1].str().compare("LOWER_DIAG_ROW") == 0)
                {
                    std::cout << "padrao reconhecido LW_ROW: " << m[1] << std::endl;
                    mType = MatrixType::LOWER_DIAG_ROW;
                }
                else
                {
                    mType = MatrixType::ERRO;
                    std::cout << "Modelo de matriz não implementado ou nao reconhecido: " << m[1] << std::endl;
                    return -1;
                }
            }
        }
    }
    else
    {
        std::cout << "Sem match numero " << text << std::endl;
        return -1;
    }

    if (size > 0 && mType != MatrixType::ERRO)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}


int main(int argc, char **argv)
{

    std::vector<std::string> files = {"dataset/brazil58.tsp","dataset/dantzig42.tsp" ,"dataset/gr48.tsp"/*,"dataset/gr120.tsp","dataset/pa561.tsp"*/};
	unsigned int nodeNumber = 0;
	matrixRepresentation problemMatrix;
	if (argc > 1)
	{
		files.clear();
		files.emplace_back(std::string(argv[1]));
		
	}
    for (size_t i = 0; i < files.size(); i++)
    {

        std::string fileContent = "";
        int beginOfMatrixPos = 0;
        
        
        std::string problemDescription;
        std::string problemData;
        MatrixType m;
        std::cout<<"Problem "<<files[i]<<std::endl;
        try
        {
            fileContent = get_file_contents(files[i].c_str());
            beginOfMatrixPos = getBeginOfMatrix(fileContent);
            problemDescription = fileContent.substr(0, beginOfMatrixPos);
            problemData = fileContent.substr(beginOfMatrixPos);
            if (getProblemData(nodeNumber, m, problemDescription) != 0)
            { // "!= 0" eh erro
                std::cout << "Erro lendo propriedades do problema " << files[i] << std::endl;
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
        problemMatrix = matrixRepresentation(nodeNumber, m);
        problemMatrix.readMatrixData(problemData);

        	Solver s(nodeNumber,problemMatrix);

        std::vector<int> aux;
        
        //aux = s.solucaoGulosa();
		//int best = INT_MAX;
		//int lat=0;
		//int p = 0;
		//std::vector<int> path;
		//for (int i = 0; i < 1; i++)
		//{
		//	printf("DFS latencia total jeff com p = %d: \n",7);
		//	path = s.dfs_latencia(0, 7,10);
		//	lat = s.funcaoObjetiva(path);
		//	if (best > lat)
		//	{
		//		best = lat;
		//		p = i;
		//	}
		//	
		//}
		//std::cout << "\n\nMelhor profundidade para dfs " << p << "valor encontrado para o problema " << files[i] << " valor: " << best << "\n" << std::endl;
        
        //printf("\n\tLocal:\n\n");
        //aux = s.BuscaLocal();
        printf("\n\tAnnealing:\n");
        aux = s.SimAnn();
    }

 
    return 0;
}