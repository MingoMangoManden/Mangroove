#ifndef VERBOSE
#.SILENT:
#endif

fresh: clean all

all: engine run

FLAGS = -lopengl32 -lgdi32
IFLAGS = -Ilibs/glfw/include -Ilibs/glad/include
LDFLAGS = libs/glfw/libglfw3.a libs/glad/src/glad.o

SRC = src/window.cpp
OUT = builds/Mangroove

INFO = [ INFO ]:


engine:
	$(info $(INFO) COMPILING MANGROOVE & DEPENDENCIES)
	cd libs/glad && gcc -o src/glad.o -Iinclude -c src/glad.c
	g++ -o $(OUT) $(SRC) $(IFLAGS) $(LDFLAGS) $(FLAGS)
	@echo


clean:
	$(info $(INFO) CLEANING FOR FRESH COMPILE)
	rm -rf builds
	mkdir -p builds
	rm -rf libs/glad/src/glad.o
	@echo

run:
	$(info $(INFO) RUNNING PROGRAM)
	./$(OUT)
	@echo
