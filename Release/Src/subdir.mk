################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/bme280.c \
../Src/ds3231.c \
../Src/i2c.c \
../Src/lcd.c \
../Src/main.c \
../Src/system_stm32f1xx.c \
../Src/tim.c 

OBJS += \
./Src/bme280.o \
./Src/ds3231.o \
./Src/i2c.o \
./Src/lcd.o \
./Src/main.o \
./Src/system_stm32f1xx.o \
./Src/tim.o 

C_DEPS += \
./Src/bme280.d \
./Src/ds3231.d \
./Src/i2c.d \
./Src/lcd.d \
./Src/main.d \
./Src/system_stm32f1xx.d \
./Src/tim.d 


# Each subdirectory must supply rules for building sources it contributes
Src/bme280.o: ../Src/bme280.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu99 -DSTM32F103xB -c -I../Inc -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/bme280.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Src/ds3231.o: ../Src/ds3231.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu99 -DSTM32F103xB -c -I../Inc -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ds3231.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Src/i2c.o: ../Src/i2c.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu99 -DSTM32F103xB -c -I../Inc -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/i2c.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Src/lcd.o: ../Src/lcd.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu99 -DSTM32F103xB -c -I../Inc -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/lcd.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Src/main.o: ../Src/main.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu99 -DSTM32F103xB -c -I../Inc -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/main.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Src/system_stm32f1xx.o: ../Src/system_stm32f1xx.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu99 -DSTM32F103xB -c -I../Inc -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/system_stm32f1xx.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Src/tim.o: ../Src/tim.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu99 -DSTM32F103xB -c -I../Inc -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/tim.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

