# Variables
CXX  = g++
CXXFLAGS = -std=c++11 -Wall -pthread -Wextra -O3 -w
TARGET1 = simulapc
TARGET2 = mvirtual

# Archivos fuente
SRC1 = main.cpp Cola_Circular.cpp
SRC2 = part2.cpp

# Compilar todos
all: $(TARGET1) $(TARGET2)

# Regla para el primer ejecutable
$(TARGET1): $(SRC1)
	$(CXX) $(CXXFLAGS) $(SRC1) -o $(TARGET1)

# Regla para el segundo ejecutable
$(TARGET2): $(SRC2)
	$(CXX) $(CXXFLAGS) $(SRC2) -o $(TARGET2)

# Limpiar los ejecutables
clean:
	rm -f $(TARGET1) $(TARGET2)
