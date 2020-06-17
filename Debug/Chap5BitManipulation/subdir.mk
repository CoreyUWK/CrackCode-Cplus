################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Chap5BitManipulation/Chap5BitManipulation.cpp 

OBJS += \
./Chap5BitManipulation/Chap5BitManipulation.o 

CPP_DEPS += \
./Chap5BitManipulation/Chap5BitManipulation.d 


# Each subdirectory must supply rules for building sources it contributes
Chap5BitManipulation/%.o: ../Chap5BitManipulation/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


