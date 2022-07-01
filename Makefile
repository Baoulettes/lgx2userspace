CXX ?= g++
CC ?= gcc
LD := lld
AR	:= ar
FORMAT := clang-format-11

UNAME := $(shell uname)
UNAMEM := $(shell uname -m)

ASAN ?= 0
DEBUG ?= 1
OPTFLAGS ?= -O0
LTO ?= 0

WARN := \
	-Wno-return-type \
	-funsigned-char \
	-fno-stack-protector -fno-common -fno-zero-initialized-in-bss -fno-strict-aliasing -fno-inline-functions -fno-inline-small-functions -fno-toplevel-reorder -ffreestanding -fwrapv \

CXXFLAGS := $(WARN) -std=c++20 -D_GNU_SOURCE -fpermissive -no-pie -nostdlib
CFLAGS := $(WARN) -std=c99 -D_GNU_SOURCE -no-pie -nostdlib
LDFLAGS :=

ifeq ($(UNAME), Linux) #LINUX
	CXXFLAGS += -mhard-float -msse2 -mfpmath=sse
	CFLAGS += -mhard-float -msse2 -mfpmath=sse
endif

CPPFLAGS := -MMD


ifeq ($(UNAME), Linux) #LINUX
TARGET := lgxuserspace.elf
endif

INC_DIRS := $(addprefix -I, \
	. \
	src \
	utils \
)

ifeq ($(UNAME), Linux) #LINUX
	INC_DIRS += $(addprefix -I, \
		/opt/X11/include \
)
endif


LDLIBS := \
	$(addprefix -l, \
	usb-1.0 \
	pcap \
)

ifeq ($(UNAME), Linux) #LINUX
LDLIBS += \
	$(addprefix -l, \
	X11 \
	SDL2 \
	GL \
	GLEW \
	pulse-simple \
)
endif

CXX_FILES := \
	$(shell find src -type f -name "*.cpp")

C_FILES := \
	$(shell find utils -type f -name "*.c")

O_FILES := \
	$(C_FILES:%.c=build/%.o) \
	$(CXX_FILES:%.cpp=build/%.o)
D_FILES := $(O_FILES:%.o=%.d)

# create build directory
SRC_DIRS := $(shell find . -type d -a -not -path "*build*")
$(shell mkdir -p $(SRC_DIRS:%=build/%))

all:
	$(MAKE) $(TARGET)

clean:
	rm -rf build $(TARGET)

.PHONY: all clean

build/%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $(OPTFLAGS) $(INC_DIRS) $< -o $@

build/%.o: %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(OPTFLAGS) $(INC_DIRS) $< -o $@


$(TARGET): $(O_FILES)
	$(CXX) $^ -o $@ $(LDFLAGS) $(LDDIRS) $(LDLIBS)

-include $(D_FILES)