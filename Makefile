
DATA=tmp.data
SOURCE=csf.c

all: edf

edf:
	gcc -lm -Wall -o $@ $(SOURCE)

rm:
	gcc -lm -Wall -o $@ $(SOURCE) -DRM

edf.png:
	gcc -lm -Wall -o csf $(SOURCE) -DSOLUTION_SPACE

	rm -f $(DATA)
	./csf > $(DATA)
	gnuplot edf.p
	rm -f $(DATA) csf

rm.png:
	gcc -lm -Wall -o csf $(SOURCE) -DRM -DSOLUTION_SPACE

	rm -f $(DATA)
	./csf > $(DATA)
	gnuplot rm.p
	rm -f $(DATA) csf

clean:
	rm -f edf rm edf.png rm.png

.PHONY: edf rm edf.png rm.png
