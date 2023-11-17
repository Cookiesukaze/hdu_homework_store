all: create write newfifo pipe
create :
	gcc create.c -o create -pthread
write :
	gcc write.c -o write -pthread
newfifo :
	gcc newfifo.c -o newfifo -pthread
pipe:
	gcc pipe.c -o pipe -pthread 	
clean :
	rm create write newfifo pipe