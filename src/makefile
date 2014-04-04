CC=g++
CFLAGS=
LDFLAGS=`pkg-config --libs --cflags opencv  --cflags libcurl` -ldl -lm -lglib-2.0
SOURCES=$(wildcard *.cpp)
HEADERS=$(wildcard *.h)
OBJECTS=$(SOURCES:.cpp=.o)
OBJ=./
EXECUTABLE=tracker

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(EXECUTABLE)


%.o : %.cc
	$(CC) -MD -c -o $@ $<
	@cp $*.d $*.P; \
		sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		                 -e '/^$$/ d' -e 's/$$/ :/' < $*.d >> $*.P; \
		rm -f $*.d

-include *.P


clean:
	@rm *.o *.P *~ tracker


