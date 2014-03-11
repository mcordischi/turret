CC=g++
CFLAGS=`pkg-config --libs --cflags opencv  --cflags libcurl` -ldl -lm -lglib-2.0

%.o: %.cpp
	$(CC) -c $< $(CFLAGS) -o $@

%: %.cpp
	$(CC) $^ $(CFLAGS) -o $@ 
