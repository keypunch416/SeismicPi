################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Static_Code/System/subdir.mk
-include Sources/SPI/subdir.mk
-include Sources/SD/utility/subdir.mk
-include Sources/SD/subdir.mk
-include Sources/HAL/subdir.mk
-include Sources/Console/subdir.mk
-include Sources/Arduino/subdir.mk
-include Sources/subdir.mk
-include Project_Settings/Startup_Code/subdir.mk
-include Generated_Code/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
SeismicPi_Bootloader.bin \

SECONDARY_SIZE += \
SeismicPi_Bootloader.siz \


# All Target
all: SeismicPi_Bootloader.elf secondary-outputs

# Tool invocations
SeismicPi_Bootloader.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM C++ Linker'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -T "C:/Users/Steve/workspace.kds/SeismicPi_Bootloader/Project_Settings/Linker_Files/ProcessorExpert.ld" -Xlinker --gc-sections -L"C:/Users/Steve/workspace.kds/SeismicPi_Bootloader/Project_Settings/Linker_Files" -Wl,-Map,"SeismicPi_Bootloader.map" -specs=nano.specs -specs=nosys.specs -o "SeismicPi_Bootloader.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

SeismicPi_Bootloader.bin: SeismicPi_Bootloader.elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy -O binary "SeismicPi_Bootloader.elf"  "SeismicPi_Bootloader.bin"
	@echo 'Finished building: $@'
	@echo ' '

SeismicPi_Bootloader.siz: SeismicPi_Bootloader.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=berkeley "SeismicPi_Bootloader.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(SECONDARY_SIZE)$(C++_DEPS)$(OBJS)$(C_DEPS)$(ASM_DEPS)$(CC_DEPS)$(SECONDARY_FLASH)$(CPP_DEPS)$(CXX_DEPS)$(C_UPPER_DEPS)$(S_UPPER_DEPS) SeismicPi_Bootloader.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
