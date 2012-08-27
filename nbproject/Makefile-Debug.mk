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
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/app_g.o \
	${OBJECTDIR}/app2.o \
	${OBJECTDIR}/utility.o \
	${OBJECTDIR}/memory.o \
	${OBJECTDIR}/app_sp.o \
	${OBJECTDIR}/generate.o \
	${OBJECTDIR}/app_cp.o \
	${OBJECTDIR}/app2_g.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/app.o \
	${OBJECTDIR}/random.o


# C Compiler Flags
CFLAGS=-m64

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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/pga_secuencial

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/pga_secuencial: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/pga_secuencial ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/app_g.o: nbproject/Makefile-${CND_CONF}.mk app_g.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/app_g.o app_g.c

${OBJECTDIR}/app2.o: nbproject/Makefile-${CND_CONF}.mk app2.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/app2.o app2.c

${OBJECTDIR}/utility.o: nbproject/Makefile-${CND_CONF}.mk utility.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/utility.o utility.c

${OBJECTDIR}/memory.o: nbproject/Makefile-${CND_CONF}.mk memory.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/memory.o memory.c

${OBJECTDIR}/app_sp.o: nbproject/Makefile-${CND_CONF}.mk app_sp.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/app_sp.o app_sp.c

${OBJECTDIR}/generate.o: nbproject/Makefile-${CND_CONF}.mk generate.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/generate.o generate.c

${OBJECTDIR}/app_cp.o: nbproject/Makefile-${CND_CONF}.mk app_cp.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/app_cp.o app_cp.c

${OBJECTDIR}/app2_g.o: nbproject/Makefile-${CND_CONF}.mk app2_g.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/app2_g.o app2_g.c

${OBJECTDIR}/main.o: nbproject/Makefile-${CND_CONF}.mk main.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.c

${OBJECTDIR}/app.o: nbproject/Makefile-${CND_CONF}.mk app.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/app.o app.c

${OBJECTDIR}/random.o: nbproject/Makefile-${CND_CONF}.mk random.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/random.o random.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/pga_secuencial

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
