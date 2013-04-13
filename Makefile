all:
	g++ MIPSsim.cpp -o MIPSsim

test:
	clang++ -g test.cc -o test

run:
	./MIPSsim sample.txt
