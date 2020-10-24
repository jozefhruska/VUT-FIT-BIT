CC = gcc
CFLAGS = -std=gnu99 -Wall -Wextra -Werror -pedantic

HEADERS = proj2_generator.h proj2_data.h
OBJECTS = proj2.o proj2_generator.o proj2_data.o

default: proj2

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

proj2: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ -lpthread

clean:
	-rm -f $(OBJECTS)
	-rm -f proj2
	-rm -f proj2.out