
PERF = perf stat -e cycles -e instructions -e cache-references -e cache-misses -e branches -e branch-misses -e L1-dcache-loads -e L1-dcache-load-misses

all : djikstra_adjmat djikstra_adjlist

perf_record_and_annotate : debug
		perf record ./djikstra_adjlist_debug < input.txt
		perf annotate -l

debug : djikstra_adjmat_debug djikstra_adjlist_debug

run : djikstra_adjmat djikstra_adjlist
	echo "=========================================================="
	echo "Running adjmat version --- "
	./djikstra_adjmat < input.txt
	echo "Running adjlist version ---"
	./djikstra_adjlist < input.txt
	echo "=========================================================="

perf_run : djikstra_adjmat djikstra_adjlist
	    echo "=========================================================="
	    echo "Running adjmat version (perf) --- "
	    $(PERF) ./djikstra_adjmat < input.txt
	    echo "Running adjlist version (perf) ---"
	    $(PERF) ./djikstra_adjlist < input.txt
	    echo "=========================================================="

djikstra_adjmat : djikstra_shpath.cpp
		  g++ -o djikstra_adjmat djikstra_shpath.cpp
djikstra_adjlist : djikstra_shpath.cpp
		    g++ -o djikstra_adjlist -DUSE_EDGE_LIST djikstra_shpath.cpp

djikstra_adjmat_debug : djikstra_shpath.cpp
		  g++ -g -o djikstra_adjmat_debug djikstra_shpath.cpp
djikstra_adjlist_debug : djikstra_shpath.cpp
		    g++ -g -o djikstra_adjlist_debug -DUSE_EDGE_LIST djikstra_shpath.cpp

clean :
	rm djikstra_adjmat
	rm djikstra_adjlist
	rm djikstra_adjmat_debug
	rm djikstra_adjlist_debug
