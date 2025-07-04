#
# Cross Platform Makefile
# Compatible with MSYS2/MINGW, Ubuntu 14.04.1 and Mac OS X
#
# You will need GLFW (http://www.glfw.org):
# Linux:
#   apt-get install libglfw-dev
# Mac OS X:
#   brew install glfw
# MSYS2:
#   pacman -S --noconfirm --needed mingw-w64-x86_64-toolchain mingw-w64-x86_64-glfw
#

CXX = g++
#CXX = clang++

EXE = soles_racing
IMGUI_DIR = ./imgui
IMPLOT_DIR = ./implot
SOURCES = main.cpp
SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp
SOURCES += $(IMPLOT_DIR)/implot.cpp $(IMPLOT_DIR)/implot_items.cpp

OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))
UNAME_S := $(shell uname -s)
LINUX_GL_LIBS = -lGL

CXXFLAGS = -std=c++11 -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends -I$(IMPLOT_DIR)
CXXFLAGS += -g -Wall -Wformat -pthread -lGLEW
LIBS = # removed -lglw32 for Mac, still in windows

##---------------------------------------------------------------------
## OPENGL ES
##---------------------------------------------------------------------

## This assumes a GL ES library available in the system, e.g. libGLESv2.so
# CXXFLAGS += -DIMGUI_IMPL_OPENGL_ES2
# LINUX_GL_LIBS = -lGLESv2

##---------------------------------------------------------------------
## BUILD FLAGS PER PLATFORM
##---------------------------------------------------------------------

ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += $(LINUX_GL_LIBS) `pkg-config --static --libs glfw3`

	CXXFLAGS += `pkg-config --cflags glfw3`
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(UNAME_S), Darwin) #APPLE
	ECHO_MESSAGE = "Mac OS X"
	LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
	LIBS += -L/usr/local/lib -L/opt/local/lib -L/opt/homebrew/lib
	LIBS += -lglew -lglfw

	CXXFLAGS += -I/usr/local/include -I/opt/local/include -I/opt/homebrew/include
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(OS), Windows_NT)
	ECHO_MESSAGE = "MinGW"
	LIBS += -lglew32 -lglfw3 -lgdi32 -lopengl32 -limm32 

	CXXFLAGS += `pkg-config --cflags glfw3`
	CFLAGS = $(CXXFLAGS)
endif

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(IMGUI_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(IMGUI_DIR)/backends/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(IMPLOT_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(EXE)
	@echo Build complete for $(ECHO_MESSAGE)

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(EXE) $(OBJS)


# # Compiler and Executable
# CXX = g++
# EXE = soles_racing

# # IMGUI Directory
# IMGUI_DIR = ./imgui

# # Sources and Objects
# SOURCES = main.cpp \
#           $(IMGUI_DIR)/imgui.cpp \
#           $(IMGUI_DIR)/imgui_demo.cpp \
#           $(IMGUI_DIR)/imgui_draw.cpp \
#           $(IMGUI_DIR)/imgui_tables.cpp \
#           $(IMGUI_DIR)/imgui_widgets.cpp \
#           $(IMGUI_DIR)/imgui_impl_glfw.cpp \
#           $(IMGUI_DIR)/imgui_impl_opengl3.cpp

# OBJS = $(SOURCES:.cpp=.o)

# # Platform Detection
# UNAME_S := $(shell uname -s)

# # Compiler Flags and Libraries
# CXXFLAGS = -std=c++11 -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends -Wall
# LDFLAGS = 
# LIBS = 

# ifeq ($(UNAME_S), Linux)
#     CXXFLAGS += `pkg-config --cflags glfw3`
#     LDFLAGS = -lGL -lm `pkg-config --static --libs glfw3`
# endif

# ifeq ($(UNAME_S), Darwin)
#     LDFLAGS = -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
#     LDFLAGS += -L/usr/local/lib -L/opt/local/lib -L/opt/homebrew/lib
#     CXXFLAGS += -I/usr/local/include -I/opt/local/include -I/opt/homebrew/include
# endif

# ifeq ($(OS), Windows_NT)
#     CXXFLAGS += -I/mingw64/include
#     LDFLAGS += -L/mingw64/lib
#     LIBS += -lglfw3 -lopengl32 -lgdi32 -limm32
# endif

# # Build Rules
# %.o: %.cpp
# 	$(CXX) $(CXXFLAGS) -c -o $@ $<

# all: $(EXE)
# 	@echo Build complete for $(UNAME_S)

# $(EXE): $(OBJS)
# 	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

# clean:
# 	rm -f $(EXE) $(OBJS)
