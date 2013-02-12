main: header_unittest.o
	g++ -o $@ $^ -lgtest -lgtest_main -lpthread
header_unittest.o: header_unittest.cpp header.h
	g++ -std=c++11 -c -o $@ $<

