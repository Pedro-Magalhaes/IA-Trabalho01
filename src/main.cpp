#include <iostream>
#include <regex>
#include <vector>
#include <string>
#include <cerrno>
#include <fstream>


/*Abre o arquivo e armazena o conteudo inteiro do texto em uma string
    Mais eficiente para fazer o parsing e gerar a matriz*/
std::string get_file_contents(const char *filename)
{
  std::ifstream in(filename, std::ios::in | std::ios::binary);
  if (in)
  {
    std::string contents;
    in.seekg(0, std::ios::end);
    contents.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], contents.size());
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
    if(std::regex_search(filecontent,m,std::regex("EDGE_WEIGHT_SECTION[\s]*\n[\s]*"))) { // encontra a string com "EDGE_WEIGHT_SECTION" incluindo a mudança de linha
        pos = m.position()+m.length(); // retorna a posição do fim da string
    } 
    return pos;
}


int main () {

    std::vector<std::string> files = {"dataset/brazil58.tsp","dataset/dantzig42.tsp","dataset/gr48.tsp","dataset/gr120.tsp","dataset/pa561.tsp"};
    std::string fileContent = "";
    int pos=0;
    for (size_t i = 0; i < files.size(); i++) {
        
        try
        {
            fileContent = get_file_contents(files[i].c_str());
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        pos = getBeginOfMatrix(fileContent);
        std::cout<<files[i]<<" posicao do primeiro digito "<<pos<<"digito :"<<fileContent.at(pos)<<std::endl;
    }
    

    return 0;
}