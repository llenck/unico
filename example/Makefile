CFLAGS += -Wall -Wextra -std=c99
INCLUDEDIRS := -I ../include
LIBS := ../unico.a

all: message.out

.PHONY: clean
clean:
	$(RM) message.out

%.out: %.c
	$(CC) $(CFLAGS) $(INCLUDEDIRS) $< $(LIBS) -o $@

