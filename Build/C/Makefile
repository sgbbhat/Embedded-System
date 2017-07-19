include sources.mk

# Overrides
CPU = cortex-m0plus
ARCH = thumb
SPECS = nosys.specs

# Compile Defines
CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
BASENAME = demo
TARGET = $(BASENAME).out
LDFLAGS = -Wl, -Map=$(BASENAME).map
CFLAGS = -mcpu=$(CPU) -m$(ARCH) --specs=$(SPECS) -Wall

OBJS = $(SRCS:.c=.o)

%.o : %.c 
	$(CC) - c  $< $(CFLAGS) -o $@

.PHONY: build
build : all
.PHONY : all
all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(LDFLAGS) -o $@

.PHONY: clean
clean: 
	rm -f $(OBJS) $(TARGET) $(BASENAME).map

