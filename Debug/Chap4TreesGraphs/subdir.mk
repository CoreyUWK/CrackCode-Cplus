################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Chap4TreesGraphs/Chap4TreesGraphs.cpp 

OBJS += \
./Chap4TreesGraphs/Chap4TreesGraphs.o 

CPP_DEPS += \
./Chap4TreesGraphs/Chap4TreesGraphs.d 


# Each subdirectory must supply rules for building sources it contributes
Chap4TreesGraphs/%.o: ../Chap4TreesGraphs/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


