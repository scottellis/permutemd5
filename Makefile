CC = gcc
CFLAGS = -O2 -Wall
LIBS = -lmhash

TARGET = permutemd5

$(TARGET): permutemd5.c
	$(CC) $(CFLAGS) $< $(LIBS) -o $@

clean:
	rm -f $(TARGET)
