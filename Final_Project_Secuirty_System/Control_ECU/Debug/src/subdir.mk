################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Control_ECU.c \
../src/buzzer.c \
../src/control_functions.c \
../src/dc_motor.c \
../src/eeprom.c \
../src/gpio.c \
../src/i2c.c \
../src/pwm.c \
../src/timer1.c \
../src/uart.c 

OBJS += \
./src/Control_ECU.o \
./src/buzzer.o \
./src/control_functions.o \
./src/dc_motor.o \
./src/eeprom.o \
./src/gpio.o \
./src/i2c.o \
./src/pwm.o \
./src/timer1.o \
./src/uart.o 

C_DEPS += \
./src/Control_ECU.d \
./src/buzzer.d \
./src/control_functions.d \
./src/dc_motor.d \
./src/eeprom.d \
./src/gpio.d \
./src/i2c.d \
./src/pwm.d \
./src/timer1.d \
./src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


