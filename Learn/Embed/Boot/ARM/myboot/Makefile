TARGET := myboot.16K
BUILD := myboot.bin

OUTPUT := myboot

COBJS += mystart.o
COBJS += mylowlevel_init.o

SDTOOLS := ./mk210
LDS := myboot.lds

CROSS_COMPLIE := arm-linux-
CC := $(CROSS_COMPLIE)gcc
LD := $(CROSS_COMPLIE)ld
OBJCOPY := $(CROSS_COMPLIE)objcopy

LDFLAGS += -T$(LDS)
#Way
.PHONY:all
##该句告诉make，all为伪目标，在新的make中已经能自动识别，可不加
#$@:目标集合
#$^:所有依赖文件集合
#$<:依赖中的第一个
all:$(TARGET)
$(TARGET):$(BUILD)
   $(SDTOOLS) $@ $^
$(BUILD):$(OUTPUT)
   $(OBJCOPY) -O binary $^ $@
$(OUTPUT):$(COBJS)
   $(LD) $(LDFLAGS) -o $@ $^
%.o:%.S
   $(CC) -c $^ 

mksd:
   sudo dd iflag=dsync oflag=dsync if=myboot.16K of=/dev/sdb seek=1

clean:
   rm -f $(TARGET) $(BUILD) *.o *.16K