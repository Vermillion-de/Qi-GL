# EXE = main
# IMGUI_DIR = ./imgui
# SOURCES = main.cpp
# SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
# SOURCES += $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl2.cpp
# OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))
# UNAME_S := $(shell uname -s)
# CXXFLAGS = -std=c++11 -I$(IMGUI_DIR)
# CXXFLAGS += -g -Wall -Wformat

# ifeq ($(UNAME_S), Linux) #LINUX
# 	ECHO_MESSAGE = "Linux"
# 	LIBS += -lGL `pkg-config --static --libs glfw3`

# 	CXXFLAGS += `pkg-config --cflags glfw3`
# 	CFLAGS = $(CXXFLAGS)
# endif


# # %.o:%.cpp
# # $(CXX) $(CXXFLAGS) -c -o $@ $<

# %.o:$(IMGUI_DIR)/%.cpp
# 	$(CXX) $(CXXFLAGS) -c -o $@ $<

# %.o:$(IMGUI_DIR)/backends/%.cpp
# 	$(CXX) $(CXXFLAGS) -c -o $@ $<

# all: $(EXE)
# 	@echo Build complete for $(ECHO_MESSAGE)

# $(EXE): $(OBJS)
# 	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

# clean:
# 	rm -f $(EXE) $(OBJS)


all:
	g++ main.cpp -o main -lglut -lGL -lglfw3 -lGLU
