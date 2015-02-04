
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

u_edf.png:
	gcc -lm -Wall -o u_edf utilization.c

	rm -f $(DATA)
	./u_edf > $(DATA)
	gnuplot u_edf.p
	rm -f $(DATA) u_edf

u_rm.png:
	gcc -lm -Wall -o u_rm utilization.c -DRM

	rm -f $(DATA)
	./u_rm > $(DATA)
	gnuplot u_rm.p
	rm -f $(DATA) u_rm

clean:
	rm -f edf rm edf.png rm.png u_edf.png u_rm.png

.PHONY: edf rm edf.png rm.png u_edf.png u_rm.png
