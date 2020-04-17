
build:
	gcc -Wall teme.c -o teme
	gcc -Wall magic.c -o magic

run-p1:
	./teme

run-p2:
	./magic

clean:
	rm teme
	rm magic
