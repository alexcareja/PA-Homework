
build:
	gcc -Wall teme.c -o teme
	gcc -Wall magic.c -o magic
	gcc -Wall ratisoare.c -o ratisoare
	gcc -Wall joc.c -o joc

run-p1:
	./teme

run-p2:
	./magic

run-p3:
	./ratisoare

run-p4:
	./joc

clean:
	rm teme
	rm magic
	rm ratisoare
	rm joc
