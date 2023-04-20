all:
	g++ -I . main.cpp -o program -std=c++20 -lpqxx
example:
	g++ -I . example.cpp -o program -std=c++20 -lpqxx
