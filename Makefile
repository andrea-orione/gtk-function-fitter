PROG = fitter

CC = g++
CFLAGS = `pkg-config gtkmm-3.0 --cflags` `root-config --cflags --ldflags`
LIBS = `pkg-config gtkmm-3.0 --libs` `root-config --glibs`

OBJECTS = fitter.o window.o fileDialog.o datiDialog.o entryOnlyNumbers.o

all: ${PROG}

${PROG}: ${OBJECTS}
	${CC} ${OBJECTS} -o ${PROG} ${CFLAGS} ${LIBS}

%.o: %.cpp
	${CC} -c $< ${CFLAGS} ${LIBS}

clean:
	rm -f *.o ${PROG}

fitter.o: fitter.cpp window.o
window.o: window.cpp fileDialog.o datiDialog.o entryOnlyNumbers.o
fileDialog.o: fileDialog.cpp
datiDialog.o: datiDialog.cpp
entryOnlyNumbers.o: entryOnlyNumbers.cpp