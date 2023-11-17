all: send1 send2 receive
send: send1.c
	gcc send1.c -lpthread -o send1
send2: send2.c
	gcc send2.c -lpthread -o send2
receive: receive.c
	gcc receive.c -lpthread -o receive
clean:
	rm send1 send2 receive
