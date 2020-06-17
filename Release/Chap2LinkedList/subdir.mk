################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Chap2LinkedList/Chap2LinkedListMain.cpp 

OBJS += \
./Chap2LinkedList/Chap2LinkedListMain.o 

CPP_DEPS += \
./Chap2LinkedList/Chap2LinkedListMain.d 


# Each subdirectory must supply rules for building sources it contributes
Chap2LinkedList/%.o: ../Chap2LinkedList/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


