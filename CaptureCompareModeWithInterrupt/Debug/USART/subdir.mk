################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../USART/USART.c 

OBJS += \
./USART/USART.o 

C_DEPS += \
./USART/USART.d 


# Each subdirectory must supply rules for building sources it contributes
USART/%.o USART/%.su USART/%.cyclo: ../USART/%.c USART/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32F429I_DISC1 -DSTM32 -DSTM32F429ZITx -DSTM32F4 -c -I../Inc -I"C:/Users/mahon/stmBareMetal/BareMetalSTM/Drivers/Device/ST/STM32F4xx/Include" -I"C:/Users/mahon/stmBareMetal/BareMetalSTM/Drivers/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=soft -mthumb -o "$@"

clean: clean-USART

clean-USART:
	-$(RM) ./USART/USART.cyclo ./USART/USART.d ./USART/USART.o ./USART/USART.su

.PHONY: clean-USART

