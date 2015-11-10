#############################################################
#
# Copyright 2015 F4E | European Joint Undertaking for ITER 
#  and the Development of Fusion Energy ('Fusion for Energy')
# 
# Licensed under the EUPL, Version 1.1 or - as soon they 
# will be approved by the European Commission - subsequent  
# versions of the EUPL (the "Licence"); 
# You may not use this work except in compliance with the 
# Licence. 
# You may obtain a copy of the Licence at: 
#  
# http://ec.europa.eu/idabc/eupl
#
# Unless required by applicable law or agreed to in 
# writing, software distributed under the Licence is 
# distributed on an "AS IS" basis, 
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either 
# express or implied. 
# See the Licence for the specific language governing 
# permissions and limitations under the Licence. 
#
#############################################################


OBJSX= DirectoryScannerGTest.x \
        DirectoryGTest.x
#InternetHostGTest.x \
	   InternetServiceGTest.x \
	   BasicSocketGTest.x \
	   BasicTCPSocketGTest.x \
	   BasicConsoleGTest.x \
	   DoubleIntegerGTest.x \
	TypeCharacteristicsGTest.x \
        ShiftGTest.x \
        BasicTypeGTest.x \
        FormatDescriptorGTest.x \
        AnyTypeGTest.x\
        StandardHeap_GenericGTest.x \
        HeapManagerGTest.x \
        EndianityGTest.x \
	SleepGTest.x \
	ProcessorGTest.x \
	AtomicGTest.x \
	ProcessorTypeGTest.x \
	EventSemGTest.x \
	HighResolutionTimerGTest.x \
	MemoryGTest.x \
	FastPollingMutexSemGTest.x \
	StringHelperGTest.x \
	StringPortableGTest.x \
	ThreadsGTest.x \
	MutexSemGTest.x \
	TimeoutTypeGTest.x \
	LoadableLibraryGTest.x \
	FastMathGTest.x \
	LinkedListableGTest.x \
	LinkedListHolderGTest.x \
	ClassRegistryDatabaseGTest.x \
	ClassPropertiesGTest.x \
	ClassRegistryItemGTest.x \
	ObjectGTest.x \
	ReferenceGTest.x \
	ReferenceContainerGTest.x \
	ReferenceTGTest.x \
	ReferenceContainerNodeGTest.x \
	ReferenceContainerFilterReferencesGTest.x \
	ReferenceContainerFilterObjectNameGTest.x \
 	ErrorManagementGTest.x \
    	BitBooleanGTest.x \
 	FractionalIntegerGTest.x \
 	BitRangeGTest.x \
 	StaticListGTest.x \
 	GlobalObjectsDatabaseGTest.x \
	TypeDescriptorGTest.x  \
	CharBufferGTest.x \
        StringIOBufferGTest.x \
        SingleBufferedStreamGTest.x \
        DoubleBufferedStreamGTest.x \
        BufferedStreamIOBufferGTest.x \
        StringGTest.x \
        StreamMemoryReferenceGTest.x \
        BufferedStreamIGTest.x \
        IOBufferGTest.x \
        BitSetToIntegerGTest.x \

	
	


PACKAGE=
ROOT_DIR=../..
MAKEDEFAULTDIR=$(ROOT_DIR)/MakeDefaults

include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)

INCLUDES += -I../ 
INCLUDES += -I../Core/BareMetal/L0Types
INCLUDES += -I../Core/BareMetal/L1Portability
INCLUDES += -I../Core/BareMetal/L2Objects
INCLUDES += -I../Core/BareMetal/L3Streams
INCLUDES += -I../Core/Scheduler/L1Portability
INCLUDES += -I../Core/FileSystem/L1Portability
INCLUDES += -I../../Source/Core/BareMetal/L0Types
INCLUDES += -I../../Source/Core/BareMetal/L1Portability
INCLUDES += -I../../Source/Core/BareMetal/L2Objects
INCLUDES += -I../../Source/Core/BareMetal/L3Streams
INCLUDES += -I../../Source/Core/Scheduler/L1Portability
INCLUDES += -I../../Source/Core/FileSystem/L1Portability
INCLUDES += -I../../Lib/gtest-1.7.0/include

all: $(OBJS)    \
        $(BUILD_DIR)/MainGTest$(EXEEXT)
	echo  $(OBJS)

include depends.$(TARGET)

include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)

