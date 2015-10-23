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

#include "BasicConsole.h"
#include "ErrorManagement.h"
#include "System.h"
#include "HRT.h"
#include "BString.h"

bool ConsoleOpen(
    BasicConsole             &con,
    ConsoleOpeningMode  openingMode,
    int                 numberOfColumns,
    int                 numberOfRows,
    TimeoutType         msecTimeout){

    con.msecTimeout         = msecTimeout;
    con.lineCount           = 0;
    con.lastPagingTime      = 0;
    con.openingMode         = openingMode;
//    con.selectedStream      = NormalStreamMode;

    int stdConsoleColumns;
    int stdConsoleRows;

#if defined (_WIN32)
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&info);

    if (openingMode & CreateNewBuffer){
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

    ConsoleSetSize(con,numberOfColumns,numberOfRows);

    DWORD consoleMode = 0;
    if (openingMode & PerformCharacterInput){
        consoleMode |= 0;
    } else {
        consoleMode |= ENABLE_ECHO_INPUT;
        consoleMode |= ENABLE_LINE_INPUT;
    }

    if (openingMode & DisableControlBreak){
        consoleMode |= 0;
    } else {
        consoleMode |= ENABLE_PROCESSED_INPUT;
    }


    SetConsoleMode(con.inputConsoleHandle,consoleMode);

    FlushConsoleInputBuffer(con.inputConsoleHandle);

#elif defined(_NCURSES) 

    int row, col;
  	con.mainwnd = initscr();  /* Start curses mode */

    curs_set(0);
    start_color();			/* Start color 			*/
        
    noecho();
    cbreak();
    scrollok(con.mainwnd, TRUE);

    nodelay(con.mainwnd, FALSE);
    intrflush(con.mainwnd, TRUE);
    wrefresh(con.mainwnd);
    
    getmaxyx(con.mainwnd, row, col);		/* find the boundaries of the screeen */

    stdConsoleColumns = col;
    stdConsoleRows    = row; 

    if (numberOfColumns <= 0) numberOfColumns = stdConsoleColumns;
    if (numberOfRows <= 0)    numberOfRows    = stdConsoleRows;
    
    ConsoleSetSize(con,numberOfColumns,numberOfRows);
    
    if (openingMode & PerformCharacterInput){
        noecho();
    } else {
        echo();
    } 
	//throws away any typeahead that has been typed by the user and has not 
	//yet been read by the program     
    flushinp(); 
#elif defined(_LINUX) || defined(_MACOSX)
    if (openingMode & PerformCharacterInput){        
#if defined(_MACOSX)
        struct termios modifyConsoleModes;
#else
        struct termio modifyConsoleModes;
#endif
        if(ioctl(fileno(stdin), TCGETA, &con.originalConsoleModes) < 0)
            return False;
        modifyConsoleModes = con.originalConsoleModes;
        modifyConsoleModes.c_lflag &= ~ICANON;
        modifyConsoleModes.c_cc[VMIN] = 1;
        modifyConsoleModes.c_cc[VTIME] = 0;
        ioctl(fileno(stdin), TCSETAW, &modifyConsoleModes);
    }
#elif defined(_RTAI)
    if (openingMode & PerformCharacterInput){        
        fcomm_set_console_performing_char_input(True);
    }
#else
    fflush(stdin);
#endif

    return True;
}

bool ConsoleClose(BasicConsole &con){
#if defined (_WIN32)
    if (con.openingMode & CreateNewBuffer){
        CloseHandle(con.outputConsoleHandle);
    }
    
#elif defined(_NCURSES)
    endwin();
    
#elif defined(_LINUX) || defined(_MACOSX)
    if (con.openingMode & PerformCharacterInput){
        ioctl(fileno(stdin), TCSETAW, &con.originalConsoleModes);
    }
#elif defined(_RTAI)
    if (con.openingMode & PerformCharacterInput){
        fcomm_set_console_performing_char_input(False);
    }
#endif
    return True;
}

bool ConsoleShow(BasicConsole &con){
#if defined (_WIN32)
    return (SetConsoleActiveScreenBuffer(con.outputConsoleHandle)==TRUE);
#else
    return True;
#endif
}


bool ConsoleWrite(BasicConsole &con,const void* buffer, uint32 &size,TimeoutType msecTimeout){

//    if (con.pageSize <= 0){
    int numberOfColumns;
    int numberOfRows;
    if ((con.openingMode & EnablePaging) &&
        (ConsoleGetWindowSize(con,numberOfColumns,numberOfRows))){

        int64 t0 = con.lastPagingTime;
        int64 t1 = HRTRead64();

        int64 dT = t1 - t0;
        double dt = dT * HRT::HRTPeriod();
//        con.AssertErrorCondition(Information,"%f",dt);
        if (dt > 0.05){
            con.lineCount = 0;
            con.lastPagingTime = t1;
        }

        char *p = (char *)buffer;
        uint32 index = 0;
        int start = 0;
        while (index < size){
            while ((con.lineCount < (numberOfRows-1)) && (index<size)){
                if (p[index] == '\n')con.lineCount++;
                index++;
            }
#if defined (_WIN32)
            WriteConsole(con.outputConsoleHandle,p+start,index-start,NULL,NULL);
#elif defined (_RTAI) || defined (_LINUX) || defined(_MACOSX)
            write(STDOUT, p+start, index-start);
#elif defined (_NCURSES)
            mvwaddch(con.mainwnd,(int)p+start,(int)index-start, p[0]);
            wrefresh(con.mainwnd);                        
#else
            fwrite(p+start,1,index-start,stdout);
#endif
            if (con.lineCount >= (numberOfRows-1)){
                start = index;
                con.lastPagingTime = t1;
                con.lineCount = 0;
                const char *message = "[PAGING] ENTER TO CONTINUE\015";
#if defined (_WIN32)
                WriteConsole(con.outputConsoleHandle,message,strlen(message),NULL,NULL);
#elif defined (_RTAI) || defined (_LINUX) || defined(_MACOSX)
                write(STDOUT, message, strlen(message));
#elif defined (_NCURSES) 

                mvwprintw(con.mainwnd,LINES,0,message);
                wrefresh(con.mainwnd);                
#else
                fwrite(message,1,strlen(message),stdout);
#endif
                char buffer[32];
                uint32 size = 1;
                ConsoleRead(con,buffer,size,msecTimeout);
            }
        }
    } else {
#if defined (_WIN32)
        return (WriteConsole(con.outputConsoleHandle,buffer,size,(unsigned long *)&size,NULL));
#else
        uint32 n = size;
#if defined (_RTAI) || defined (_LINUX) || defined(_MACOSX)
        n=write(STDOUT, buffer, n);

#elif defined (_NCURSES)
         
        char* tempbuf = (char*)buffer; 
        for(int k = 0; k<n; k++){   			
   		    waddch(con.mainwnd,tempbuf[k]);
        }        
        wrefresh(con.mainwnd);
        
#else
        n=fwrite(buffer,1,n,stdout);
#endif
        if (n>0)  size = n;
        return (n>0);
#endif
    }
    return True;
}

bool ConsoleRead(BasicConsole &con,void* buffer, uint32 &size,TimeoutType msecTimeout){

#if defined (_WIN32)
    if (con.msecTimeout.IsFinite()){
        DWORD ret;
        FlushConsoleInputBuffer(con.inputConsoleHandle);
        ret = WaitForSingleObject(con.inputConsoleHandle,(DWORD)con.msecTimeout.msecTimeout);
        FlushConsoleInputBuffer(con.inputConsoleHandle);
        ret = WaitForSingleObject(con.inputConsoleHandle,(DWORD)con.msecTimeout.msecTimeout);
        if (ret!=0) {
            size = 0;
            return True;
        }
    }
    DWORD ret = ReadConsole(con.inputConsoleHandle,buffer,size,(unsigned long *)&size,NULL);
    return ret;
#elif defined (_NCURSES)

    echo();  
    int n = size;
    int counter = 0;
    int c = 0;
    char *cBuffer = (char *)buffer;
    while (1){
     
        if(counter == n)
            break;   	
        c = wgetch(con.mainwnd);
        if (c == 10){
        	cBuffer[counter] = '\0';
            break;
        }        
        cBuffer[counter] = (char)c;
        counter++;           
    }
    
    wnoutrefresh(con.mainwnd);
    doupdate();
	return True;
  
#else
    uint32 n = size;
    if(con.openingMode & PerformCharacterInput){    	
    	((char *)buffer)[0] = (char)getchar();	
	size = 1;
    }
    else{
    	char *temp = NULL;
        while((temp = fgets((char *)buffer,n,stdin)) == NULL);
    	if((n = strlen(temp)) > 0) size = n;
    }
    return (n>0);
#endif

}


bool ConsoleSetTitleBar(const char *title){
#if defined (_WIN32)
    return SetConsoleTitle(title);
#else
    return False;
#endif
}

bool ConsoleSetWindowSize(BasicConsole &con,int numberOfColumns, int numberOfRows){
#if defined (_WIN32)
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
        CStaticAssertPlatformErrorCondition(OSError,"BasicConsole:SetWindowSize:failed SetConsoleWindowInfo ");
        return False;
    }
  
#elif defined(_NCURSES)

    int x,y;
    getmaxyx(con.mainwnd,y,x);   /* find the boundaries of the screeen */
    
    if (numberOfColumns > x)          numberOfColumns = x;
    if (numberOfColumns > LINES)      numberOfColumns = LINES;
    if (numberOfRows    > y)          numberOfRows    = y;
    if (numberOfRows    > COLS)       numberOfRows    = COLS;
    if (numberOfColumns < 1    )      numberOfColumns = 1;
    if (numberOfRows    < 1    )      numberOfRows    = 1;
    wresize(con.mainwnd,numberOfRows,numberOfColumns);
    wrefresh(con.mainwnd);


#endif
    return True;
}

bool ConsoleGetWindowSize(BasicConsole &con,int &numberOfColumns, int &numberOfRows){
#if defined (_WIN32)

    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(con.outputConsoleHandle,&info);

    numberOfColumns = info.srWindow.Right  - info.srWindow.Left + 1;
    numberOfRows    = info.srWindow.Bottom - info.srWindow.Top   + 1;
    return True;
    
#else
    numberOfColumns = -1;
    numberOfRows    = -1;
    return False;
#endif
}

bool ConsoleSetSize(BasicConsole &con,int numberOfColumns, int numberOfRows){
#if defined (_WIN32)

    COORD max = GetLargestConsoleWindowSize(con.outputConsoleHandle);

    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(con.outputConsoleHandle,&info);

    COORD stage1BufferSize;
    stage1BufferSize.X =info.dwSize.X;
    if (stage1BufferSize.X < numberOfColumns) stage1BufferSize.X = numberOfColumns;
    stage1BufferSize.Y =info.dwSize.Y;
    if (stage1BufferSize.Y < numberOfRows) stage1BufferSize.Y = numberOfRows;
    if (!SetConsoleScreenBufferSize(con.outputConsoleHandle,stage1BufferSize)){
        CStaticAssertPlatformErrorCondition(OSError,"BasicConsole:SetSize:failed SetConsoleScreenBufferSize ");
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
        CStaticAssertPlatformErrorCondition(OSError,"BasicConsole:SetSize:failed SetConsoleWindowInfo ");
        return False;
    }

    COORD stage2BufferSize;
    stage2BufferSize.X =numberOfColumns;
    stage2BufferSize.Y =numberOfRows;
    return (SetConsoleScreenBufferSize(con.outputConsoleHandle,stage2BufferSize) != FALSE);

#elif defined (_NCURSES)

    wresize(con.mainwnd,numberOfRows,numberOfColumns);
    wrefresh(con.mainwnd);
    return True;
    
#else
    return False;
#endif
}


bool ConsoleGetSize(BasicConsole &con,int &numberOfColumns, int &numberOfRows){
#if defined (_WIN32)
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (GetConsoleScreenBufferInfo(con.outputConsoleHandle,&info)== FALSE) {
        CStaticAssertPlatformErrorCondition(OSError,"BasicConsole:GetSize:failed GetConsoleScreenBufferInfo ");
        return False;
    }
    numberOfColumns = info.dwSize.X;
    numberOfRows    = info.dwSize.Y;
    return True;
    
#elif defined(_NCURSES)
    getmaxyx(con.mainwnd, numberOfRows, numberOfColumns);		/* find the boundaries of the screeen */
    wrefresh(con.mainwnd);
    return True;
    
#else
    return False;
#endif
}

bool ConsoleSetCursorPosition(BasicConsole &con,int column, int row){
#if defined (_WIN32)
    COORD c = {column,row};
    return (SetConsoleCursorPosition(con.outputConsoleHandle,c) == TRUE);

#elif defined (_NCURSES)
    //moves the logical cursor position....we need the refresh() to move the phisical screen cursor
    wmove(con.mainwnd,column,row);
    wrefresh(con.mainwnd);
    return True;
    
#else
    return False;
#endif
}

bool ConsoleGetCursorPosition(BasicConsole &con,int &column, int &row){
#if defined (_WIN32)
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (GetConsoleScreenBufferInfo(con.outputConsoleHandle,&info)== FALSE) return False;
    column = info.dwCursorPosition.X;
    row = info.dwCursorPosition.Y;
    return True;
    
#elif defined (_NCURSES)  
    getyx(con.mainwnd,column,row);
    return True;   
        
#else
    return False;
#endif
}


bool ConsoleSetColour(BasicConsole &con,Colours foreGroundColour,Colours backGroundColour){

#if defined (_WIN32)

    WORD attribute;
    attribute = (int)foreGroundColour & 0xF;
    attribute |= ((int)backGroundColour & 0xF) << 4;

    if (!SetConsoleTextAttribute(con.outputConsoleHandle,attribute)) return False;

    return True;
#elif defined(_RTAI) || defined (_LINUX) || defined(_MACOSX)
    char command[13];
    //Use dark or not...
    int attr = (foreGroundColour < Grey);
    int fg = foreGroundColour;
    int bg = backGroundColour;
    if(fg > Grey){
        fg -= Grey;
    }
    if(bg > Grey){
        bg -= Grey;
    }
        
    if(fg == Grey){
        fg = White;
    }
    if(bg == Grey){
        bg = White;
    }
    /* Command is the control command to the terminal */
    sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
    write(1, command, strlen(command));

#elif defined (_NCURSES) 
    if(!has_colors()){
        return False;       
    }
    
    int fg = foreGroundColour;
    int bg = backGroundColour;

	//dark colours and Grey not availables!!	
    if(fg == Grey)
	   fg = White; 
	if(bg == Grey)
	   bg = White;
	
	if(fg == DarkGrey)
	   fg = Black; 
	if(bg == DarkGrey)
	   bg = Black;
	   
	if(fg > Grey)
	   fg = fg - Grey;
	if(bg > Grey)
	   bg = bg - Grey;
	   	
	char command[13];
	int attr = (foreGroundColour < Grey);
	sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
	write(1, command, strlen(command));
	
    return True;    
#else
    return False;
#endif

}

bool ConsoleClear(BasicConsole &con){
#if defined (_WIN32)
    COORD c;
    c.X = 0;
    c.Y = 0;

    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(con.outputConsoleHandle,&info);
    int nOfChars = info.dwSize.Y * info.dwSize.X;

    FillConsoleOutputAttribute(con.outputConsoleHandle,info.wAttributes,nOfChars,c,NULL);
    return (FillConsoleOutputCharacter(con.outputConsoleHandle,' ',nOfChars,c,NULL));

#elif defined(_NCURSES)
    wclear(con.mainwnd);
    wrefresh(con.mainwnd);

#elif defined(_RTAI) || defined(_LINUX) || defined(_MACOSX)
	for (int i=0;i<40;i++) {
		write(STDOUT, "\n", 1);
	}
	
#else
    return False;
#endif
}

bool ConsolePlotChar(BasicConsole &con,char c,Colours foreGroundColour,Colours backGroundColour,int column, int row){
#if defined (_WIN32)
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

#else
    if (!ConsoleSetCursorPosition(con,column,row)) return False;
    if (!ConsoleSetColour(con,foreGroundColour,backGroundColour)) return False;
    uint32 size = 1;
    con.Write((const void *)&c, size, 0);
    return False;
#endif
}

