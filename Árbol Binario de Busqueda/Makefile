CFLAGS = -g -std=c99 -Wall
CFLAGS += -Wconversion -Wno-sign-conversion -Werror -Wbad-function-cast -Wshadow
CFLAGS += -Wpointer-arith -Wunreachable-code -Wformat=2


# Se puede comentar durante el desarrollo.
CFLAGS += -Werror

test: abb_aux
	./abb_test.py

abb_aux: abb.o abb_aux.o

# Dependencias adicionales.
-include deps.mk

clean:
	rm -f abb_aux *.o

.PHONY: test clean

