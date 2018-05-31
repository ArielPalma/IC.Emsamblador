RSA: rsaProceso.o
	mpic++ -o rsaProceso rsarsaProceso.o
RSAPROCESO.o: rsaProceso.cpp
	mpic++ rsaProceso.cpp -o rsaProceso

clean:
	rm -f rsaProceso.o rsaProceso

run:
	mpirun -mca plm_rsh_no_tree_spwan 1 -hostfile host.txt -n 4  ./rsaProceso
