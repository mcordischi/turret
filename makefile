CC=g++
CFLAGS=`pkg-config --libs --cflags opencv` -ldl -lm

%: %.cpp
	$(CC) $< $(CFLAGS) -o $@
