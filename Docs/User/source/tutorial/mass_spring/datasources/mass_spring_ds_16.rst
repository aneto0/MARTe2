.. date: 10/04/2026
   author: Andre' Neto
   copyright: Copyright 2017 F4E | European Joint Undertaking for ITER and
   the Development of Fusion Energy ('Fusion for Energy').
   Licensed under the EUPL, Version 1.1 or - as soon they will be approved
   by the European Commission - subsequent versions of the EUPL (the "Licence")
   You may not use this work except in compliance with the Licence.
   You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
   warning: Unless required by applicable law or agreed to in writing, 
   software distributed under the Licence is distributed on an "AS IS"
   basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
   or implied. See the Licence permissions and limitations under the Licence.

Other DataSources
=================

This section concludes the introduction to the DataSources used in the mass-spring-damper example, listing other commonly used DataSources that are not covered in the previous sections.

- :vcisdoxygenmccl:`MDSReader`, :vcisdoxygenmccl:`MDSWriter`: Read/write from/to `MDSplus <https://www.mdsplus.org/>`__ trees.
- :vcisdoxygenmccl:`NI1588Timestamp`: Timestamp events using the NI-1588 board.
- :vcisdoxygenmccl:`NI6259ADC`, :vcisdoxygenmccl:`NI6259DAC`, :vcisdoxygenmccl:`NI6259DIO`: Read and write from the National Instruments NI-6259 DAQ card.
- :vcisdoxygenmccl:`NI6368ADC`, :vcisdoxygenmccl:`NI6368DAC`, :vcisdoxygenmccl:`NI6368DIO`: Read and write from the National Instruments NI-6368 DAQ card.
- :vcisdoxygenmccl:`NI9157MxiDataSource`, :vcisdoxygenmccl:`NI9157CircularFifoReader`: Interface to the NI-9157 Mxi data acquisition system.
- :vcisdoxygenmccl:`UARTDataSource`: UART reader.
- :vcisdoxygenmccl:`UEIReader`, :vcisdoxygenmccl:`UEIWriter`: `United Electronic Industries (UEI) <https://www.ueidaq.com/>`__ read/write from UEIDAQ hardware.