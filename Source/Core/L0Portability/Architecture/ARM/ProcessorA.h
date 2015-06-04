/*
 * Copyright 2011 EFDA | European Fusion Development Agreement
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they 
 will be approved by the European Commission - subsequent  
 versions of the EUPL (the "Licence"); 
 * You may not use this work except in compliance with the 
 Licence. 
 * You may obtain a copy of the Licence at: 
 *  
 * http://ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in 
 writing, software distributed under the Licence is 
 distributed on an "AS IS" basis, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either 
 express or implied. 
 * See the Licence for the specific language governing 
 permissions and limitations under the Licence. 
 *
 * $Id: Processor.cpp 43 2012-02-08 17:07:58Z astephen $
 *
 **/

/** @file
  * @brief Functions to obtain the cpu informations.*/



#ifndef PROCESSOR_P_H
#define PROCESSOR_P_H

/**
 * @brief Implementation of the cpuid function for x86 and gcc
 * @param info drives the type of request being asked, e.g. 0 for Vendor, 1 for Family and Model, ...
 * @param eax the CPU EAX register
 * @param ebx the CPU EBX register
 * @param ecx the CPU ECX register
 * @param edx the CPU EDX register
 */
static inline void ProcessorCPUID(uint32 info, uint32 &eax, uint32 &ebx,
                                  uint32 &ecx, uint32 &edx) {

	
}

/** 
 * @see Processor::Family()
 * @brief Get the cpu family.
 * @return the cpu family.
 */
uint32 ProcessorFamily() {
	return 1;
}

/** 
 * @see Processor::Model()
 * @brief Get the cpu model.
 * @return the cpu model.
 */
uint32 ProcessorModel() {
	return 1;
}

/**
 * @brief Stores the processor vendor
 */
static char processorVendorId[13];

/** 
 * @see Processor::VendorId()
 * @brief Get the identifier of the cpu.
 * @return the cpu id.
 */
const char *ProcessorVendorId() {
	return "";
}
#endif

