################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Chap7ObjectOrientedDesign/DeckOfCards/Card.cpp \
../Chap7ObjectOrientedDesign/DeckOfCards/Hand.cpp 

OBJS += \
./Chap7ObjectOrientedDesign/DeckOfCards/Card.o \
./Chap7ObjectOrientedDesign/DeckOfCards/Hand.o 

CPP_DEPS += \
./Chap7ObjectOrientedDesign/DeckOfCards/Card.d \
./Chap7ObjectOrientedDesign/DeckOfCards/Hand.d 


# Each subdirectory must supply rules for building sources it contributes
Chap7ObjectOrientedDesign/DeckOfCards/%.o: ../Chap7ObjectOrientedDesign/DeckOfCards/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


