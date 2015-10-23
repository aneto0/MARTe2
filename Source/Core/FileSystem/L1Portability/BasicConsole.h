/*
 * Copyright 2011 EFDA | European Fusion Development Agreement
 *
 * Licensed under the EUPL, Version 1.1 or – as soon they 
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
 * $Id: BasicConsole.h 3 2012-01-15 16:26:07Z aneto $
 *
**/

/** 
 * @file 
 * The simplest human interface: a text terminal
 */
#if !defined (BASIC_CONSOLE_)
#define BASIC_CONSOLE_

#include "TimeoutType.h"

class BasicConsole;
/** flags determining the console operating modes */
enum ConsoleOpeningMode{
    /** Default mode */
    ConsoleDefault        =  0,

    /** WIN32 only: operates on a display buffer different from the current one*/
    CreateNewBuffer       =  1,

    /** Does not wait for \n\r */
    PerformCharacterInput =  2,

    /** CTRL-C does not create an exception */
    DisableControlBreak   =  4,

    /** Enable paging */
    EnablePaging          =  8
};

/** allows combining ConsoleOpeningMode data */
static inline ConsoleOpeningMode operator|(ConsoleOpeningMode a, ConsoleOpeningMode b){
    return (ConsoleOpeningMode )((int)a | (int)b);
}

/** allows combining ConsoleOpeningMode data */
static inline ConsoleOpeningMode operator&(ConsoleOpeningMode a, ConsoleOpeningMode b){
    return (ConsoleOpeningMode )((int)a & (int)b);
}

/** allows combining ConsoleOpeningMode data */
static inline ConsoleOpeningMode Not(ConsoleOpeningMode a){
    return (ConsoleOpeningMode )(~(int)a );
}

extern "C" {
    /** */
    bool ConsoleOpen(BasicConsole &con, ConsoleOpeningMode openingMode,int numberOfColumns, int numberOfRows,TimeoutType msecTimeout = TTInfiniteWait);

    /** */
    bool ConsoleShow(BasicConsole &con);

    /** */
    bool ConsoleClose(BasicConsole &con);

    /** */
    bool ConsoleWrite(BasicConsole &con,const void* buffer,uint32 &size,TimeoutType msecTimeout);

    /** */
    bool ConsoleRead(BasicConsole &con,void* buffer,uint32 &size,TimeoutType msecTimeout);

    /** */
    bool ConsoleSetTitleBar(const char *title);

    /** */
    bool ConsoleSetSize(BasicConsole &con,int numberOfColumns, int numberOfRows);

    /** */
    bool ConsoleGetSize(BasicConsole &con,int &numberOfColumns, int &numberOfRows);

    /** */
    bool ConsoleSetWindowSize(BasicConsole &con,int numberOfColumns, int numberOfRows);

    /** */
    bool ConsoleGetWindowSize(BasicConsole &con,int &numberOfColumns, int &numberOfRows);

    /** */
    bool ConsoleSetCursorPosition(BasicConsole &con,int column, int row);

    /** */
    bool ConsoleGetCursorPosition(BasicConsole &con,int &column, int &row);

    /** */
    bool ConsoleClear(BasicConsole &con);

    /** */
    bool ConsoleSetColour(BasicConsole &con,Colours foreGroundColour,Colours backGroundColour);

    /** */
    bool ConsoleSwitch(BasicConsole &con,const char *name);

    /** */
    bool ConsolePlotChar(BasicConsole &con,char c,Colours foreGroundColour,Colours backGroundColour,int column, int row);

}


/** Implements a tream interface to the console */
class BasicConsole {

#if defined (_WIN32)
    /** */
    HANDLE              inputConsoleHandle;

    /** */
    HANDLE              outputConsoleHandle;
    
#elif defined (_NCURSES)

    WINDOW             *mainwnd;
    
#elif defined (_LINUX)
    
    struct termio       originalConsoleModes;
    
#elif defined(_MACOSX)
    struct termios      originalConsoleModes;
    
#endif

    /** how many lines since last paging ?*/
    int32               lineCount;

    /** how long since last paging */
    int64               lastPagingTime;

    /** how long to wait when reading */
    TimeoutType         msecTimeout;

    /** sets of flags describing the console status*/
    ConsoleOpeningMode  openingMode;

private:

    friend bool ConsoleOpen(BasicConsole &con, ConsoleOpeningMode openingMode,int numberOfColumns, int numberOfRows,TimeoutType msecTimeout);
    friend bool ConsoleClose(BasicConsole &con);
    friend bool ConsoleShow(BasicConsole &con);
    friend bool ConsoleWrite(BasicConsole &con,const void* buffer, uint32 &size,TimeoutType msecTimeout);
    friend bool ConsoleRead(BasicConsole &con,void* buffer, uint32 &size,TimeoutType msecTimeout);
    friend bool ConsoleSetTitleBar(const char *title);
    friend bool ConsoleSetSize(BasicConsole &con,int numberOfColumns, int numberOfRows);
    friend bool ConsoleGetSize(BasicConsole &con,int &numberOfColumns, int &numberOfRows);
    friend bool ConsoleSetCursorPosition(BasicConsole &con,int column, int row);
    friend bool ConsoleSetWindowSize(BasicConsole &con,int numberOfColumns, int numberOfRows);
    friend bool ConsoleGetWindowSize(BasicConsole &con,int &numberOfColumns, int &numberOfRows);
    friend bool ConsoleGetCursorPosition(BasicConsole &con,int &column, int &row);
    friend bool ConsoleClear(BasicConsole &con);
    friend bool ConsoleSetColour(BasicConsole &con,Colours foreGroundColour,Colours backGroundColour);
    friend bool ConsoleSwitch(BasicConsole &con,const char *name);
    friend bool ConsolePlotChar(BasicConsole &con,char c,Colours foreGroundColour,Colours backGroundColour,int column, int row);


public:
    /** Creates a console stream with the desired parameters */
                        BasicConsole(
                                ConsoleOpeningMode  openingMode     = ConsoleDefault,
                                int             numberOfColumns     = -1,
                                int             numberOfRows        = -1,
                                TimeoutType     msecTimeout         = TTInfiniteWait){
        ConsoleOpen(*this,openingMode,numberOfColumns,numberOfRows,msecTimeout);
    }

    /** destructor  */
    virtual ~BasicConsole(){
        ConsoleClose(*this);
    }

    /** Switches to display this console buffer. */
    inline bool         Show(){
        return ConsoleShow(*this);
    }


protected:
    /** The actual Write */
    inline bool        Write(
                                const void*     buffer,
                                uint32 &        size,
                                TimeoutType     msecTimeout){
        return ConsoleWrite(*this,buffer,size,msecTimeout);
    }

    /** The actual Read */
    inline bool        Read(
                                void*           buffer,
                                uint32 &        size,
                                TimeoutType     msecTimeout){
        return ConsoleRead(*this,buffer,size,msecTimeout);
    }

public:

    /** Set Title Bar text */
    inline bool         SetTitleBar(const char *title){
        return ConsoleSetTitleBar(title);
    }

    /** Sets the size of the buffer */
    inline bool         SetSize(
                                int             numberOfColumns,
                                int             numberOfRows){
        return ConsoleSetSize(*this,numberOfColumns, numberOfRows);
    }

    /** Retrieves the size of the buffer */
    inline bool         GetSize (
                                int &           numberOfColumns,
                                int &           numberOfRows){
        return ConsoleGetSize(*this,numberOfColumns, numberOfRows);
    }

    /** Sets the size of the window */
    inline bool         SetWindowSize(
                                int             numberOfColumns,
                                int             numberOfRows){
        return ConsoleSetWindowSize(*this,numberOfColumns,numberOfRows);
    }

    /** Returns the size of the window */
    inline bool         GetWindowSize(
                                int &               numberOfColumns,
                                int &               numberOfRows){
        return ConsoleGetWindowSize(*this,numberOfColumns,numberOfRows);
    }

    /** Sets the size position of the cursor */
    inline bool         SetCursorPosition(
                                int                 column,
                                int                 row){
        return ConsoleSetCursorPosition(*this,column,row);
    }

    /** Retrieves the size position of the cursor */
    inline bool         GetCursorPosition(
                                int &               column,
                                int &               row){
        return ConsoleGetCursorPosition(*this,column,row);
    }

    /** Sets the font fg and bg colours */
    inline bool          SetColour(
                                Colours             foreGroundColour,
                                Colours             backGroundColour){
        return ConsoleSetColour(*this,foreGroundColour,backGroundColour);
    }

    /** Clears content */
    inline bool         Clear(){
        lineCount = 0;
        return ConsoleClear(*this);
    }

    /** enable = False disables it */
    inline void         SetPaging(bool enable){
        lineCount = 0;
        if(enable){
            openingMode = openingMode |  EnablePaging;
        }else{
            openingMode = openingMode & Not(EnablePaging);
        }
    }


    /** write a single char on the console at a given position and a given colour set*/
    inline bool         PlotChar(
                                char                c,
                                Colours             foreGroundColour,
                                Colours             backGroundColour,
                                int                 column,
                                int                 row){
        return ConsolePlotChar(*this,c,foreGroundColour,backGroundColour,column,row);
    }

};

#endif

