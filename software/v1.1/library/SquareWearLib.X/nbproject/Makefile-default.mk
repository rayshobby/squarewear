#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=lib
DEBUGGABLE_SUFFIX=
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/SquareWearLib.X.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=lib
DEBUGGABLE_SUFFIX=
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/SquareWearLib.X.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/USBSerial/ccode/usb_descriptors.o ${OBJECTDIR}/USBSerial/ccode/usb_device.o ${OBJECTDIR}/USBSerial/ccode/usb_function_cdc.o ${OBJECTDIR}/USBSerial/ccode/usb_serial_cdc.o ${OBJECTDIR}/SquareWearLib.o
POSSIBLE_DEPFILES=${OBJECTDIR}/USBSerial/ccode/usb_descriptors.o.d ${OBJECTDIR}/USBSerial/ccode/usb_device.o.d ${OBJECTDIR}/USBSerial/ccode/usb_function_cdc.o.d ${OBJECTDIR}/USBSerial/ccode/usb_serial_cdc.o.d ${OBJECTDIR}/SquareWearLib.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/USBSerial/ccode/usb_descriptors.o ${OBJECTDIR}/USBSerial/ccode/usb_device.o ${OBJECTDIR}/USBSerial/ccode/usb_function_cdc.o ${OBJECTDIR}/USBSerial/ccode/usb_serial_cdc.o ${OBJECTDIR}/SquareWearLib.o


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/SquareWearLib.X.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F14K50
MP_PROCESSOR_OPTION_LD=18f14k50
MP_LINKER_DEBUG_OPTION= -u_DEBUGCODESTART=0x3e70 -u_DEBUGCODELEN=0x190
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/USBSerial/ccode/usb_descriptors.o: USBSerial/ccode/usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/USBSerial/ccode 
	@${RM} ${OBJECTDIR}/USBSerial/ccode/usb_descriptors.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -p$(MP_PROCESSOR_OPTION) -I"./USBSerial/include" -ms -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/USBSerial/ccode/usb_descriptors.o   USBSerial/ccode/usb_descriptors.c 
	@${DEP_GEN} -d ${OBJECTDIR}/USBSerial/ccode/usb_descriptors.o 
	@${FIXDEPS} "${OBJECTDIR}/USBSerial/ccode/usb_descriptors.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/USBSerial/ccode/usb_device.o: USBSerial/ccode/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/USBSerial/ccode 
	@${RM} ${OBJECTDIR}/USBSerial/ccode/usb_device.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -p$(MP_PROCESSOR_OPTION) -I"./USBSerial/include" -ms -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/USBSerial/ccode/usb_device.o   USBSerial/ccode/usb_device.c 
	@${DEP_GEN} -d ${OBJECTDIR}/USBSerial/ccode/usb_device.o 
	@${FIXDEPS} "${OBJECTDIR}/USBSerial/ccode/usb_device.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/USBSerial/ccode/usb_function_cdc.o: USBSerial/ccode/usb_function_cdc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/USBSerial/ccode 
	@${RM} ${OBJECTDIR}/USBSerial/ccode/usb_function_cdc.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -p$(MP_PROCESSOR_OPTION) -I"./USBSerial/include" -ms -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/USBSerial/ccode/usb_function_cdc.o   USBSerial/ccode/usb_function_cdc.c 
	@${DEP_GEN} -d ${OBJECTDIR}/USBSerial/ccode/usb_function_cdc.o 
	@${FIXDEPS} "${OBJECTDIR}/USBSerial/ccode/usb_function_cdc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/USBSerial/ccode/usb_serial_cdc.o: USBSerial/ccode/usb_serial_cdc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/USBSerial/ccode 
	@${RM} ${OBJECTDIR}/USBSerial/ccode/usb_serial_cdc.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -p$(MP_PROCESSOR_OPTION) -I"./USBSerial/include" -ms -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/USBSerial/ccode/usb_serial_cdc.o   USBSerial/ccode/usb_serial_cdc.c 
	@${DEP_GEN} -d ${OBJECTDIR}/USBSerial/ccode/usb_serial_cdc.o 
	@${FIXDEPS} "${OBJECTDIR}/USBSerial/ccode/usb_serial_cdc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/SquareWearLib.o: SquareWearLib.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/SquareWearLib.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PICKIT2=1 -p$(MP_PROCESSOR_OPTION) -I"./USBSerial/include" -ms -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/SquareWearLib.o   SquareWearLib.c 
	@${DEP_GEN} -d ${OBJECTDIR}/SquareWearLib.o 
	@${FIXDEPS} "${OBJECTDIR}/SquareWearLib.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
else
${OBJECTDIR}/USBSerial/ccode/usb_descriptors.o: USBSerial/ccode/usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/USBSerial/ccode 
	@${RM} ${OBJECTDIR}/USBSerial/ccode/usb_descriptors.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"./USBSerial/include" -ms -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/USBSerial/ccode/usb_descriptors.o   USBSerial/ccode/usb_descriptors.c 
	@${DEP_GEN} -d ${OBJECTDIR}/USBSerial/ccode/usb_descriptors.o 
	@${FIXDEPS} "${OBJECTDIR}/USBSerial/ccode/usb_descriptors.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/USBSerial/ccode/usb_device.o: USBSerial/ccode/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/USBSerial/ccode 
	@${RM} ${OBJECTDIR}/USBSerial/ccode/usb_device.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"./USBSerial/include" -ms -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/USBSerial/ccode/usb_device.o   USBSerial/ccode/usb_device.c 
	@${DEP_GEN} -d ${OBJECTDIR}/USBSerial/ccode/usb_device.o 
	@${FIXDEPS} "${OBJECTDIR}/USBSerial/ccode/usb_device.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/USBSerial/ccode/usb_function_cdc.o: USBSerial/ccode/usb_function_cdc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/USBSerial/ccode 
	@${RM} ${OBJECTDIR}/USBSerial/ccode/usb_function_cdc.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"./USBSerial/include" -ms -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/USBSerial/ccode/usb_function_cdc.o   USBSerial/ccode/usb_function_cdc.c 
	@${DEP_GEN} -d ${OBJECTDIR}/USBSerial/ccode/usb_function_cdc.o 
	@${FIXDEPS} "${OBJECTDIR}/USBSerial/ccode/usb_function_cdc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/USBSerial/ccode/usb_serial_cdc.o: USBSerial/ccode/usb_serial_cdc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/USBSerial/ccode 
	@${RM} ${OBJECTDIR}/USBSerial/ccode/usb_serial_cdc.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"./USBSerial/include" -ms -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/USBSerial/ccode/usb_serial_cdc.o   USBSerial/ccode/usb_serial_cdc.c 
	@${DEP_GEN} -d ${OBJECTDIR}/USBSerial/ccode/usb_serial_cdc.o 
	@${FIXDEPS} "${OBJECTDIR}/USBSerial/ccode/usb_serial_cdc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/SquareWearLib.o: SquareWearLib.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/SquareWearLib.o.d 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"./USBSerial/include" -ms -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/SquareWearLib.o   SquareWearLib.c 
	@${DEP_GEN} -d ${OBJECTDIR}/SquareWearLib.o 
	@${FIXDEPS} "${OBJECTDIR}/SquareWearLib.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: archive
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/SquareWearLib.X.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_AR} $(MP_EXTRA_AR_PRE) -c dist/${CND_CONF}/${IMAGE_TYPE}/SquareWearLib.X.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}  
else
dist/${CND_CONF}/${IMAGE_TYPE}/SquareWearLib.X.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_AR} $(MP_EXTRA_AR_PRE) -c dist/${CND_CONF}/${IMAGE_TYPE}/SquareWearLib.X.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}  
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
