
all: csf

csf: csf.c
	gcc -lm -Wall -Werror -o csf csf.c
