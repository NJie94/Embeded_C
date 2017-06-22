################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/ADC.c" \
"../Sources/Joystick.c" \
"../Sources/PIT.c" \
"../Sources/T6_Main.c" \
"../Sources/clock.c" \
"../Sources/display.c" \
"../Sources/lcd_fonts.c" \
"../Sources/lcd_spi.c" \
"../Sources/menu.c" \
"../Sources/nokia_LCD.c" \
"../Sources/timer.c" \
"../Sources/uart.c" \

C_SRCS += \
../Sources/ADC.c \
../Sources/Joystick.c \
../Sources/PIT.c \
../Sources/T6_Main.c \
../Sources/clock.c \
../Sources/display.c \
../Sources/lcd_fonts.c \
../Sources/lcd_spi.c \
../Sources/menu.c \
../Sources/nokia_LCD.c \
../Sources/timer.c \
../Sources/uart.c \

OBJS += \
./Sources/ADC.o \
./Sources/Joystick.o \
./Sources/PIT.o \
./Sources/T6_Main.o \
./Sources/clock.o \
./Sources/display.o \
./Sources/lcd_fonts.o \
./Sources/lcd_spi.o \
./Sources/menu.o \
./Sources/nokia_LCD.o \
./Sources/timer.o \
./Sources/uart.o \

C_DEPS += \
./Sources/ADC.d \
./Sources/Joystick.d \
./Sources/PIT.d \
./Sources/T6_Main.d \
./Sources/clock.d \
./Sources/display.d \
./Sources/lcd_fonts.d \
./Sources/lcd_spi.d \
./Sources/menu.d \
./Sources/nokia_LCD.d \
./Sources/timer.d \
./Sources/uart.d \

OBJS_QUOTED += \
"./Sources/ADC.o" \
"./Sources/Joystick.o" \
"./Sources/PIT.o" \
"./Sources/T6_Main.o" \
"./Sources/clock.o" \
"./Sources/display.o" \
"./Sources/lcd_fonts.o" \
"./Sources/lcd_spi.o" \
"./Sources/menu.o" \
"./Sources/nokia_LCD.o" \
"./Sources/timer.o" \
"./Sources/uart.o" \

C_DEPS_QUOTED += \
"./Sources/ADC.d" \
"./Sources/Joystick.d" \
"./Sources/PIT.d" \
"./Sources/T6_Main.d" \
"./Sources/clock.d" \
"./Sources/display.d" \
"./Sources/lcd_fonts.d" \
"./Sources/lcd_spi.d" \
"./Sources/menu.d" \
"./Sources/nokia_LCD.d" \
"./Sources/timer.d" \
"./Sources/uart.d" \

OBJS_OS_FORMAT += \
./Sources/ADC.o \
./Sources/Joystick.o \
./Sources/PIT.o \
./Sources/T6_Main.o \
./Sources/clock.o \
./Sources/display.o \
./Sources/lcd_fonts.o \
./Sources/lcd_spi.o \
./Sources/menu.o \
./Sources/nokia_LCD.o \
./Sources/timer.o \
./Sources/uart.o \


# Each subdirectory must supply rules for building sources it contributes
Sources/ADC.o: ../Sources/ADC.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/ADC.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/ADC.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/Joystick.o: ../Sources/Joystick.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/Joystick.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/Joystick.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/PIT.o: ../Sources/PIT.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/PIT.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/PIT.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/T6_Main.o: ../Sources/T6_Main.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/T6_Main.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/T6_Main.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/clock.o: ../Sources/clock.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/clock.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/clock.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/display.o: ../Sources/display.c
	@echo 'Building file: $<'
	@echo 'Executing target #6 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/display.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/display.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/lcd_fonts.o: ../Sources/lcd_fonts.c
	@echo 'Building file: $<'
	@echo 'Executing target #7 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/lcd_fonts.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/lcd_fonts.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/lcd_spi.o: ../Sources/lcd_spi.c
	@echo 'Building file: $<'
	@echo 'Executing target #8 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/lcd_spi.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/lcd_spi.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/menu.o: ../Sources/menu.c
	@echo 'Building file: $<'
	@echo 'Executing target #9 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/menu.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/menu.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/nokia_LCD.o: ../Sources/nokia_LCD.c
	@echo 'Building file: $<'
	@echo 'Executing target #10 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/nokia_LCD.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/nokia_LCD.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/timer.o: ../Sources/timer.c
	@echo 'Building file: $<'
	@echo 'Executing target #11 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/timer.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/timer.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/uart.o: ../Sources/uart.c
	@echo 'Building file: $<'
	@echo 'Executing target #12 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/uart.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/uart.o"
	@echo 'Finished building: $<'
	@echo ' '


