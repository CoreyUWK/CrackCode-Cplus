################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Chap1ArraysStrings/Chap1ArraysStringsMain.cpp 

OBJS += \
./Chap1ArraysStrings/Chap1ArraysStringsMain.o 

CPP_DEPS += \
./Chap1ArraysStrings/Chap1ArraysStringsMain.d 


# Each subdirectory must supply rules for building sources it contributes
Chap1ArraysStrings/%.o: ../Chap1ArraysStrings/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


