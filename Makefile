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
RM=rm

SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin

# todos os arquivos .cpp da pasta serao considerados src
SRC= $(wildcard $(SRCDIR)/*.cpp)
# todos os arquivos do SRC (.cpp) vao gerar um obj
OBJ= $(SRC:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
all: $(BINDIR)/$(EXEC)
$(BINDIR)/$(EXEC):$(OBJ)
	$(CXX) -o $@ $^ $(LIBS)
$(OBJ): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@  $(LIBS)
	
clean:
	$(RM) $(OBJ)
	$(RM) $(EXEC)