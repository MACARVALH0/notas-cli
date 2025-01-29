# Variáveis
CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -Ilib -mconsole
LDFLAGS = -Llib -lsqlite3
SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst src/%.cpp, build/%.o, $(SRC))
OUT = build/main

# Regra padrão
all: build_dir $(OUT)

# Regra para criar o executável
$(OUT): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $@ $(LDFLAGS)

# Regra para compilar arquivos .cpp em .o
build/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regra para criar o diretório build
build_dir:
	if not exist build mkdir build

# Regra para executar a aplicação
run: $(OUT)
	 ./$(OUT).exe

# Limpar arquivos gerados
clean:
	if exist build/*.o del /Q build\*.o
	if exist build\main.exe del /Q build\main.exe
	if exist build rmdir /S /Q build
