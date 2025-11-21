CXX = g++
CXXFLAGS = -O3 -std=c++17

SRC = main.cpp wos.cpp
HDR = wos.h

all: wos_serial wos_omp

wos_serial: $(SRC) $(HDR)
	$(CXX) $(CXXFLAGS) -o wos_serial $(SRC)

wos_omp: $(SRC) $(HDR)
	$(CXX) $(CXXFLAGS) -fopenmp -o wos_omp $(SRC)

clean:
	rm -f wos_serial wos_omp