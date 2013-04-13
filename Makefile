mips:
	clang++ -g p2.cc -o mips 

test:
	clang++ -g test.cc -o test

run:
	./mips sample.txt
