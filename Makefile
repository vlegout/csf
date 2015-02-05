
DATA=tmp.data

SOURCE=csf.c
SOURCE_U=utilization.c

FLAGS=-Wall -lm

all: edf

full: edf.png rm.png u_edf.png u_rm.png

edf:
	gcc $(FLAGS) -o $@ $(SOURCE)

rm:
	gcc $(FLAGS) -o $@ $(SOURCE) -DRM

edf.png:
	gcc $(FLAGS) -o csf $(SOURCE) -DSOLUTION_SPACE

	rm -f $(DATA)
	./csf > $(DATA)
	gnuplot edf.p
	rm -f $(DATA) csf

rm.png:
	gcc $(FLAGS) -o csf $(SOURCE) -DRM -DSOLUTION_SPACE

	rm -f $(DATA)
	./csf > $(DATA)
	gnuplot rm.p
	rm -f $(DATA) csf

u_edf.png:
	gcc $(FLAGS) -o u_edf $(SOURCE_U)

	rm -f $(DATA)
	./u_edf > $(DATA)
	gnuplot u_edf.p
	rm -f $(DATA) u_edf

u_rm.png:
	gcc $(FLAGS) -o u_rm $(SOURCE_U) -DRM

	rm -f $(DATA)
	./u_rm > $(DATA)
	gnuplot u_rm.p
	rm -f $(DATA) u_rm

clean:
	rm -f edf rm edf.png rm.png u_edf.png u_rm.png

.PHONY: edf rm edf.png rm.png u_edf.png u_rm.png
