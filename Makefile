OBJ_DIR=obj_dir
TRACE_DIR=trace

ifeq ($(OS),Windows_NT)
	DYNAMIC_LIB_PREFIX=
	DYNAMIC_LIB_EXT=.dll
else
	DYNAMIC_LIB_PREFIX=lib

	UNAME=$(shell uname)
	ifeq ($(UNAME),Darwin)
		DYNAMIC_LIB_EXT=.dylib
	else
		DYNAMIC_LIB_EXT=.so
	endif
endif

SDRAM_SM=fizzim/sdram.fzm

SDRAM_PREFIX=sdram
SDRAM_VM_PREFIX=V$(SDRAM_PREFIX)
SDRAM_DRIVER=$(OBJ_DIR)/$(SDRAM_VM_PREFIX)
SDRAM_DRIVER_RTL=rtl/sdram.sv
SDRAM_DRIVER_SRC=sim/sdram_driver.cpp

SDRAM_TEST_DIR=sim/sdram-test
SDRAM_TEST_SRC=$(wildcard $(SDRAM_TEST_DIR)/**/*.rs)
SDRAM_TEST=$(SDRAM_TEST_DIR)/target/release/$(DYNAMIC_LIB_PREFIX)sdram_test$(DYNAMIC_LIB_EXT)
SDRAM_TRACE=$(TRACE_DIR)/sdram_test.vcd

VERILATOR=verilator
VERILATOR_FLAGS=-Wall -Wno-fatal -O3 --x-assign fast --noassert -CFLAGS "-O3 -std=c++11" --trace --sv

FIZZIM=perl
FIZZIM_FLAGS=-language SystemVerilog

RM=rm
RM_FLAGS=-rf

.PHONY: all dirs fizzim clean

all: dirs $(SDRAM_DRIVER) $(SDRAM_TEST)

dirs: $(OBJ_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

fizzim: $(SDRAM_SM)
	$(PERL) fizzim/fizzim.pl < $(SDRAM_SM) > $(SDRAM_DRIVER_RTL) $(FIZZIM_FLAGS)

$(SDRAM_DRIVER): fizzim $(SDRAM_DRIVER_RTL) $(SDRAM_DRIVER_SRC)
	$(VERILATOR) $(VERILATOR_FLAGS) -cc $(SDRAM_DRIVER_RTL) --exe $(SDRAM_DRIVER_SRC)
	$(MAKE) -j -C $(OBJ_DIR) -f $(SDRAM_VM_PREFIX).mk

$(SDRAM_TEST): $(SDRAM_TEST_SRC)
	cd $(SDRAM_TEST_DIR) && cargo build --release

test: dirs $(SDRAM_DRIVER) $(SDRAM_TEST)
	$(SDRAM_DRIVER) $(SDRAM_TEST) $(SDRAM_TRACE)

clean:
	$(RM) $(RM_FLAGS) $(SDRAM_DRIVER_RTL)
	$(RM) $(RM_FLAGS) $(OBJ_DIR)
	$(RM) $(RM_FLAGS) $(TRACE_DIR)/*.vcd
	cd $(SDRAM_TEST_DIR) && cargo clean