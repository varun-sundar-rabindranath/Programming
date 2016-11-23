
PERF = perf stat -e cycles -e instructions -e cache-references -e cache-misses -e branches -e branch-misses -e L1-dcache-loads -e L1-dcache-load-misses

all : heap_sort

run : heap_sort
	echo "=========================================================="
	echo "Running heap sort --- "
	./heap_sort < ./input.txt
	echo "=========================================================="

perf_run : heap_sort
		echo "=========================================================="
		echo "Running heap sort --- "
		$(PERF) ./heap_sort < ./input.txt
		echo "=========================================================="

heap_sort : heap.cpp heap.h use_heap.cpp
		g++ -o heap_sort heap.cpp use_heap.cpp

perf_record_and_annotate : debug
		perf record ./heap_sort_debug < input.txt
		perf annotate -l

debug : heap_sort_debug

heap_sort_debug : heap.cpp heap.h use_heap.cpp
			g++ -g -o heap_sort_debug heap.cpp use_heap.cpp


clean :
	rm heap_sort
	rm heap_sort_debug
