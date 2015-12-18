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


OBJSX= AnyObjectGTest.x \
		AnyTypeGTest.x \
		AnyTypeCreatorGTest.x \
		AtomicGTest.x \
		BasicConsoleGTest.x \
		BasicFileGTest.x \
		BasicSocketGTest.x \
		BasicTCPSocketGTest.x \
		BasicTypeGTest.x \
		BasicUDPSocketGTest.x \
		BitBooleanGTest.x \
		BitRangeGTest.x \
		BitSetToIntegerGTest.x \
		BufferedStreamIGTest.x \
		BufferedStreamIOBufferGTest.x \
		CharBufferGTest.x \
		ClassPropertiesGTest.x \
		ClassRegistryDatabaseGTest.x \
		ClassRegistryItemGTest.x \
		ConfigurationDatabaseGTest.x \
		DirectoryGTest.x \
		DirectoryScannerGTest.x \
		DoubleBufferedStreamGTest.x \
		DoubleIntegerGTest.x \
		EndianityGTest.x \
		ErrorManagementGTest.x \
		EventSemGTest.x \
		FastMathGTest.x \
		FastPollingMutexSemGTest.x \
		FormatDescriptorGTest.x \
		FractionalIntegerGTest.x \
		GlobalObjectsDatabaseGTest.x \
HeapManagerGTest.x \
		HighResolutionTimerGTest.x \
		IOBufferGTest.x \
		InternetHostGTest.x \
		InternetServiceGTest.x \
		JsonParserGTest.x \
		LexicalAnalyzerGTest.x \
		LinkedListHolderGTest.x \
		LinkedListableGTest.x \
		LoadableLibraryGTest.x \
		MatrixGTest.x \
		MemoryGTest.x \
		MutexSemGTest.x \
		ObjectGTest.x \
		ProcessorGTest.x \
		ProcessorTypeGTest.x \
		ReferenceContainerFilterObjectNameGTest.x \
		ReferenceContainerFilterReferencesGTest.x \
		ReferenceContainerGTest.x \
		ReferenceContainerNodeGTest.x \
		ReferenceGTest.x \
		ReferenceTGTest.x \
		SelectGTest.x \
		ShiftGTest.x \
		SingleBufferedStreamGTest.x \
		SleepGTest.x \
		StandardHeap_GenericGTest.x \
		StaticListGTest.x \
		StreamMemoryReferenceGTest.x \
		StreamStringGTest.x \
		StreamStringIOBufferGTest.x \
		StringHelperGTest.x \
		StringPortableGTest.x \
		ThreadsGTest.x \
		TimeStampGTest.x \
		TimeoutTypeGTest.x \
		TokenGTest.x \
		TokenInfoGTest.x \
		TypeCharacteristicsGTest.x \
		TypeConversionGTest.x \
		TypeDescriptorGTest.x  \
		VectorGTest.x \
		StandardParserGTest.x \
		XMLParserGTest.x 

PACKAGE=
ROOT_DIR=../..
MAKEDEFAULTDIR=$(ROOT_DIR)/MakeDefaults

include $(MAKEDEFAULTDIR)/MakeStdLibDefs.$(TARGET)

INCLUDES += -I../ 
INCLUDES += -I../Core/BareMetal/L0Types
INCLUDES += -I../Core/BareMetal/L1Portability
INCLUDES += -I../Core/BareMetal/L2Objects
INCLUDES += -I../Core/BareMetal/L3Streams
INCLUDES += -I../Core/BareMetal/L4Configuration
INCLUDES += -I../Core/Scheduler/L1Portability
INCLUDES += -I../Core/FileSystem/L1Portability
INCLUDES += -I../../Source/Core/BareMetal/L0Types
INCLUDES += -I../../Source/Core/BareMetal/L1Portability
INCLUDES += -I../../Source/Core/BareMetal/L2Objects
INCLUDES += -I../../Source/Core/BareMetal/L3Streams
INCLUDES += -I../../Source/Core/BareMetal/L4Configuration
INCLUDES += -I../../Source/Core/Scheduler/L1Portability
INCLUDES += -I../../Source/Core/FileSystem/L1Portability
INCLUDES += -I../../Lib/gtest-1.7.0/include

all: $(OBJS)    \
        $(BUILD_DIR)/MainGTest$(EXEEXT) 
	echo  $(OBJS)

include depends.$(TARGET)

include $(MAKEDEFAULTDIR)/MakeStdLibRules.$(TARGET)

