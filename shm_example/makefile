make : 
	gcc producer.c -pthread -lrt -o producer
	gcc consumer.c -pthread -lrt -o consumer

.PHONY : clean
clean :
	rm -f producer
	rm -f comsumer
