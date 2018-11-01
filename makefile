

log: login.o SQLite.o
	gcc -o log SQLite.o login.o -l sqlite3 -lncurses

SQLite.o: src/SQLite.c src/SQLite.h
	gcc -c src/SQLite.c

login.o: src/login.c src/login.h
	gcc -c -lncurses src/login.c

# login.o:
# clean:
# 	rm -f  *.o *~
