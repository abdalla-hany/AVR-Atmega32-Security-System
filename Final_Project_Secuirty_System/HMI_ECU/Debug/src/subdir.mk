################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/HMI_ECU.c \
../src/gpio.c \
../src/hmi_functions.c \
../src/keypad.c \
../src/lcd.c \
../src/timer1.c \
../src/uart.c 

OBJS += \
./src/HMI_ECU.o \
./src/gpio.o \
./src/hmi_functions.o \
./src/keypad.o \
./src/lcd.o \
./src/timer1.o \
./src/uart.o 

C_DEPS += \
./src/HMI_ECU.d \
./src/gpio.d \
./src/hmi_functions.d \
./src/keypad.d \
./src/lcd.d \
./src/timer1.d \
./src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


