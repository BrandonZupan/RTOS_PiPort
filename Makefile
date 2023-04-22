SD_PATH ?= /Volumes/POGGER/
ARMGNU ?= aarch64-unknown-linux-gnu

COPS = -Wall -nostdlib -nostartfiles -ffreestanding -Iinclude -mgeneral-regs-only
ASMOPS = -Iinclude

BUILD_DIR = build
SRC_DIR = src

all : kernel8.img

clean :
	rm -rf $(BUILD_DIR) *.img

install : kernel8.img
	cp kernel8.img $(SD_PATH)
	diskutil eject $(SD_PATH)

serial :
	screen /dev/tty.usbserial-0001 115200

readelf : kernel8.img
	$(ARMGNU)-readelf -a $(BUILD_DIR)/kernel8.elf

$(BUILD_DIR)/%_c.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(ARMGNU)-gcc $(COPS) -MMD -c $< -o $@

$(BUILD_DIR)/%_s.o: $(SRC_DIR)/%.S
	$(ARMGNU)-gcc $(ASMOPS) -MMD -c $< -o $@

build/font_psf.o: src/font.psf
	$(ARMGNU)-ld -r -b binary -o $(BUILD_DIR)/font_psf.o $(SRC_DIR)/font.psf

build/font_sfn.o: src/font.sfn
	$(ARMGNU)-ld -r -b binary -o $(BUILD_DIR)/font_sfn.o $(SRC_DIR)/font.sfn

C_FILES = $(wildcard $(SRC_DIR)/*.c)
ASM_FILES = $(wildcard $(SRC_DIR)/*.S)
OBJ_FILES = $(C_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%_c.o)
OBJ_FILES += $(ASM_FILES:$(SRC_DIR)/%.S=$(BUILD_DIR)/%_s.o)
# OBJ_FILES += $(ASM_FILES:$(SRC_DIR)/%.S=$(BUILD_DIR)/%_s.o)

BIN_FILES = build/font_psf.o build/font_sfn.o

DEP_FILES = $(OBJ_FILES:%.o=%.d)
-include $(DEP_FILES)

kernel8.img: $(SRC_DIR)/linker.ld $(OBJ_FILES) $(BIN_FILES)
	$(ARMGNU)-ld -T $(SRC_DIR)/linker.ld -o $(BUILD_DIR)/kernel8.elf  $(OBJ_FILES) $(BIN_FILES)
	$(ARMGNU)-objcopy $(BUILD_DIR)/kernel8.elf -O binary kernel8.img

# kernel8.img: $(SRC_DIR)/linker.ld $(OBJ_FILES) $(BIN_FILES)
# 	$(ARMGNU)-ld -T $(SRC_DIR)/linker.ld -o $(BUILD_DIR)/kernel8.elf $(BIN_FILES)
# 	$(ARMGNU)-objcopy $(BUILD_DIR)/kernel8.elf -O binary kernel8.img