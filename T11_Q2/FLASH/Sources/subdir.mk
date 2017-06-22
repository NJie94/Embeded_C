################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/FTM_IC.c" \
"../Sources/clock.c" \
"../Sources/main.c" \
"../Sources/uart.c" \

C_SRCS += \
../Sources/FTM_IC.c \
../Sources/clock.c \
../Sources/main.c \
../Sources/uart.c \

OBJS += \
./Sources/FTM_IC.o \
./Sources/clock.o \
./Sources/main.o \
./Sources/uart.o \

C_DEPS += \
./Sources/FTM_IC.d \
./Sources/clock.d \
./Sources/main.d \
./Sources/uart.d \

OBJS_QUOTED += \
"./Sources/FTM_IC.o" \
"./Sources/clock.o" \
"./Sources/main.o" \
"./Sources/uart.o" \

C_DEPS_QUOTED += \
"./Sources/FTM_IC.d" \
"./Sources/clock.d" \
"./Sources/main.d" \
"./Sources/uart.d" \

OBJS_OS_FORMAT += \
./Sources/FTM_IC.o \
./Sources/clock.o \
./Sources/main.o \
./Sources/uart.o \


# Each subdirectory must supply rules for building sources it contributes
Sources/FTM_IC.o: ../Sources/FTM_IC.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/FTM_IC.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/FTM_IC.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/clock.o: ../Sources/clock.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/clock.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/clock.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/main.o: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/main.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/main.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/uart.o: ../Sources/uart.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/uart.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/uart.o"
	@echo 'Finished building: $<'
	@echo ' '


