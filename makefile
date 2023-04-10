all: 
	gcc -g -Wall slab_test_1.c slab.c slab.h
	./a.out
clean: 
	$(RM) ./a.out slab.h.gch