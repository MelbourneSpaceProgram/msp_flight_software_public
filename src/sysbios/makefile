
XOPTS = -I"C:/ti/xdctools_3_50_01_12_core/packages/" -Dxdc_target_types__=C:/ti/simplelink_msp432_sdk_1_30_00_40/kernel/tirtos/packages/ti/targets/arm/elf/std.h -Dxdc_target_name__=M4F

vpath % C:/ti/simplelink_msp432_sdk_1_30_00_40/kernel/tirtos/packages/ti/sysbios/
vpath %.c C:/ti/xdctools_3_50_01_12_core/packages/

CCOPTS    = --endian=little -mv7M4 --abi=eabi --float_support=fpv4spd16 -q -ms --opt_for_speed=2  --program_level_compile -o3 -g --optimize_with_debug  -Dti_sysbios_knl_Task_minimizeLatency__D=FALSE  -Dti_sysbios_posix_Settings_debug__D=FALSE -Dti_sysbios_posix_Settings_supportsMutexPriority__D=FALSE -Dti_sysbios_knl_Clock_stopCheckNext__D=FALSE -Dti_sysbios_family_arm_m3_Hwi_enableException__D=TRUE -Dti_sysbios_family_arm_m3_Hwi_disablePriority__D=32U -Dti_sysbios_family_arm_m3_Hwi_numSparseInterrupts__D=0U

XDC_ROOT  = C:/ti/xdctools_3_50_01_12_core/packages/

BIOS_ROOT = C:/ti/simplelink_msp432_sdk_1_30_00_40/kernel/tirtos/packages/ti/sysbios/

BIOS_DEFS =  -Dti_sysbios_BIOS_swiEnabled__D=TRUE -Dti_sysbios_BIOS_taskEnabled__D=TRUE -Dti_sysbios_BIOS_clockEnabled__D=TRUE -Dti_sysbios_BIOS_runtimeCreatesEnabled__D=TRUE -Dti_sysbios_knl_Task_moduleStateCheckFlag__D=FALSE -Dti_sysbios_knl_Task_objectCheckFlag__D=FALSE -Dti_sysbios_hal_Hwi_DISABLE_ALL_HOOKS -Dti_sysbios_knl_Swi_DISABLE_ALL_HOOKS -Dti_sysbios_BIOS_smpEnabled__D=FALSE -Dti_sysbios_Build_useHwiMacros -Dti_sysbios_Build_useIndirectReferences=FALSE -Dti_sysbios_knl_Swi_numPriorities__D=16 -Dti_sysbios_knl_Task_deleteTerminatedTasks__D=FALSE -Dti_sysbios_knl_Task_numPriorities__D=16 -Dti_sysbios_knl_Task_checkStackFlag__D=TRUE -Dti_sysbios_knl_Task_initStackFlag__D=TRUE -Dti_sysbios_knl_Task_DISABLE_ALL_HOOKS -Dti_sysbios_knl_Clock_TICK_SOURCE=ti_sysbios_knl_Clock_TickSource_TIMER -Dti_sysbios_knl_Clock_TICK_MODE=ti_sysbios_knl_Clock_TickMode_PERIODIC -Dti_sysbios_hal_Core_delegate_getId=ti_sysbios_hal_CoreNull_getId__E -Dti_sysbios_hal_Core_delegate_interruptCore=ti_sysbios_hal_CoreNull_interruptCore__E -Dti_sysbios_hal_Core_delegate_lock=ti_sysbios_hal_CoreNull_lock__E -Dti_sysbios_hal_Core_delegate_unlock=ti_sysbios_hal_CoreNull_unlock__E -Dti_sysbios_hal_Core_numCores__D=1 -Dti_sysbios_hal_CoreNull_numCores__D=1 -Dti_sysbios_utils_Load_taskEnabled__D=TRUE -Dti_sysbios_utils_Load_swiEnabled__D=FALSE -Dti_sysbios_utils_Load_hwiEnabled__D=FALSE -Dti_sysbios_family_arm_m3_Hwi_dispatcherSwiSupport__D=TRUE -Dti_sysbios_family_arm_m3_Hwi_dispatcherTaskSupport__D=TRUE -Dti_sysbios_family_arm_m3_Hwi_dispatcherAutoNestingSupport__D=TRUE -Dti_sysbios_family_arm_m3_Hwi_dispatcherIrpTrackingSupport__D=TRUE -Dti_sysbios_knl_Semaphore_supportsEvents__D=FALSE -Dti_sysbios_knl_Semaphore_supportsPriority__D=FALSE -Dxdc_runtime_Assert_DISABLE_ALL -Dxdc_runtime_Log_DISABLE_ALL --define=__MSP432P401R__ 

BIOS_INC  = -I"C:/ti/simplelink_msp432_sdk_1_30_00_40/kernel/tirtos/packages/"

INCS =  $(BIOS_INC) -I"C:/ti/simplelink_msp432_sdk_1_30_00_40/source/" -I"C:/ti/simplelink_msp432_sdk_1_30_00_40/kernel/tirtos/packages/" -I"C:/ti/ccsv7/ccs_base/" -I"C:/ti/xdctools_3_50_01_12_core/packages/" -I"../" -I"/packages//" 

CC = C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.2.LTS/bin/armcl -c $(CCOPTS) -I C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.2.LTS/include
ASM = C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.2.LTS/bin/armcl -c $(CCOPTS) -I C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.2.LTS/include
AR = C:/ti/ccsv7/tools/compiler/ti-cgt-arm_16.9.2.LTS/bin/armar rq

DEL = C:/ti/xdctools_3_50_01_12_core/packages/../bin/rm -f
CP = C:/ti/xdctools_3_50_01_12_core/packages/../bin/cp -f

define RM
	$(if $(wildcard $1),$(DEL) $1,:)
endef

define ASSEMBLE
	@echo asmem4f $< ...
	@$(ASM) $(BIOS_DEFS) $(XOPTS) $(INCS) $<
endef

all: sysbios.aem4f

m3_Hwi_asm.obj: family/arm/m3/Hwi_asm.sv7M makefile
	@-$(call RM, $@)
	$(ASSEMBLE) --output_file=m3_Hwi_asm.obj

m3_Hwi_asm_switch.obj: family/arm/m3/Hwi_asm_switch.sv7M makefile
	@-$(call RM, $@)
	$(ASSEMBLE) --output_file=m3_Hwi_asm_switch.obj

m3_IntrinsicsSupport_asm.obj: family/arm/m3/IntrinsicsSupport_asm.sv7M makefile
	@-$(call RM, $@)
	$(ASSEMBLE) --output_file=m3_IntrinsicsSupport_asm.obj

m3_TaskSupport_asm.obj: family/arm/m3/TaskSupport_asm.sv7M makefile
	@-$(call RM, $@)
	$(ASSEMBLE) --output_file=m3_TaskSupport_asm.obj


BIOS.obj: BIOS.c knl/Clock.c knl/Idle.c knl/Intrinsics.c knl/Event.c knl/Mailbox.c knl/Queue.c knl/Semaphore.c knl/Swi.c knl/Swi_andn.c knl/Task.c hal/Hwi.c hal/Hwi_stack.c hal/Hwi_startup.c hal/Seconds.c family/arm/msp432/init/Boot.c family/arm/m3/Hwi.c family/arm/m3/TaskSupport.c heaps/HeapMem.c posix/pthread.c posix/pthread_barrier.c posix/pthread_cond.c posix/pthread_key.c posix/pthread_mutex.c posix/pthread_rwlock.c posix/clock.c posix/mqueue.c posix/sched.c posix/semaphore.c posix/sleep.c posix/timer.c gates/GateHwi.c gates/GateMutex.c family/arm/msp432/Timer.c family/arm/msp432/ClockFreqs.c family/arm/msp432/Seconds.c makefile
	@-$(call RM, $@)
	@echo clem4f $< ...
	@$(CC) $(BIOS_DEFS) $(XOPTS) $(INCS) \
	$(BIOS_ROOT)BIOS.c \
	$(BIOS_ROOT)knl/Clock.c \
	$(BIOS_ROOT)knl/Idle.c \
	$(BIOS_ROOT)knl/Intrinsics.c \
	$(BIOS_ROOT)knl/Event.c \
	$(BIOS_ROOT)knl/Mailbox.c \
	$(BIOS_ROOT)knl/Queue.c \
	$(BIOS_ROOT)knl/Semaphore.c \
	$(BIOS_ROOT)knl/Swi.c \
	$(BIOS_ROOT)knl/Swi_andn.c \
	$(BIOS_ROOT)knl/Task.c \
	$(BIOS_ROOT)hal/Hwi.c \
	$(BIOS_ROOT)hal/Hwi_stack.c \
	$(BIOS_ROOT)hal/Hwi_startup.c \
	$(BIOS_ROOT)hal/Seconds.c \
	$(BIOS_ROOT)family/arm/msp432/init/Boot.c \
	$(BIOS_ROOT)family/arm/m3/Hwi.c \
	$(BIOS_ROOT)family/arm/m3/TaskSupport.c \
	$(BIOS_ROOT)heaps/HeapMem.c \
	$(BIOS_ROOT)posix/pthread.c \
	$(BIOS_ROOT)posix/pthread_barrier.c \
	$(BIOS_ROOT)posix/pthread_cond.c \
	$(BIOS_ROOT)posix/pthread_key.c \
	$(BIOS_ROOT)posix/pthread_mutex.c \
	$(BIOS_ROOT)posix/pthread_rwlock.c \
	$(BIOS_ROOT)posix/clock.c \
	$(BIOS_ROOT)posix/mqueue.c \
	$(BIOS_ROOT)posix/sched.c \
	$(BIOS_ROOT)posix/semaphore.c \
	$(BIOS_ROOT)posix/sleep.c \
	$(BIOS_ROOT)posix/timer.c \
	$(BIOS_ROOT)gates/GateHwi.c \
	$(BIOS_ROOT)gates/GateMutex.c \
	$(BIOS_ROOT)family/arm/msp432/Timer.c \
	$(BIOS_ROOT)family/arm/msp432/ClockFreqs.c \
	$(BIOS_ROOT)family/arm/msp432/Seconds.c \

sysbios.aem4f: BIOS.obj m3_Hwi_asm.obj m3_Hwi_asm_switch.obj m3_IntrinsicsSupport_asm.obj m3_TaskSupport_asm.obj
	@-$(call RM, $@)
	@echo arem4f $^ ...
	@$(AR) $@ $^


clean:
	@$(DEL) ..\makefile.libs
	@-$(call RM, *)
