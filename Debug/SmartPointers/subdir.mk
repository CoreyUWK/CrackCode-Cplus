################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../SmartPointers/SmartPointers.cpp 

OBJS += \
./SmartPointers/SmartPointers.o 

CPP_DEPS += \
./SmartPointers/SmartPointers.d 


# Each subdirectory must supply rules for building sources it contributes
SmartPointers/%.o: ../SmartPointers/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


