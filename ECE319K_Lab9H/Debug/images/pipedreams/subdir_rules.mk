################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
images/pipedreams/%.o: ../images/pipedreams/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1240/ccs/tools/compiler/ti-cgt-armllvm_2.1.3.LTS/bin/tiarmclang.exe" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"C:/Users/Matthew King/Documents/UT-CollegeStuff/ECE-319H-Tiwari-Valvano/GameDesignProject/SourceCode/MSPM0_ValvanoWare/ECE319K_Lab9H" -I"C:/Users/Matthew King/Documents/UT-CollegeStuff/ECE-319H-Tiwari-Valvano/GameDesignProject/SourceCode/MSPM0_ValvanoWare/ECE319K_Lab9H/Debug" -I"C:/ti/mspm0_sdk_1_20_01_06/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_1_20_01_06/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"images/pipedreams/$(basename $(<F)).d_raw" -MT"$(@)" -I"C:/Users/Matthew King/Documents/UT-CollegeStuff/ECE-319H-Tiwari-Valvano/GameDesignProject/SourceCode/MSPM0_ValvanoWare/ECE319K_Lab9H/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


