CXX	= g++

all:
	$(CXX) -lcurl -lpthread 01/main.cpp -o teiba
	$(CXX) -lcurl -lpthread 02/main.cpp -o teiba_multi
