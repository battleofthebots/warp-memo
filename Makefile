compile:
	gcc uaf.c -no-pie -o uaf

hacknote:notes.c
	gcc -m32 -no-pie notes.c -o notes

clean:
	rm notes