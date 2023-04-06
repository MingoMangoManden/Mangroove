ifndef VERBOSE
.SILENT:
endif

OS = $(shell uname)

FLAGS = -g
IFLAGS = -Ilibs/glfw/include -Ilibs/glad/include
LDFLAGS = libs/glfw/src/libglfw3.a libs/glad/src/glad.o libs/cglm/libcglm.a

SRC = src/*.c
OUT = builds/Mangroove

INFO = [ INFO ]:

# Mac
ifeq ($(OS), Darwin)
	LDFLAGS += -framework OpenGL -framework IOKit -framework CoreVideo -framework Cocoa
endif

# Windows
ifneq (,$(findstring MINGW64_NT, $(OS)))
	LDFLAGS += -lopengl32 -lgdi32
endif

all: clean libs engine run

clean:
	@echo Cleaning project for freshness...
	rm -rf builds
	rm -rf libs/glad/src/glad.o
	rm -rf libs/glfw
	rm -rf libs/cglm
	@echo

libs: glad glfw cglm

glad:
	@echo Compiling glad...
	cd libs/glad && gcc -o src/glad.o -Iinclude -c src/glad.c

glfw:
	@echo Compiling glfw...
	cd libs && git clone https://github.com/glfw/glfw.git
	cd libs/glfw && cmake . -G "Unix Makefiles" && make

cglm:
	@echo Compiling cglm...
	cd libs && git clone https://github.com/recp/cglm.git
	cd libs/cglm && cmake . -G "Unix Makefiles" -DCGLM_STATIC=ON && make

engine:
	@echo Compiling Mangroove...
	mkdir -p builds
	gcc -o $(OUT) $(SRC) $(IFLAGS) $(LDFLAGS) $(FLAGS)
	@echo

run:
	@echo Running Mangroove...
	@echo
	./$(OUT)
	@echo
