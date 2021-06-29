# Directories
INCDIR := ./include
SRCDIR := ./src
OBJDIR := ./src/obj
LIBDIR := ./lib
BUILDDIR := ./bin
DEBUGDIR := ./debug

# Compilation info
CC := gcc
CFLAGS := -Wall -lcurses -pthread -g -I$(INCDIR)

# Name of the executable
TARGET := centipede

# Find all the .h files in the include directory
HFILES := $(shell find $(INCDIR) -type f -name '*.h')

# Find all the .c files in the src directory
# Extracts the file names and puts them in _CFILES
CFILES := $(shell find $(SRCDIR)\
		 -maxdepth 1 -type f -name '*.c' -exec basename {} ';')

# Create a .o file for each .c file
# Replaces the .c suffix with a .o
# Then format the .o files as ./obj/file.o
_OBJFILES := $(CFILES:.c=.o)
OBJFILES := $(patsubst %, $(OBJDIR)/%, $(_OBJFILES))

#Build the executable
$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(BUILDDIR)/$@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HFILES)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: test clean run debug valgrind helgrind

test:
	@echo $(CFILES)
	@echo $(HFILES)
	@echo $(OBJFILES)

clean:
	rm -f $(OBJDIR)/*.o $(BUILDDIR)/$(TARGET)

cleand:
	rm -f $(OBJDIR)/*.o $(BUILDDIR)/$(TARGET) $(DEBUGDIR)/*.txt

run: $(TARGET)
	@$(BUILDDIR)/$(TARGET)

debug: $(TARGET)
	@gdb $(BUILDDIR)/$(TARGET)

valgrind: $(TARGET)
	@valgrind --leak-check=full \
			--show-leak-kinds=all \
			--track-origins=yes \
			--verbose \
			--log-file=$(DEBUGDIR)/valgrind-out.txt \
			$(BUILDDIR)/$(TARGET)

helgrind: $(TARGET)
	@valgrind --tool=helgrind \
			--verbose \
			--log-file=$(DEBUGDIR)/helgrind-out.txt \
			$(BUILDDIR)/$(TARGET)
	
	