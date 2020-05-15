CC=g++
CPPFLAGS=-O2
LDFLAGS=-lpthread
DEBUG =-Wall -Wextra -Werror -pedantic -march=x86-64 -mtune=generic
PROGNAME=primark

$(PROGNAME): main.o
	$(CC) -o $(PROGNAME) main.o $(CPPFLAGS) $(DEBUG) $(LDFLAGS)

strip:
	strip -S --strip-unneeded --remove-section=.note.gnu.gold-version --remove-section=.comment --remove-section=.note --remove-section=.note.gnu.build-id --remove-section=.note.ABI-tag $(PROGNAME)


.PHONY: clean
clean:
	rm $(PROGNAME) *.o
