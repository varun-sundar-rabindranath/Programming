
/* Define heap */
typedef struct min_heap {
  int heap_size;
  int array_length;
  int *array;

  void min_heapify(int idx);
  void build_min_heap();
  int extract_min();

  min_heap(int *_array, int _array_length) {
    array = _array;
    array_length = _array_length;
    heap_size = 0;
    build_min_heap();
  }

  min_heap() {
    array = 0x0;
    array_length = 0;
    heap_size = 0;
  }

}min_heap;

