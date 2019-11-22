project(lamp_simulator C ASM)
cmake_minimum_required(VERSION 3.8)
set(PREFIX arm-none-eabi-)
set(CMAKE_C_COMPILER "${PREFIX}gcc")
set(CMAKE_CXX_COMPILER "${PREFIX}g++")
set(CMAKE_ASM_COMPILER "${PREFIX}gcc")
set(CP "${PREFIX}objcopy")
set(SZ "${PREFIX}size")
set(HEX "${CP} -O ihex")
set(BIN "${CP} -O binary -S")
set(CMAKE_CROSSCOMPILING 1)
set(CMAKE_SYSTEM_PROCESSOR cortex-m0)
set(MCU "-mcpu=${CMAKE_SYSTEM_PROCESSOR} -mthumb ${FPU} ${FLOAT-ABI}")
file(GLOB C_SOURCES
    "Src/main.c"
    "Src/stm32f0xx_it.c" 
    "Src/stm32f0_discovery.c" 
    "Src/system_stm32f0xx.c" 
    "Drivers/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_adc.c" 
    "Drivers/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_cec.c" 
    "Drivers/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_comp.c" 
    "Drivers/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_crc.c" 
    "Drivers/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_dbgmcu.c" 
    "Drivers/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_dma.c" 
    "Drivers/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_exti.c" 
    "Drivers/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_flash.c" 
    "Drivers/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_gpio.c" 
    "Drivers/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_i2c.c" 
    "Drivers/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_iwdg.c" 
    "Drivers/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_misc.c" 
    "Drivers/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_pwr.c" 
    "Drivers/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_rcc.c" 
    "Drivers/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_rtc.c" 
    "Drivers/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_spi.c" 
    "Drivers/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_syscfg.c" 
    "Drivers/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_tim.c" 
    "Drivers/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_usart.c" 
    "Drivers/STM32F0xx_StdPeriph_Driver/src/stm32f0xx_wwdg.c"
)
file(GLOB ASM_SOURCES "startup_stm32f051x8.s")
set_source_files_properties(startup_stm32f051x8.s PROPERTIES COMPILE_FLAGS "-x assembler-with-cpp")
add_definitions(
    -DUSE_FULL_LL_DRIVER
    -DHSE_VALUE=8000000
    -DHSE_STARTUP_TIMEOUT=100
    -DLSE_STARTUP_TIMEOUT=5000
    -DLSE_VALUE=32768
    -DHSI_VALUE=8000000
    -DLSI_VALUE=40000
    -DVDD_VALUE=3300
    -DPREFETCH_ENABLE=1
    -DINSTRUCTION_CACHE_ENABLE=0
    -DUSE_STDPERIPH_DRIVER
    -DDATA_CACHE_ENABLE=0
    -DSTM32F051x8
)

include_directories(
    Inc
    Drivers/STM32F0xx_HAL_Driver/Inc
    Drivers/CMSIS/Device/ST/STM32F0xx/Include
    Drivers/CMSIS/Include
    Drivers/CMSIS/Include
    Drivers/STM32F0xx_StdPeriph_Driver/inc
)

set(CMAKE_C_FLAGS "${MCU} -Og -Wall -fdata-sections -ffunction-sections -g -gdwarf-2 -MMD -MP -MF\"$(@:%.o=%.d)\" -Wa,-a,-ad,-alms=\"$(notdir $(<:.c=.lst))\"")
set(LDSCRIPT ../STM32F051R8Tx_FLASH.ld)
set(LIBS "-lc -lm -lnosys")
set(CMAKE_EXE_LINKER_FLAGS "${MCU} -specs=nano.specs -T${LDSCRIPT} ${LIBS} -Wl,-Map=${PROJECT_NAME}.map,--cref -Wl,--gc-sections")
add_executable(${PROJECT_NAME}.elf ${C_SOURCES} ${ASM_SOURCES})
