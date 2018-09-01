#include <iostream>
#include <regex>
#include <vector>
#include <string>
#include <cerrno>
#include <fstream>


enum MatrixType {LOWER_DIAG_ROW,UPPER_ROW,ERRO};  // Tipo de matriz descrita nos arquivos


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
    in.seekg(0, std::ios::end); // leva o cursor do arquivo até final
    contents.resize(in.tellg()); // reserva espaço em memória pra caber todo o arquivo
    in.seekg(0, std::ios::beg); //retorna o cursor pro inicio do arquivo
    in.read(&contents[0], contents.size()); // le o arquivo inteiro e coloca na memoria
    in.close();
    return(contents);
  }
  throw(errno);
}


/* Vai encontrar a posicao do primeiro numero na string e
    saberemos onde começar a leitura da matriz  */
int getBeginOfMatrix(std::string &filecontent) {
    std::smatch m; // 
    
    if(filecontent.empty()) {
        return -1;
    }
    int pos = -1;
    if(std::regex_search(filecontent,m,std::regex("EDGE_WEIGHT_SECTION[.]*[[:cntrl:]][[:blank:]]*"))) { // encontra a string com "EDGE_WEIGHT_SECTION" incluindo a mudança de linha
        pos = m.position()+m.length(); // retorna a posição do fim da string
    } 
    return pos;
}

/*
    Recebe a string com a descricao do problema e extrai o numero de cidades e
    o tipo de matriz do problema
*/

int getProblemData( unsigned int & size, MatrixType & mType, std::string & text ) {
    std::regex numNodeRegex = std::regex("DIMENSION:[[:blank:]]*([[:d:]]+)");
    std::regex matrixTypeRegex = std::regex("EDGE_WEIGHT_FORMAT:[[:blank:]]*([[:alpha:]_]+)");
    std::smatch m;
    
    if( std::regex_search( text,m,numNodeRegex) ) { // o regex_serch retorna em m[1] o numero
        std::stringstream ss(m[1].str());
        if( ss >> size  ) { // Conseguimos pegar o numero de nodes, vamos pegar o tipo de matriz
            if( std::regex_search( text,m,matrixTypeRegex) ) {
                if( m[1].str().compare("UPPER_ROW") ) {
                    mType = MatrixType::UPPER_ROW;
                } else if( m[1].str().compare("LOWER_DIAG_ROW") ) {
                    mType = MatrixType::LOWER_DIAG_ROW;
                } else {
                    std::cout<< "Modelo de matriz não implementado ou nao reconhecido: "<<m[1]<<std::endl;
                    return -1;
                }
            }
        }        
    } else {
        std::cout<<"Sem match numero "<< text <<std::endl;
        return -1;
    }

    
    if( size > 0 && mType != MatrixType::ERRO ) {
        return 0;
    } else {
        return -1;
    }
    
}

/*
    Aloca a matriz, por enquanto na forma de uma matriz realmente,
    A melhor implementacao seria em uma vetor de uma dimensao com
    um calculo de offset para simular uma matriz
    https://stackoverflow.com/questions/27380024/store-triangular-matrix-efficiently
*/
void alocateMatrix(std::vector< std::vector<int> > v, int numNodes) {
    int i = numNodes;
    v.resize(i);
    while( i > 0 ) {
        v[--i].resize(i+1); // +1 pois o elemento v[0] deve ter 1 posição e o v[n-1] n posicoes
    }
    std::cout<<"Size do vetor: "<<v.size()<<"Size do ultimo elemento "<<v.back().size()<<std::endl;
}

/*
    Pega os pesos de uma matriz inferior
*/
int getMatrixFromLower(std::vector< std::vector<int> > & weights,int nodeNumber,std::string & data) {
    return 0;
}

/*
    Pega os pesos de uma matriz superior
*/
int getMatrixFromUpper(std::vector< std::vector<int> > & weights,int nodeNumber,std::string & data) {
    return 0;    
}

int getWeightData( std::vector< std::vector<int> > & weights, int nodeNumber, MatrixType mType ,std::string & data ) {
    
    switch (mType)
    {
        case MatrixType::LOWER_DIAG_ROW :
            // getMatrixFromLower(weights,nodeNumber,data);
            break;
        case MatrixType::UPPER_ROW :
            // getMatrixFromUpper(weights,nodeNumber,data);
            break;
        default:
            break;
    }
    return 0;
}


int main () {

    std::vector<std::string> files = {/*"dataset/brazil58.tsp","dataset/dantzig42.tsp","dataset/gr48.tsp","dataset/gr120.tsp",*/"dataset/pa561.tsp"};
    std::string fileContent = "";
    int beginOfMatrixPos=0;
    std::vector< std::vector<int> > weights;
    for (size_t i = 0; i < files.size(); i++) {
        unsigned int nodeNumber = 0;
        MatrixType m; 
        try
        {
            fileContent = get_file_contents(files[i].c_str());
            beginOfMatrixPos = getBeginOfMatrix(fileContent);
            std::string problemDescription = fileContent.substr( 0, beginOfMatrixPos );
            std::string problemData = fileContent.substr(beginOfMatrixPos);
            fileContent.clear();
            if( getProblemData( nodeNumber, m, problemDescription ) != 0 ) { // "!= 0" eh erro
                std::cout<<"Erro lendo propriedades do problema "<<files[i]<<std::endl;
            }
            std::cout<<"Dados "<<files[i]<<":\n"<<"Nodes: "<<nodeNumber<<"\nMatrix: "<<m<<std::endl;
            alocateMatrix(weights,nodeNumber); // Aloca espaco para a matriz de adjacencia
            getWeightData(weights,nodeNumber,m,problemData); // Preenche a matriz com os pesos
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
       

    }
    

    return 0;
}