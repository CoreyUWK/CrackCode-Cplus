################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Chap3StacksQueues/Chap3StacksQueues.cpp 

OBJS += \
./Chap3StacksQueues/Chap3StacksQueues.o 

CPP_DEPS += \
./Chap3StacksQueues/Chap3StacksQueues.d 


# Each subdirectory must supply rules for building sources it contributes
Chap3StacksQueues/%.o: ../Chap3StacksQueues/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


