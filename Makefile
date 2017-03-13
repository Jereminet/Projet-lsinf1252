build :
		gcc -g -Wall -W -Werror -std=gnu99 -c matrix.c
		gcc -g -Wall -W -Werror -std=gnu99 -c test.c -lcunit -I$(HOME)/local/include -L$(HOME)/local/lib  matrix.c
		gcc -g -Wall -W -Werror -std=gnu99 -o test test.o matrix.o -lcunit -I$(HOME)/local/include -L$(HOME)/local/lib
