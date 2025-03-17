PROJECT_NAME = $(shell basename "$(realpath ./)")

# Configurations
NRF_IC = nrf52833
SDK_VERSION = 16
SOFTDEVICE_MODEL = blank

# Source and header files
APP_HEADER_PATHS += .
APP_SOURCE_PATHS += .
APP_SOURCES = $(notdir $(wildcard ./*.c))

# Include platform header paths
APP_HEADER_PATHS += ../include

include ../boards/microbit_v2/Board.mk

# Path to the nRF base directory (adjust as needed based on your structure)
NRF_BASE_DIR ?= ../../nrf52x-base
include $(NRF_BASE_DIR)/make/AppMakefile.mk