
DATA=tmp.data

all: csf

csf: csf.c
	gcc -lm -Wall -o csf csf.c

plot.png:
	gcc -lm -Wall -o csf csf.c -DSOLUTION_SPACE

	rm -f $(DATA)
	./csf > $(DATA)
	gnuplot csf.p
	rm -f $(DATA)

clean:
	rm -f csf csf.png
