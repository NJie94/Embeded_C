################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/Joystick.c" \
"../Sources/clock.c" \
"../Sources/lcd_fonts.c" \
"../Sources/lcd_spi.c" \
"../Sources/main.c" \
"../Sources/nokia_LCD.c" \
"../Sources/timer.c" \
"../Sources/uart.c" \

C_SRCS += \
../Sources/Joystick.c \
../Sources/clock.c \
../Sources/lcd_fonts.c \
../Sources/lcd_spi.c \
../Sources/main.c \
../Sources/nokia_LCD.c \
../Sources/timer.c \
../Sources/uart.c \

OBJS += \
./Sources/Joystick.o \
./Sources/clock.o \
./Sources/lcd_fonts.o \
./Sources/lcd_spi.o \
./Sources/main.o \
./Sources/nokia_LCD.o \
./Sources/timer.o \
./Sources/uart.o \

C_DEPS += \
./Sources/Joystick.d \
./Sources/clock.d \
./Sources/lcd_fonts.d \
./Sources/lcd_spi.d \
./Sources/main.d \
./Sources/nokia_LCD.d \
./Sources/timer.d \
./Sources/uart.d \

OBJS_QUOTED += \
"./Sources/Joystick.o" \
"./Sources/clock.o" \
"./Sources/lcd_fonts.o" \
"./Sources/lcd_spi.o" \
"./Sources/main.o" \
"./Sources/nokia_LCD.o" \
"./Sources/timer.o" \
"./Sources/uart.o" \

C_DEPS_QUOTED += \
"./Sources/Joystick.d" \
"./Sources/clock.d" \
"./Sources/lcd_fonts.d" \
"./Sources/lcd_spi.d" \
"./Sources/main.d" \
"./Sources/nokia_LCD.d" \
"./Sources/timer.d" \
"./Sources/uart.d" \

OBJS_OS_FORMAT += \
./Sources/Joystick.o \
./Sources/clock.o \
./Sources/lcd_fonts.o \
./Sources/lcd_spi.o \
./Sources/main.o \
./Sources/nokia_LCD.o \
./Sources/timer.o \
./Sources/uart.o \


# Each subdirectory must supply rules for building sources it contributes
Sources/Joystick.o: ../Sources/Joystick.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/Joystick.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/Joystick.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/clock.o: ../Sources/clock.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/clock.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/clock.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/lcd_fonts.o: ../Sources/lcd_fonts.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/lcd_fonts.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/lcd_fonts.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/lcd_spi.o: ../Sources/lcd_spi.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/lcd_spi.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/lcd_spi.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/main.o: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/main.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/main.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/nokia_LCD.o: ../Sources/nokia_LCD.c
	@echo 'Building file: $<'
	@echo 'Executing target #6 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/nokia_LCD.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/nokia_LCD.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/timer.o: ../Sources/timer.c
	@echo 'Building file: $<'
	@echo 'Executing target #7 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/timer.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/timer.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/uart.o: ../Sources/uart.c
	@echo 'Building file: $<'
	@echo 'Executing target #8 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/uart.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/uart.o"
	@echo 'Finished building: $<'
	@echo ' '


