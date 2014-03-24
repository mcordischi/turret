CC=g++
CFLAGS=-c `pkg-config --libs --cflags opencv  --cflags libcurl` -ldl -lm -lglib-2.0
LDFLAGS=`pkg-config --libs --cflags opencv  --cflags libcurl` -ldl -lm -lglib-2.0
SOURCES=CameraControl.cpp MovingAlgorithm.cpp SearchingAlgorithm.cpp testPrecision.cpp utils.cpp main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=tracker

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) -o $(EXECUTABLE)

CameraControl.o: CameraControl.h
	$(CC) $(CFLAGS) $% $^ -o $@

main.o: CameraControl.h MovingAlgorithm.h SearchingAlgorithm.h utils.h testPrecision.h
	$(CC) $(CFLAGS) $% $^ -o $@

MovingAlgorithm.o: MovingAlgorithm.h CameraControl.h
	$(CC) $(CFLAGS) $% $^ -o $@

SearchingAlgorithm.o: SearchingAlgorithm.h MovingAlgorithm.h CameraControl.h
	$(CC) $(CFLAGS) $% $^ -o $@

testPrecision.o: testPrecision.h CameraControl.h utils.h
	$(CC) $(CFLAGS) $% $^ -o $@

utils.o: utils.h /usr/include/unistd.h /usr/include/features.h
	$(CC) $(CFLAGS) $% $^ -o $@


all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
		$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
		$(CC) $(CFLAGS) $< -o $@

clean:
	     \rm *.o *~ tracker

