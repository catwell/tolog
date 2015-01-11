CC=gcc
CFLAGS=

.PHONY: clean

tolog: tolog.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f tolog
