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
SRC+=$(shell ls src/null_*.c)
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
    SRC_COCOA+=$(shell ls src/*.m)
    SRC+=src/cocoa_time.c
    SRC+=src/posix_module.c
    SRC+=src/posix_thread.c
else
ifeq ($(shell uname),Windows)
    LIB_SHARED=libglfw.$(LIB_VER).dll
    CFLAGS+=-D_GLFW_WIN32
    SRC+=$(shell ls src/win32_*.c)
    SRC+=src/wgl_context.c

else
    LIB_SHARED=libglfw.$(LIB_VER).so
    CFLAGS+=-D_GLFW_X11
    CFLAGS+=-D_GLFW_WAYLAND
    SRC+=$(shell ls src/posix_*.c)
    SRC+=$(shell ls src/x11_*.c)
    SRC+=$(shell ls src/wl_*.c)
    SRC+=src/glx_context.c
    SRC+=src/xkb_unicode.c
    SRC+=src/linux_joystick.c

endif
endif

INC=$(shell ls src/*.h)
INC_API=$(shell ls include/GLFW/*.h)
SRC_EXAMPLES=$(shell ls examples/*.c)

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
