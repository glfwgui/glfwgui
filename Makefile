include Makefile.config

CC ?= gcc
BUILD ?= ./build

$(shell mkdir -p $(BUILD)/src)
$(shell mkdir -p $(BUILD)/examples)

SRC=src/context.c
SRC+=src/egl_context.c
SRC+=src/init.c
SRC+=src/input.c
SRC+=src/monitor.c
SRC+=src/null_init.c
SRC+=src/null_joystick.c
SRC+=src/null_monitor.c
SRC+=src/null_window.c
SRC+=src/osmesa_context.c
SRC+=src/platform.c
SRC+=src/vulkan.c
SRC+=src/window.c

SRC_COCOA=

LIB_VER=3.5
LIB_STATIC=libglfw3.a

ifeq ($(shell uname),Darwin)
    LIB_SHARED=libglfw.$(LIB_VER).dylib
    CFLAGS+=-D_GLFW_COCOA
    LDFLAGS+=-framework OpenGL -framework Cocoa -framework IOKit -framework CoreFoundation -framework QuartzCore
    SRC+=src/cocoa_time.c
    SRC_COCOA+=src/cocoa_init.m
    SRC_COCOA+=src/cocoa_joystick.m
    SRC_COCOA+=src/cocoa_monitor.m
    SRC_COCOA+=src/cocoa_window.m
    SRC_COCOA+=src/nsgl_context.m
    SRC_COCOA+=src/cocoa_gui.m
else
ifeq ($(shell uname),Windows)
    LIB_SHARED=libglfw.$(LIB_VER).dll
    CFLAGS+=-D_GLFW_WIN32
    SRC+=src/wgl_context.c
    SRC+=src/win32_init.c
    SRC+=src/win32_joystick.c
    SRC+=src/win32_module.c
    SRC+=src/win32_monitor.c
    SRC+=src/win32_thread.c
    SRC+=src/win32_time.c
    SRC+=src/win32_window.c
else
    LIB_SHARED=libglfw.$(LIB_VER).so
    CFLAGS+=-D_GLFW_X11
    CFLAGS+=-D_GLFW_WAYLAND
    SRC+=src/posix_time.c
    SRC+=src/glx_context.c
    SRC+=src/x11_init.c
    SRC+=src/x11_monitor.c
    SRC+=src/x11_window.c
    SRC+=src/xkb_unicode.c
    SRC+=src/wl_init.c
    SRC+=src/wl_monitor.c
    SRC+=src/wl_window.c
    SRC+=src/linux_joystick.c
    SRC+=src/posix_poll.c
endif
endif

ifneq ($(shell uname),Windows)
    SRC+=src/posix_module.c
    SRC+=src/posix_thread.c
endif

INC=src/cocoa_joystick.h
INC+=src/cocoa_platform.h
INC+=src/cocoa_time.h
INC+=src/internal.h
INC+=src/linux_joystick.h
INC+=src/mappings.h
INC+=src/null_joystick.h
INC+=src/null_platform.h
INC+=src/platform.h
INC+=src/posix_poll.h
INC+=src/posix_thread.h
INC+=src/posix_time.h
INC+=src/win32_joystick.h
INC+=src/win32_platform.h
INC+=src/win32_thread.h
INC+=src/win32_time.h
INC+=src/wl_platform.h
INC+=src/x11_platform.h
INC+=src/xkb_unicode.h
INC_API+=include/GLFW/glfw3.h
INC_API+=include/GLFW/glfw3gui.h
INC_API+=include/GLFW/glfw3native.h

SRC_EXAMPLES=examples/boing.c
SRC_EXAMPLES+=examples/gears.c
SRC_EXAMPLES+=examples/heightmap.c
SRC_EXAMPLES+=examples/offscreen.c
SRC_EXAMPLES+=examples/particles.c
SRC_EXAMPLES+=examples/sharing.c
SRC_EXAMPLES+=examples/splitview.c
SRC_EXAMPLES+=examples/triangle-opengl.c
SRC_EXAMPLES+=examples/triangle-opengles.c
SRC_EXAMPLES+=examples/wave.c
SRC_EXAMPLES+=examples/windows.c
SRC_EXAMPLES+=examples/gui.c

OBJ := $(SRC:.c=.o)
OBJ_COCOA := $(SRC_COCOA:.m=.o_cocoa)
OBJS = $(OBJ) $(OBJ_COCOA)
EXAMPLES := $(SRC_EXAMPLES:.c=)
EXAMPLES_STATIC := $(SRC_EXAMPLES:.c=_static)

BUILD_OBJ := $(addprefix $(BUILD)/, $(OBJ))
BUILD_OBJ_COCOA := $(addprefix $(BUILD)/, $(OBJ_COCOA))
BUILD_OBJS = $(BUILD_OBJ) $(BUILD_OBJ_COCOA)
BUILD_EXAMPLES := $(addprefix $(BUILD)/, $(EXAMPLES))
BUILD_EXAMPLES_STATIC := $(addprefix $(BUILD)/, $(EXAMPLES_STATIC))

$(BUILD)/src/%.o_cocoa: src/%.m $(INC)
	$(CC) -Iinclude/ $(CFLAGS) -c -x objective-c $< -o $@

$(BUILD)/src/%.o: src/%.c $(INC)
	$(CC) -Iinclude/ $(CFLAGS) -c $< -o $@

.PHONE: all
all: $(BUILD)/$(LIB_SHARED) $(BUILD)/$(LIB_STATIC)

$(BUILD)/examples/particles: examples/particles.c $(INC_API)
	$(CC) -Iinclude/ -Ideps/ $(CFLAGS) $(LDFLAGS) ./deps/tinycthread.c ./deps/getopt.c examples/particles.c -L$(BUILD)/ -lglfw.$(LIB_VER) -o $@

$(BUILD)/examples/%: examples/%.c $(INC_API)
	$(CC) -Iinclude/ -Ideps/ $(CFLAGS) $(LDFLAGS) $< -L$(BUILD)/ -lglfw.$(LIB_VER) -o $@

$(BUILD)/examples/particles_static: examples/particles.c $(INC_API)
	$(CC) -Iinclude/ -Ideps/ $(CFLAGS) $(LDFLAGS) ./deps/tinycthread.c ./deps/getopt.c examples/particles.c $(BUILD)/$(LIB_STATIC) -o $@

$(BUILD)/examples/%_static: examples/%.c $(INC_API)
	$(CC) -Iinclude/ -Ideps/ $(CFLAGS) $(LDFLAGS) $< $(BUILD)/$(LIB_STATIC) -o $@

$(BUILD)/$(LIB_STATIC): $(BUILD_OBJS) $(SRC) $(INC) $(INC_API)
	ar rcs $@ $(BUILD_OBJS)

$(BUILD)/$(LIB_SHARED): $(BUILD_OBJS) $(SRC) $(INC) $(INC_API)
	$(CC) -shared $(BUILD_OBJS) $(LDFLAGS) -o $@

.PHONE: examples
examples: $(BUILD_EXAMPLES)

.PHONE: examples_static
examples_static: $(BUILD_EXAMPLES_STATIC)

.PHONE: clean
clean:
	rm $(BUILD)/$(LIB_STATIC) $(BUILD)/$(LIB_SHARED) $(BUILD_OBJS) $(BUILD_EXAMPLES) $(BUILD_EXAMPLES_STATIC)
