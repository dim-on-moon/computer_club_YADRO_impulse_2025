CXX = g++
# Флаги для компиляции
CXXFLAGS = -Wall -Wextra -std=c++17 -I./include -MMD -MP
TEST_CXXFLAGS = $(CXXFLAGS) -I./src -I/usr/include/gtest -I/usr/include/gmock
# Флаги линковки для тестов
TEST_LDFLAGS = -lgtest -lgtest_main -lgmock -lpthread

SRC_DIR = src
INCLUDE_DIR = include
TEST_DIR = tests
OBJ_DIR = obj
BIN_DIR = bin
BIN_TEST_DIR = $(BIN_DIR)/tests

MAIN_SRC = $(SRC_DIR)/main.cpp
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
SRC_FILES_NO_MAIN := $(filter-out $(MAIN_SRC), $(SRC_FILES))

TEST_SRC = $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJ = $(patsubst $(TEST_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(TEST_SRC))
TEST_BIN = $(patsubst $(TEST_DIR)/%.cpp, $(BIN_TEST_DIR)/%, $(TEST_SRC))

OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))
OBJ_FILES_NO_MAIN := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES_NO_MAIN))

MAIN_OBJ = $(OBJ_DIR)/main.o
MAIN_BIN = $(BIN_DIR)/main

# Цель по умолчанию
all: $(MAIN_BIN) $(TEST_BIN)

# Создание необходимых директорий
dirs:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR) $(BIN_TEST_DIR)

# Генерация зависимостей
DEP_FILES = $(OBJ_FILES:.o=.d) $(TEST_OBJ:.o=.d)
-include $(DEP_FILES)

# Компиляция исходников в объектные файлы
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | dirs
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Компиляция main.o отдельно
$(MAIN_OBJ): $(MAIN_SRC) | dirs
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Сборка основной программы (main)
$(MAIN_BIN): $(MAIN_OBJ) $(OBJ_FILES_NO_MAIN) | dirs
	$(CXX) $^ -o $@ $(LDFLAGS)

# Компиляция исходников тестов в объектные файлы
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp | dirs
	$(CXX) $(TEST_CXXFLAGS) -c $< -o $@

# Сборка каждого теста
$(BIN_TEST_DIR)/%: $(OBJ_DIR)/%.o $(OBJ_FILES_NO_MAIN) | dirs
	$(CXX) $^ -o $@ $(TEST_LDFLAGS)	

# Запуск всех юнит-тестов
run_tests: $(TEST_BIN)
	@for bin in $(TEST_BIN); do echo "Running $$bin..."; ./$$bin || exit 1; done

# Переменная для имени входного файла
INPUT_FILE ?= input.txt

# Запуск основной программы
run: $(MAIN_BIN)
	./$(MAIN_BIN) $(INPUT_FILE)

# Запуск программы на тестовых входных данных
INPUT_EXAMPLES_DIR = input_examples
OUTPUT_EXAMPLES_DIR = output_examples

TEST_FILES = $(wildcard $(INPUT_EXAMPLES_DIR)/*.txt)

# Цель для тестирования на наборе файлов
test_files: $(MAIN_BIN)
	@echo "Running file-based tests..."
	@passed=0; failed=0; \
	for input_file in $(TEST_FILES); do \
		base=$$(basename $$input_file .txt); \
		expected_file=$(OUTPUT_EXAMPLES_DIR)/$${base}_expected.txt; \
		output_file=$(OUTPUT_EXAMPLES_DIR)/$${base}_output.txt; \
		if [ ! -f $$expected_file ]; then \
			echo "SKIP: $$input_file (no $$expected_file)"; \
			continue; \
		fi; \
		./$(MAIN_BIN) $$input_file > $$output_file 2>/dev/null; \
		if diff -q $$output_file $$expected_file > /dev/null; then \
			echo "PASS: $$input_file"; \
			passed=$$((passed + 1)); \
		else \
			echo "FAIL: $$input_file"; \
			echo "Differences:"; \
			diff $$output_file $$expected_file; \
			failed=$$((failed + 1)); \
		fi; \
		rm -f $$output_file; \
	done; \
	echo "Summary: $$passed passed, $$failed failed"; \
	if [ $$failed -ne 0 ]; then \
		false; \
	fi

# Общая цель для всех тестов
check: run_tests test_files

# Очистка
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all run run_tests clean dirs test_files check
