cmake_minimum_required(VERSION 3.8)
project(LocusTester C CXX ASM)
set(CMAKE_CXX_STANDARD 20)

# Toolchain
set(PREFIX "arm-none-eabi-")
set(CMAKE_CXX_COMPILER ${PREFIX}g++)
set(CMAKE_C_COMPILER ${PREFIX}gcc)
set(CMAKE_ASM_COMPILER ${PREFIX}g++)
set(AS ${PREFIX}as)
set(AR ${PREFIX}ar)
set(OBJCOPY ${PREFIX}objcopy)
set(OBJDUMP ${PREFIX}objdump)
set(SIZE ${PREFIX}size)

# link script
set(LDSCRIPT STM32F051K8Ux_FLASH.ld)
get_filename_component(LDSCRIPT ${LDSCRIPT} REALPATH)

set(INCLUDE_DIRS
	Core/Inc
	Drivers/STM32F0xx_HAL_Driver/Inc
	Drivers/STM32F0xx_HAL_Driver/Inc/Legacy
	Drivers/CMSIS/Device/ST/STM32F0xx/Include
	Drivers/CMSIS/Include
	libs/st7735s/source
	libs/fonts/
)

# Sources
set(ASM_SOURCES startup_stm32f051x8.s)
file(GLOB_RECURSE
	CXX_SOURCES
	Drivers/*.c
	Core/*.c
	libs/st7735s/source/st7735s.c
	Core/*.cpp
)

# Compile/link flags
set(GCC_COMPILE_FLAGS
	-mcpu=cortex-m0
	-mthumb
	-Og
	-Wall
	-fdata-sections
	-ffunction-sections
	-g
	-gdwarf-2
)

set(AS_COMPILE_FLAGS
	-x assembler-with-cpp
	-mcpu=cortex-m0
	-mthumb
	-Og
	-Wall
	-fdata-sections
	-ffunction-sections
	-g
	-gdwarf-2
)

set(GCC_LINK_OPTIONS
	-mcpu=cortex-m0
	-mthumb
	-specs=nano.specs
	-specs=nosys.specs
	# -Wl,-Map=build/LocusTester.map,--cref
	-Wl,--gc-sections
	-T${LDSCRIPT}
)

set(DEFINITIONS
	-DUSE_HAL_DRIVER
	-DSTM32F051x8
)

# Hagl library
add_subdirectory(libs/hagl)

# Hagl HAL library
add_subdirectory(libs/hagl_hal)

set(LIBRARIES
	c
	m
	nosys
	hagl_hal
	hagl
)

add_executable(${PROJECT_NAME}.elf ${CXX_SOURCES} ${ASM_SOURCES})
target_include_directories(${PROJECT_NAME}.elf PRIVATE ${INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}.elf PRIVATE ${LIBRARIES})
target_compile_options(${PROJECT_NAME}.elf PRIVATE ${GCC_COMPILE_FLAGS})
target_link_options(${PROJECT_NAME}.elf PRIVATE ${GCC_LINK_OPTIONS})
target_compile_definitions(${PROJECT_NAME}.elf PRIVATE ${DEFINITIONS})
set_property(SOURCE ${ASM_SOURCES} APPEND PROPERTY COMPILE_OPTIONS ${AS_COMPILE_FLAGS})

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
	COMMAND ${OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
	COMMAND ${OBJCOPY} -Obinary -S $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
)
