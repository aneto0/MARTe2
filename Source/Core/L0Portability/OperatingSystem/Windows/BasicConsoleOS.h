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
 * $Id: BasicConsole.cpp 3 2012-01-15 16:26:07Z aneto $
 *
**/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <setjmp.h>
#include <float.h>
#include <math.h>
#include <io.h>
#include <sys/types.h>
#include <windows.h>
#include <winuser.h>
#include <winbase.h>
#include <process.h>
#include <conio.h>
#include <typeinfo.h>
#include "../../HighResolutionTimer.h"

bool BasicConsoleOSSetSize(BasicConsole &con,int numberOfColumns, int numberOfRows){

    COORD max = GetLargestConsoleWindowSize(con.outputConsoleHandle);

    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(con.outputConsoleHandle,&info);

    COORD stage1BufferSize;
    stage1BufferSize.X =info.dwSize.X;
    if (stage1BufferSize.X < numberOfColumns) stage1BufferSize.X = numberOfColumns;
    stage1BufferSize.Y =info.dwSize.Y;
    if (stage1BufferSize.Y < numberOfRows) stage1BufferSize.Y = numberOfRows;
    if (!SetConsoleScreenBufferSize(con.outputConsoleHandle,stage1BufferSize)){
       // CStaticAssertPlatformErrorCondition(OSError,"BasicConsole:SetSize:failed SetConsoleScreenBufferSize ");
        return False;
    }

    int windowColumns = info.srWindow.Right  - info.srWindow.Left + 1;
    int windowRows    = info.srWindow.Bottom - info.srWindow.Top   + 1;

    if (windowColumns > numberOfColumns)  windowColumns = numberOfColumns;
    if (windowRows    > numberOfRows)     windowRows    = numberOfRows;

    SMALL_RECT srect;
    srect.Left   = info.srWindow.Left;
    srect.Top    = info.srWindow.Top;
    srect.Right  = srect.Left + windowColumns - 1;
    srect.Bottom = srect.Top  + windowRows - 1;

    if (!SetConsoleWindowInfo(con.outputConsoleHandle,TRUE,&srect)){
      //  CStaticAssertPlatformErrorCondition(OSError,"BasicConsole:SetSize:failed SetConsoleWindowInfo ");
        return False;
    }

    COORD stage2BufferSize;
    stage2BufferSize.X =numberOfColumns;
    stage2BufferSize.Y =numberOfRows;
    return (SetConsoleScreenBufferSize(con.outputConsoleHandle,stage2BufferSize) != FALSE);

}

bool BasicConsoleOSOpen(
    BasicConsole             &con,
    int                 numberOfColumns,
    int                 numberOfRows){

//    con.selectedStream      = NormalStreamMode;

    int stdConsoleColumns;
    int stdConsoleRows;

    CONSOLE_SCREEN_BUFFER_INFO info;
    //get the screen information (size, cursor position, ecc...)
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&info);

    if (con.openingMode & CreateNewBuffer){
        //get the console handles
        con.inputConsoleHandle  = GetStdHandle(STD_INPUT_HANDLE);
        con.outputConsoleHandle = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,0,NULL,CONSOLE_TEXTMODE_BUFFER,NULL);
    } else {
        con.inputConsoleHandle  = GetStdHandle(STD_INPUT_HANDLE);
        con.outputConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    }


    stdConsoleColumns = info.dwSize.X;
    stdConsoleRows    = info.dwSize.Y;

    if (numberOfColumns <= 0) numberOfColumns = stdConsoleColumns;
    if (numberOfRows <= 0)    numberOfRows    = stdConsoleRows;

    BasicConsoleOSSetSize(con,numberOfColumns,numberOfRows);

    DWORD consoleMode = 0;
    if (con.openingMode & PerformCharacterInput){
        consoleMode |= 0;
    } else {
        consoleMode |= ENABLE_ECHO_INPUT;
        consoleMode |= ENABLE_LINE_INPUT;
    }

    if (con.openingMode & DisableControlBreak){
        consoleMode |= 0;
    } else {
        consoleMode |= ENABLE_PROCESSED_INPUT;
    }


    SetConsoleMode(con.inputConsoleHandle,consoleMode);

    FlushConsoleInputBuffer(con.inputConsoleHandle);

    return True;
}

bool BasicConsoleOSClose(BasicConsole &con){
    if (con.openingMode & CreateNewBuffer){
        CloseHandle(con.outputConsoleHandle);
    }


    return True;
}

bool BasicConsoleOSShow(BasicConsole &con){
    return (SetConsoleActiveScreenBuffer(con.outputConsoleHandle)==TRUE);
}


bool BasicConsoleOSWrite(BasicConsole &con,const void* buffer, uint32 &size){

        return WriteConsole(con.outputConsoleHandle,buffer,size,(unsigned long *)&size,NULL);

}

bool BasicConsoleOSRead(BasicConsole &con,void* buffer, uint32 &size,TimeoutType msecTimeout){
    DWORD ret=0;
    if (con.msecTimeout.IsFinite()){
       
        FlushConsoleInputBuffer(con.inputConsoleHandle);
        ret = WaitForSingleObject(con.inputConsoleHandle,(DWORD)con.msecTimeout.msecTimeout);
        FlushConsoleInputBuffer(con.inputConsoleHandle);
        ret = WaitForSingleObject(con.inputConsoleHandle,(DWORD)con.msecTimeout.msecTimeout);
        if (ret!=0) {
            size = 0;
            return True;
        }
    }

	if(con.openingMode & PerformCharacterInput){
	    size=1;
	    DWORD ret = ReadConsole(con.inputConsoleHandle,buffer,size,(unsigned long *)&size,NULL);	
	}
	else{
            //in this cas buffer is a string accordingly with linux behavior.
    	    DWORD ret = ReadConsole(con.inputConsoleHandle,buffer,size-1,(unsigned long *)&size,NULL);

	    //anyway terminate the string
	    ((char*)buffer)[size]='\0';
        }
    return ret!=0;


}


bool BasicConsoleOSSetTitleBar(BasicConsole &con, const char *title){
    return SetConsoleTitle(title);

}

bool BasicConsoleOSSetWindowSize(BasicConsole &con,int numberOfColumns, int numberOfRows){
    COORD max = GetLargestConsoleWindowSize(con.outputConsoleHandle);
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(con.outputConsoleHandle,&info);

    if (numberOfColumns > max.X)         numberOfColumns = max.X;
    if (numberOfColumns > info.dwSize.X) numberOfColumns = info.dwSize.X;
    if (numberOfRows    > max.Y)         numberOfRows    = max.Y;
    if (numberOfRows    > info.dwSize.Y) numberOfRows    = info.dwSize.Y;
    if (numberOfColumns < 1    )         numberOfColumns = 1;
    if (numberOfRows    < 1    )         numberOfRows    = 1;

    SMALL_RECT srect;
    srect.Left   = info.srWindow.Left;
    srect.Top    = info.srWindow.Top;
    srect.Right  = srect.Left + numberOfColumns - 1;
    srect.Bottom = srect.Top  + numberOfRows - 1;

    if (!SetConsoleWindowInfo(con.outputConsoleHandle,TRUE,&srect)){
      //  CStaticPlatformErrorCondition(OSError,"BasicConsole:SetWindowSize:failed SetConsoleWindowInfo ");
        return False;
    }

    return True;
}

bool BasicConsoleOSGetWindowSize(BasicConsole &con,int &numberOfColumns, int &numberOfRows){

    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(con.outputConsoleHandle,&info);

    numberOfColumns = info.srWindow.Right  - info.srWindow.Left + 1;
    numberOfRows    = info.srWindow.Bottom - info.srWindow.Top   + 1;
    return True;


}


bool BasicConsoleOSGetSize(BasicConsole &con,int &numberOfColumns, int &numberOfRows){
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (GetConsoleScreenBufferInfo(con.outputConsoleHandle,&info)== FALSE) {
     //  CStaticAssertPlatformErrorCondition(OSError,"BasicConsole:GetSize:failed GetConsoleScreenBufferInfo ");
        return False;
    }
    numberOfColumns = info.dwSize.X;
    numberOfRows    = info.dwSize.Y;
    return True;


}

bool BasicConsoleOSSetCursorPosition(BasicConsole &con,int column, int row){
    COORD c = {column,row};
    return (SetConsoleCursorPosition(con.outputConsoleHandle,c) == TRUE);
}

bool BasicConsoleOSGetCursorPosition(BasicConsole &con,int &column, int &row){
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (GetConsoleScreenBufferInfo(con.outputConsoleHandle,&info)== FALSE) return False;
    column = info.dwCursorPosition.X;
    row = info.dwCursorPosition.Y;
    return True;
}


bool BasicConsoleOSSetColour(BasicConsole &con,Colours foreGroundColour,Colours backGroundColour){


    WORD attribute;
    attribute = (int)foreGroundColour & 0xF;
    attribute |= ((int)backGroundColour & 0xF) << 4;

    if (!SetConsoleTextAttribute(con.outputConsoleHandle,attribute)) return False;

    return True;

}

bool BasicConsoleOSClear(BasicConsole &con){
    COORD c;
    c.X = 0;
    c.Y = 0;

    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(con.outputConsoleHandle,&info);
    int nOfChars = info.dwSize.Y * info.dwSize.X;

    FillConsoleOutputAttribute(con.outputConsoleHandle,info.wAttributes,nOfChars,c,NULL);
    return (FillConsoleOutputCharacter(con.outputConsoleHandle,' ',nOfChars,c,NULL));


}

bool BasicConsoleOSPlotChar(BasicConsole &con,char c,Colours foreGroundColour,Colours backGroundColour,int column, int row){

    COORD coord;
    coord.X = 0;
    coord.Y = 0;

    COORD bufferSize;
    bufferSize.X = 1;
    bufferSize.Y = 1;

    WORD attribute;
    attribute = (int)foreGroundColour & 0xF;
    attribute |= ((int)backGroundColour & 0xF) << 4;

    CHAR_INFO cInfo;
    cInfo.Char.AsciiChar = c;
    cInfo.Attributes = attribute;

    SMALL_RECT writeRegion;
    writeRegion.Left   = row;
    writeRegion.Right  = row;
    writeRegion.Top    = column;
    writeRegion.Bottom = column;

    return (WriteConsoleOutput(con.outputConsoleHandle,&cInfo,bufferSize,coord,&writeRegion) != FALSE);

}

