CFLAGS=
LIBS=-Iinclude -lfreeglut -lopengl32

OBJFILES = main.o matrix.o vector.o math.o threed.o gfx.o model.o string_tools.o

all: clean main.exe

%.o: %.cpp
	g++ -c $(CFLAGS) $(LIBS) $< -o $@

main.exe: $(OBJFILES)
	g++ $(OBJFILES) $(LIBS) -o $@

clean:
	del *.o *.exe