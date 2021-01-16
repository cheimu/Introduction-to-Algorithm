rndgraph:	rndgraph.c
	gcc -lm rndgraph.c -o `arch`/rndgraph

pub:	rndgraph
	cp i686/rndgraph /cse/abstract/ruzzo/bin/rndgraph

cq:	clique.c
	gcc -lm clique.c -o clique
	./clique 5 1 0 0
