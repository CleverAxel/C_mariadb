#https://stackoverflow.com/questions/1950926/create-directories-using-make-file
#https://gl.developpez.com/tutoriel/outil/makefile/
BUILDDIR = build
LIB = -lmariadb
INCLUDE = -I ./include

# Find all .c files in src and subdirectories
SOURCES = main.c $(shell find src -type f -name "*.c")
OBJECTS = $(patsubst src/%.c, $(BUILDDIR)/%.o, $(SOURCES))

# Target to build the main executable
$(BUILDDIR)/main: $(OBJECTS)
	@mkdir -p $(@D)
	@gcc -o $@ $^ $(LIB) $(INCLUDE)

# Generic rule to build object files
$(BUILDDIR)/%.o: src/%.c
	@mkdir -p $(@D)
	@gcc -c $< -o $@ $(LIB) $(INCLUDE)

.PHONY: clean

clean:
	rm -rf $(BUILDDIR)

