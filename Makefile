# Executable name
EXENAME = prosimmain
# Includes directory
IDIR =./include
# Objects directory
ODIR=./obj
# Library directory
LDIR =./lib
# Source Directory
SDIR = ./src


# Compiler command
CC=cc
# Compiler flags
CFLAGS=-I$(IDIR) -std=c99 -pedantic

# Library linking
LIBS=-lm

_DEPS = 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = queue.o reader.o cmdline.o main.o handler.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXENAME): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
