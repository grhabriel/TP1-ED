# ==========================================
# Pastas
# ==========================================
SRC_FOLDER     := src
OBJ_FOLDER     := obj
BIN_FOLDER     := bin
INCLUDE_FOLDER := include

# ==========================================
# Compilador e flags
# ==========================================
CC       := gcc
CFLAGS   := -std=c99 -Wall -Wextra -g
CPPFLAGS := -I$(INCLUDE_FOLDER) -MMD -MP
LDLIBS   := -lm

# ==========================================
# Nome do executável (OBRIGATÓRIO)
# ==========================================
TARGET := tp1.out
EXEC   := $(BIN_FOLDER)/$(TARGET)

# ==========================================
# Arquivos
# ==========================================
SRC := $(wildcard $(SRC_FOLDER)/*.c)
OBJ := $(patsubst $(SRC_FOLDER)/%.c, $(OBJ_FOLDER)/%.o, $(SRC))
DEP := $(OBJ:.o=.d)

# ==========================================
# Regras principais
# ==========================================
all: $(EXEC)

# Ligação (link-editing) do executável
$(EXEC): $(OBJ) | $(BIN_FOLDER)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

# Compilação dos objetos e geração de dependências (.d)
$(OBJ_FOLDER)/%.o: $(SRC_FOLDER)/%.c | $(OBJ_FOLDER)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Criação das pastas caso não existam
$(BIN_FOLDER) $(OBJ_FOLDER):
	mkdir -p $@

# Inclui os arquivos de dependências de headers gerados pelo gcc
-include $(DEP)

# ==========================================
# Execução e Depuração
# ==========================================
run: $(EXEC)
	./$(EXEC)

valgrind: $(EXEC)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(EXEC)

# ==========================================
# Limpeza e Rebuild
# ==========================================
clean:
	rm -f $(OBJ_FOLDER)/*.o $(OBJ_FOLDER)/*.d $(EXEC)

rebuild: clean all

.PHONY: all clean rebuild run valgrind
