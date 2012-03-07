#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=Cygwin-Windows
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/app_ng.o \
	${OBJECTDIR}/app_g.o \
	${OBJECTDIR}/app2.o \
	${OBJECTDIR}/utility.o \
	${OBJECTDIR}/app_ir.o \
	${OBJECTDIR}/app_sc_ng.o \
	${OBJECTDIR}/memory.o \
	${OBJECTDIR}/app_sp.o \
	${OBJECTDIR}/app2_sc_ng.o \
	${OBJECTDIR}/app_irb.o \
	${OBJECTDIR}/generate.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/app2_ng.o \
	${OBJECTDIR}/app2_g.o \
	${OBJECTDIR}/app2_irb.o \
	${OBJECTDIR}/app.o \
	${OBJECTDIR}/random.o \
	${OBJECTDIR}/app2_ir.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/pga_secuencial.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/pga_secuencial.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/pga_secuencial ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/app_ng.o: app_ng.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/app_ng.o app_ng.c

${OBJECTDIR}/app_g.o: app_g.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/app_g.o app_g.c

${OBJECTDIR}/app2.o: app2.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/app2.o app2.c

${OBJECTDIR}/utility.o: utility.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/utility.o utility.c

${OBJECTDIR}/app_ir.o: app_ir.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/app_ir.o app_ir.c

${OBJECTDIR}/app_sc_ng.o: app_sc_ng.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/app_sc_ng.o app_sc_ng.c

${OBJECTDIR}/memory.o: memory.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/memory.o memory.c

${OBJECTDIR}/app_sp.o: app_sp.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/app_sp.o app_sp.c

${OBJECTDIR}/app2_sc_ng.o: app2_sc_ng.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/app2_sc_ng.o app2_sc_ng.c

${OBJECTDIR}/app_irb.o: app_irb.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/app_irb.o app_irb.c

${OBJECTDIR}/generate.o: generate.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/generate.o generate.c

${OBJECTDIR}/main.o: main.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.c

${OBJECTDIR}/app2_ng.o: app2_ng.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/app2_ng.o app2_ng.c

${OBJECTDIR}/app2_g.o: app2_g.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/app2_g.o app2_g.c

${OBJECTDIR}/app2_irb.o: app2_irb.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/app2_irb.o app2_irb.c

${OBJECTDIR}/app.o: app.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/app.o app.c

${OBJECTDIR}/random.o: random.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/random.o random.c

${OBJECTDIR}/app2_ir.o: app2_ir.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/app2_ir.o app2_ir.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/pga_secuencial.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
