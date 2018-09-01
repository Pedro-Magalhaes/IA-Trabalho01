#### $@    Nome da regra. 
#### $<    Nome da primeira dependência 
#### $^ 	Lista de dependências
#### $? 	Lista de dependências mais recentes que a regra.
#### $* 	Nome do arquivo sem sufixo

CC=gcc
CXX=g++
CFLAGS=-W -Wall -std=c++14
LIBS=-lm
EXEC=buscaTSP
RM=rm -f
# todos os arquivos .cpp da pasta serao considerados src
SRC= $(wildcard src/*.cpp)
# todos os arquivos do SRC (.cpp) vao gerar um obj
OBJ= $(SRC:.cpp=.o)
all: $(EXEC)
buscaTSP:$(OBJ)
	$(CXX) -o $@ $^ $(LIBS)
%.o: %.cpp
	$(CXX) -o $@ -c $^ $(CFLAGS) $(LIBS)
	
clean:
	$(RM) $(OBJ)
	$(RM) $(EXEC)