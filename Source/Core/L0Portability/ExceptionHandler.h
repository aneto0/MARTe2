/*
 * Copyright 2015 F4E | European Joint Undertaking for 
 * ITER and the Development of Fusion Energy ('Fusion for Energy')
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
 * See the Licence  
 permissions and limitations under the Licence. 
 *
 * $Id: $
 *
 **/
/**
 * @file
 * @brief Exception handling interface. Place holder for future implementations.
 */
#ifndef EXCEPTION_HANDLER
#define EXCEPTION_HANDLER

/** @brief Exception handler plugin interface. */
class ExceptionHandler {
private:
    /** What to do in case of exception. */
    uint32 action;
public:
    /** Pass handling back to OS. */
    static const uint32 NotHandled = 0x1;
    /** Handled: program can continue. */
    static const uint32 ContinueExec = 0x2;
    /** Handled: jump to thread start and proceed to termination. */
    static const uint32 KillThread = 0x3;
    /** Handled: jump to thread start and terminate task. */
    static const uint32 KillTask = 0x4;
    /** Handled: jump to thread start and retry. */
    static const uint32 TryAgain = 0x5;
    /** Pass handling back to other handlers in stack. */
    static const uint32 TryOther = 0x6;
    /** do not report error information (ored). */
    static const uint32 NoReport = 0x10000;

public:
    /** @brief Default handler. */
    ExceptionHandler(uint32 action = NotHandled) {
        this->action = action;
    }

    /** This is the action to be performed. */
    /*virtual uint32 Catch(ExceptionInformation &info){
     return action;
     }*/
};

#endif

