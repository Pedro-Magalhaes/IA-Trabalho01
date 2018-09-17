#### MAKE PARA OS TESTES #### Por enquanto ainda esta errado mas por algum motivo funciona

#### $@    Nome da regra. 
#### $<    Nome da primeira dependência 
#### $^ 	Lista de dependências
#### $? 	Lista de dependências mais recentes que a regra.
#### $* 	Nome do arquivo sem sufixo

CC=gcc
CXX=g++
CFLAGS=-W -Wall -std=c++14
LIBS=-lm
EXEC=teste_matrixRepresentation
RM=rm

SRCDIR   = ../
OBJDIR   = ../../obj
BINDIR   = ../../bin

# todos os arquivos .cpp da pasta serao considerados src
SRCTEST= $(wildcard *.cpp)
# todos os arquivos do SRC (.cpp) vao gerar um obj
OBJ= $(SRCTEST:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
all: $(BINDIR)/$(EXEC)
$(BINDIR)/$(EXEC):$(OBJ)
	$(CXX) -o $@ $^ $(LIBS)
$(OBJ): $(OBJDIR)/%.o : %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@  $(LIBS)
	
clean:
	$(RM) $(OBJ)
	$(RM) $(EXEC)