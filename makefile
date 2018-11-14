client: login.o clientTCP.o SQLite.o
	gcc -o client clientTCP.o login.o SQLite.o -lsqlite3

login.o: src/login.c src/login.h
	gcc -c -lncurses src/login.c
clientTCP.o: src/clientTCP.c #src/clientTCP.h
	gcc -c src/clientTCP.c

# log: login.o SQLite.o
# 	gcc -o log SQLite.o login.o -l sqlite3 -lncurses

SQLite.o: src/SQLite.c src/SQLite.h
	gcc -c src/SQLite.c

# login.o:
# clean:
# 	rm -f  *.o *~
