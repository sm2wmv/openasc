# Linus Widströmer, OH0GRY
# License: GNU GENERAL PUBLIC LICENSE

OBJS=main.o socket.o bus.o config.o logger.o mqueue.o bus_sync.o
TARGET=wmvtunnel

CFLAGS=-Wall -pedantic --std=c99 -D_GNU_SOURCE 
LDFLAGS=-lpthread

CC=gcc

all:	$(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(LDFLAGS) $(OBJS)

%.o:	%.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f *.o *~ $(TARGET)

indent:
	indent -npro -kr -i8 -ts8 -sob -l80 -ss -ncs -cp1 *.c *.h

include *.d
%.d: %.c
	$(SHELL) -ec '$(CC) -M \
	$(CPPFLAGS) $< | \
	sed '\''s/$*.o/& $@/g'\'' \
	> $@'
