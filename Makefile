ifndef VERBOSE
.SILENT:
endif

OS = $(shell uname)

FLAGS = -g
IFLAGS = -Ilibs/glfw/include -Ilibs/glad/include
LDFLAGS = libs/glfw/src/libglfw3.a libs/glad/src/glad.o

SRC = src/main.c
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

#fresh: clean glfw glad all
all: clean glfw glad engine run

clean:
	$(info $(INFO) CLEANING FOR FRESHNESS)
	rm -rf builds
	rm -rf libs/glad/src/glad.o
	rm -rf libs/glfw
	@echo

glad:
	$(info $(INFO) COMPILING GLAD)
	cd libs/glad && gcc -o src/glad.o -Iinclude -c src/glad.c

glfw:
	$(info $(INFO) COMPILING GLFW)
	cd libs && git clone https://github.com/glfw/glfw.git
	cd libs/glfw && cmake . -G "Unix Makefiles" && make

engine:
	$(info $(OS))
	$(info $(INFO) COMPILING MANGROOVE)

	mkdir -p builds

	gcc -o $(OUT) $(SRC) $(IFLAGS) $(LDFLAGS) $(FLAGS)

	@echo

run:
	$(info $(INFO) RUNNING PROGRAM)
	$(info )
	./$(OUT)
	@echo
