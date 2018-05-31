RSA: rsaP.o
	g++ -o rsaP rsaP.o
RSA.o: rsaP.cpp
	g++ -fopenmp rsaP.cpp -o rsaP

clean:
	rm -f rsaP.o rsaP

run:
	./rsaP
