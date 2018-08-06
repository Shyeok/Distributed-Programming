all:
	gcc -g -Wall -o linked_list linked_list.c
	gcc -g -Wall -o pth_ll_rwl pth_ll_rwl.c my_rand.c -lpthread
	gcc -g -Wall -o pth_ll pth_ll.c my_rand.c -lpthread
clean:
	rm linked_list
	rm pth_ll_rwl
	rm pth_ll
