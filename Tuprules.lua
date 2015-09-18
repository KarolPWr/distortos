--
-- file: Tuprules.lua
--
-- author: Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
--
-- This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
-- distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
--
-- date: 2015-09-18
--

------------------------------------------------------------------------------------------------------------------------
-- tup build configuration
------------------------------------------------------------------------------------------------------------------------

TOP = tup.getcwd()

------------------------------------------------------------------------------------------------------------------------
-- toolchain configuration
------------------------------------------------------------------------------------------------------------------------

TOOLCHAIN = "arm-none-eabi-"

AS = TOOLCHAIN .. "gcc"
CC = TOOLCHAIN .. "gcc"
CXX = TOOLCHAIN .. "g++"
LD = TOOLCHAIN .. "g++"
OBJCOPY = TOOLCHAIN .. "objcopy"
OBJDUMP = TOOLCHAIN .. "objdump"
SIZE = TOOLCHAIN .. "size"

------------------------------------------------------------------------------------------------------------------------
-- project configuration
------------------------------------------------------------------------------------------------------------------------

-- file with $(CONFIG_SELECTED_CONFIGURATION) variable
tup.include("selectedConfiguration.mk")

-- path to distortosConfiguration.mk file selected by $(CONFIG_SELECTED_CONFIGURATION) variable
DISTORTOS_CONFIGURATION_MK = TOP .. "/" .. CONFIG_SELECTED_CONFIGURATION .. "/distortosConfiguration.mk"

-- output folder
OUTPUT = TOP .. "/output/"

-- project name
PROJECT = "distortos"

-- core type
COREFLAGS = "-mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16"

-- global C++ flags
CXXFLAGS = ""

-- global C flags
CFLAGS = ""

-- global assembler flags
ASFLAGS = ""

-- optimization flags ("-O0" - no optimization, "-O1" - optimize, "-O2" - optimize even more, "-Os" - optimize for size
-- or "-O3" - optimize yet more)
OPTIMIZATION = "-O2"

-- define warning options here
CXXWARNINGS = "-Wall -Wextra -Wshadow"
CWARNINGS = "-Wall -Wstrict-prototypes -Wextra -Wshadow"

-- C++ language standard ("c++98", "gnu++98" - default, "c++11", "gnu++11")
CXXSTD = "-std=gnu++11"

-- C language standard ("c89" / "iso9899:1990", "iso9899:199409", "c99" / "iso9899:1999", "gnu89" - default, "gnu99")
CSTD = "-std=gnu99"

-- debug flags
DBGFLAGS = "-g -ggdb3"

-- linker flags
LDFLAGS = ""

------------------------------------------------------------------------------------------------------------------------
-- load configuration variables from distortosConfiguration.mk file selected by user
------------------------------------------------------------------------------------------------------------------------

local file = assert(io.open("./" .. CONFIG_SELECTED_CONFIGURATION .. "/distortosConfiguration.mk", "r"))
for line in file:lines() do
	local configPattern = "CONFIG_[A-Za-z0-9_]+"
	local standardConfigPattern = "^(" .. configPattern .. ")=(.*)$"
	local name, value = line:match(standardConfigPattern)
	if name ~= nil and value ~= nil then
		local unquotedValue = value:match("^\"(.*)\"$")
		if unquotedValue ~= nil then
			value = unquotedValue
		end	
	else
		local notSetConfigPattern = "^# (" .. configPattern .. ") is not set$"
		name = line:match(notSetConfigPattern)
		if name ~= nil then
			value = "n"
		end
	end

	if name ~= nil and value ~= nil then
		_G[name] = value
	end
end
file:close()

------------------------------------------------------------------------------------------------------------------------
-- compilation flags
------------------------------------------------------------------------------------------------------------------------

ASFLAGS += COREFLAGS
ASFLAGS += DBGFLAGS

CFLAGS += COREFLAGS
CFLAGS += OPTIMIZATION
CFLAGS += CWARNINGS
CFLAGS += CSTD
CFLAGS += DBGFLAGS
CFLAGS += "-ffunction-sections -fdata-sections"

CXXFLAGS += COREFLAGS
CXXFLAGS += OPTIMIZATION
CXXFLAGS += CXXWARNINGS
CXXFLAGS += CXXSTD
CXXFLAGS += DBGFLAGS
CXXFLAGS += "-ffunction-sections -fdata-sections -fno-rtti -fno-exceptions"

-- path to linker script (generated automatically)
LDSCRIPT = OUTPUT .. CONFIG_CHIP .. ".ld"

LDFLAGS += "-T" .. LDSCRIPT
LDFLAGS += COREFLAGS
LDFLAGS += "-g -Wl,-Map=" .. OUTPUT .. PROJECT .. ".map,--cref,--gc-sections"

------------------------------------------------------------------------------------------------------------------------
-- "constants" with include paths
------------------------------------------------------------------------------------------------------------------------

-- "standard" includes
STANDARD_INCLUDES += "-I" .. OUTPUT .. "include -I" .. TOP .. "/include"

-- architecture includes
ARCHITECTURE_INCLUDES += CONFIG_ARCHITECTURE_INCLUDES:gsub("(%g+)", "-I" .. TOP .. "/%1")

------------------------------------------------------------------------------------------------------------------------
-- tup/lua functions
------------------------------------------------------------------------------------------------------------------------

-- get "file specific flags" from table (ASFLAGS, CFLAGS or CXXFLAGS) for file named filename
function getSpecificFlags(table, filename)
	specific_flags = {}
	if table[filename] ~= nil then
		specific_flags = table[filename]
	end
	return specific_flags
end

-- assemble file named input
function as(input)
	specific_flags = getSpecificFlags(ASFLAGS, input)
	inputs = {input, extra_inputs = {"$(TOP)/<headers>"}}
	outputs = {OUTPUT .. tup.getrelativedir(TOP) .. "/%B.o", "$(TOP)/<objects>"}
	tup.rule(inputs, "^c^ $(AS) $(ASFLAGS) $(specific_flags) -c %f -o %o", outputs)
end

-- compile (C) file named input
function cc(input)
	specific_flags = getSpecificFlags(CFLAGS, input)
	inputs = {input, extra_inputs = {"$(TOP)/<headers>"}}
	outputs = {OUTPUT .. tup.getrelativedir(TOP) .. "/%B.o", "$(TOP)/<objects>"}
	tup.rule(inputs, "^c^ $(CC) $(CFLAGS) $(specific_flags) -c %f -o %o", outputs)
end

-- compile (C++) file named input
function cxx(input)
	specific_flags = getSpecificFlags(CXXFLAGS, input)
	inputs = {input, extra_inputs = {"$(TOP)/<headers>"}}
	outputs = {OUTPUT .. tup.getrelativedir(TOP) .. "/%B.o", "$(TOP)/<objects>"}
	tup.rule(inputs, "^c^ $(CXX) $(CXXFLAGS) $(specific_flags) -c %f -o %o", outputs)
end

-- link all objects from $(TOP)/<objects> into file named output
function link(output)
	inputs = {"$(TOP)/<objects>", extra_inputs = {"$(TOP)/<ldscripts>"}}
	extra_output = {OUTPUT .. PROJECT .. ".map"}
	tup.rule(inputs, "$(LD) $(LDFLAGS) %<objects> -o %o", {output, extra_outputs = extra_output})
end

-- convert file named input (elf) to intel hex file named output
function hex(input, output)
	tup.rule(input, "$(OBJCOPY) -O ihex %f %o", output)
end

-- convert file named input (elf) to binary file named output
function bin(input, output)
	tup.rule(input, "$(OBJCOPY) -O binary %f %o", output)
end

-- dump symbols from file named input (elf) to file named output
function dmp(input, output)
	tup.rule(input, "$(OBJDUMP) -x --syms --demangle %f > %o", output)
end

-- generate disassembly of file named input (elf) to file named output
function lss(input, output)
	tup.rule(input, "$(OBJDUMP) --demangle -S %f > %o", output)
end

-- print size of file named input (elf)
function size(input)
	tup.rule({input}, "$(SIZE) -B %f")
end
