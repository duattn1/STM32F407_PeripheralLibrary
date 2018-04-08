# Project: Flash_Progamming_Utility_On_PC
# Makefile created by Dev-C++ 5.11

CPP      = g++.exe -D__DEBUG__
CC       = gcc.exe -D__DEBUG__
WINDRES  = windres.exe
OBJ      = src/hex_parser.o src/main.o src/serial_port_communication.o src/stm32f4discovery_bootloader_commands.o
LINKOBJ  = src/hex_parser.o src/main.o src/serial_port_communication.o src/stm32f4discovery_bootloader_commands.o
LIBS     = -L"C:/Program Files/Dev-Cpp/MinGW64/x86_64-w64-mingw32/lib32" -static-libgcc -m32 -g3
INCS     = -I"C:/Program Files/Dev-Cpp/MinGW64/include" -I"C:/Program Files/Dev-Cpp/MinGW64/x86_64-w64-mingw32/include" -I"C:/Program Files/Dev-Cpp/MinGW64/lib/gcc/x86_64-w64-mingw32/4.9.2/include" -I"E:/C/Dev-C/Bootloader_USART/inc" -I"E:/C/GIT/STM32F4_Discovey_Bootloader/Projects/Flash_Progamming_Utility_On_PC/inc"
CXXINCS  = -I"C:/Program Files/Dev-Cpp/MinGW64/include" -I"C:/Program Files/Dev-Cpp/MinGW64/x86_64-w64-mingw32/include" -I"C:/Program Files/Dev-Cpp/MinGW64/lib/gcc/x86_64-w64-mingw32/4.9.2/include" -I"C:/Program Files/Dev-Cpp/MinGW64/lib/gcc/x86_64-w64-mingw32/4.9.2/include/c++" -I"E:/C/GIT/STM32F4_Discovey_Bootloader/Projects/Flash_Progamming_Utility_On_PC/inc"
BIN      = Flash_Progamming_Utility_On_PC.exe
CXXFLAGS = $(CXXINCS) -ansi -m32 -g3
CFLAGS   = $(INCS) -ansi -m32 -g3
RM       = rm.exe -f

.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after

clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CC) $(LINKOBJ) -o $(BIN) $(LIBS)

src/hex_parser.o: src/hex_parser.c
	$(CC) -c src/hex_parser.c -o src/hex_parser.o $(CFLAGS)

src/main.o: src/main.c
	$(CC) -c src/main.c -o src/main.o $(CFLAGS)

src/serial_port_communication.o: src/serial_port_communication.c
	$(CC) -c src/serial_port_communication.c -o src/serial_port_communication.o $(CFLAGS)

src/stm32f4discovery_bootloader_commands.o: src/stm32f4discovery_bootloader_commands.c
	$(CC) -c src/stm32f4discovery_bootloader_commands.c -o src/stm32f4discovery_bootloader_commands.o $(CFLAGS)
