CC=gcc
FLAGS=-no-pie -s
compile:
	$(CC) $(FLAGS) notes.c -o warp-memo

clean:
	rm warp-memo
