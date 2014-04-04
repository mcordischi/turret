CC=g++
CFLAGS=
LDFLAGS=`pkg-config --libs --cflags opencv  --cflags libcurl` -ldl -lm -lglib-2.0
SOURCES=*.cpp
OBJECTS=$(SOURCES:.cpp=.o)
OBJ=./
EXECUTABLE=tracker

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(EXECUTABLE)

depend:
	makedepend -- $(CFLAGS) -- $(SOURCES)

clean:
	@rm *.o *~ tracker
# DO NOT DELETE

FoscamCameraControl.o: FoscamCameraControl.h utils.h AbstractCameraControl.h
HorizontalTracker.o: HorizontalTracker.h AbstractCameraControl.h utils.h
main.o: AbstractCameraControl.h utils.h AbstractTracker.h AbstractDetector.h
main.o: testPrecision.h FoscamCameraControl.h HorizontalTracker.h
main.o: OpenCVDetector.h
OpenCVDetector.o: OpenCVDetector.h AbstractTracker.h AbstractCameraControl.h
OpenCVDetector.o: utils.h AbstractDetector.h /usr/include/stdio.h
OpenCVDetector.o: /usr/include/features.h /usr/include/libio.h
OpenCVDetector.o: /usr/include/_G_config.h /usr/include/wchar.h
OpenCVDetector.o: /usr/include/xlocale.h
testPrecision.o: testPrecision.h AbstractCameraControl.h utils.h
utils.o: utils.h /usr/include/unistd.h /usr/include/features.h
utils.o: /usr/include/getopt.h
