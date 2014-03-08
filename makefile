CC=g++
CFLAGS=`pkg-config --libs --cflags opencv  --cflags libcurl` -ldl -lm

%.o: %.cpp
	$(CC) -c $< -o $@

%: %.cpp control.o
	$(CC) $^ $(CFLAGS) -o $@
