################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../eigen-eigen-323c052e1731/doc/tutorial.cpp 

OBJS += \
./eigen-eigen-323c052e1731/doc/tutorial.o 

CPP_DEPS += \
./eigen-eigen-323c052e1731/doc/tutorial.d 


# Each subdirectory must supply rules for building sources it contributes
eigen-eigen-323c052e1731/doc/%.o: ../eigen-eigen-323c052e1731/doc/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 Linux g++ compiler'
	arm-linux-gnueabihf-g++ -Wall -O0 -g3 -I"C:\Xilinx\SDK\2018.3\gnu\aarch32\nt\gcc-arm-linux-gnueabi\arm-linux-gnueabihf\include\c++\7.3.1" -I"C:\Users\easrkev\Desktop\FER\DIPLOMSKI_PROJEKT\Reconstruction_code\XSDK_CS\compressive_sensing_zedboard\eigen-eigen-323c052e1731" -c -fmessage-length=0 -MT"$@" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


