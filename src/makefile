CC=g++
CFLAGS=
LDFLAGS=`pkg-config --libs --cflags opencv  --cflags libcurl` -ldl -lm -lglib-2.0
SOURCES=CameraControl.cpp MovingAlgorithm.cpp SearchingAlgorithm.cpp testPrecision.cpp utils.cpp main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
OBJ=./
EXECUTABLE=tracker

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(EXECUTABLE)

CameraControl.o: CameraControl.h utils.h
	$(CC) -c $(CFLAGS) CameraControl.cpp -o $(OBJ)$@

main.o: CameraControl.h MovingAlgorithm.h SearchingAlgorithm.h utils.h testPrecision.h
	$(CC) -c $(CFLAGS) main.cpp -o $(OBJ)$@

MovingAlgorithm.o: MovingAlgorithm.h CameraControl.h
	$(CC) -c $(CFLAGS) MovingAlgorithm.cpp -o $(OBJ)$@

SearchingAlgorithm.o: SearchingAlgorithm.h MovingAlgorithm.h CameraControl.h
	$(CC) -c $(CFLAGS) SearchingAlgorithm.cpp -o $(OBJ)$@

testPrecision.o: testPrecision.h CameraControl.h utils.h
	$(CC) -c $(CFLAGS) testPrecision.cpp -o $(OBJ)$@

utils.o: utils.h
	$(CC) -c $(CFLAGS) utils.cpp -o $(OBJ)$@

all: $(SOURCES) $(EXECUTABLE)

.cpp.o:
		$(CC) $(CFLAGS) $< -o $@

clean:
	     \rm *.o *~ tracker

