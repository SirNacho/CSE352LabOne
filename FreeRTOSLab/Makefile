# nRF application makefile
PROJECT_NAME = $(shell basename "$(realpath ./)")

# Configurations
NRF_IC = nrf52832
SDK_VERSION = 15
SOFTDEVICE_MODEL = s132

# Source and header files
APP_HEADER_PATHS += include
APP_SOURCE_PATHS += src
APP_SOURCES += main.c 
APP_SOURCES += tasks.c 
APP_SOURCES += list.c 
APP_SOURCES += timers.c 
APP_SOURCES += queue.c 
APP_SOURCES += port.c, 
APP_SOURCES += port_cmsis.c 
APP_SOURCES += portcmsis_systick.c
APP_SOURCES += head_2.c
#$(notdir $(wildcard ./*.c))

# Path to base of nRF52-base repo
NRF_BASE_DIR = ../CSE352LabOne/software/nrf52x-base/

# Include board Makefile (if any)
include ../CSE352LabOne/software/boards/buckler_revC/Board.mk

# Include main Makefile
include $(NRF_BASE_DIR)make/AppMakefile.mk
