#include <windows.h>
#include <WinNT.h>

#include <stdio.h>
#include <stdlib.h>

#include <map>
#include <string>

#define PEEF_PRINT_ORDINALS              0x01
#define PEEF_ORDER_BY_ORDINALS           0x02
#define PEEF_MAKE_UNDERSCORE_ALIAS       0x04
#define PEEF_MAKE_NO_UNDERSCORE_ALIAS    0x08
#define PEEF_USE_FILENAME                0x10
#define PEEF_CALL_LIB                    0x20
#define PEEF_VERBOSE                     0x40


void printPeExports(FILE *pf, const char *filename, DWORD *base, DWORD flags) 
{ 
  PIMAGE_DOS_HEADER             pDOSHeader = (PIMAGE_DOS_HEADER) base;
  PIMAGE_NT_HEADERS             pNTHeaders = (PIMAGE_NT_HEADERS)((const char*)base + pDOSHeader->e_lfanew);
  PIMAGE_EXPORT_DIRECTORY       pExportDir = (PIMAGE_EXPORT_DIRECTORY)((const char*)base + pNTHeaders->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT ].VirtualAddress);
  PDWORD                        pAddr = (DWORD *)((const char*)base + pExportDir->AddressOfFunctions);
  PWORD                         pOrdinals = (WORD *)((const char*)base + pExportDir->AddressOfNameOrdinals);
  PDWORD                        pName = (DWORD *)((const char*)base + pExportDir->AddressOfNames);

  ::std::map< WORD, ::std::string > byOrdinal;
  //::std::map< ::std::string > byOrdinal;
  const char* pModuleName = (const char*)((const char*)base + pExportDir->Name);

  fprintf(pf, "LIBRARY %s\n", (flags&PEEF_USE_FILENAME) ? filename : (const char *)(pModuleName));
  fprintf(pf, "EXPORTS\n");

  for(DWORD i=0; i<pExportDir->NumberOfNames; i++) 
     {
      if (flags&PEEF_ORDER_BY_ORDINALS)
         {
          byOrdinal[(WORD)(pExportDir->Base) + pOrdinals[ i ] ]  = (const char*)base+pName[ i ];
         }
      else
         {
          const char* fnName = (const char*)((const char*)base+pName[ i ]);
          fprintf(pf, "    %s", fnName );
          if (flags&PEEF_PRINT_ORDINALS)
             fprintf(pf, "    @%d", pExportDir->Base + pOrdinals[ i ]);
          fprintf(pf, "\n");

          if (flags&PEEF_MAKE_NO_UNDERSCORE_ALIAS && *fnName=='_')
             {
              const char* newName = fnName;
              for(; *newName && *newName=='_'; ++newName) {}
              fprintf(pf, "    %s=%s\n", newName,fnName );
             }

          if (flags&PEEF_MAKE_UNDERSCORE_ALIAS)
             fprintf(pf, "    _%s=%s\n", fnName,fnName );

          fflush(pf);
         }
     }
  if (flags&PEEF_ORDER_BY_ORDINALS)
     {
      ::std::map< WORD, ::std::string >::const_iterator it = byOrdinal.begin();
      for(; it != byOrdinal.end(); ++it)
         {
          fprintf(pf, "    %s", (const char*)(it->second.c_str()));
          if (flags&PEEF_PRINT_ORDINALS)
             fprintf(pf, "    @%d", it->first );
          fprintf(pf, "\n");
          if (flags&PEEF_MAKE_NO_UNDERSCORE_ALIAS && *it->second.c_str()=='_')
             {
              const char* newName = it->second.c_str();
              for(; *newName && *newName=='_'; ++newName) {}
              fprintf(pf, "    %s=%s\n", newName,it->second.c_str() );
             }
          if (flags&PEEF_MAKE_UNDERSCORE_ALIAS)
             fprintf(pf, "    _%s=%s\n", it->second.c_str(),it->second.c_str() );
          fflush(pf);
         }
     }
}



void printHelp()
   {
    // stderr
    fprintf(stderr, "Usage: expdef [-v] [-dname] [-l] [-p] [-o] [-u] [-r] [-f] filename.dll\n");
    fprintf(stderr, "Where:\n");
    fprintf(stderr, "      -v     - verbose output\n");
    fprintf(stderr, "      -dname - output def file name (stdout used if not taken)\n");
    fprintf(stderr, "      -l     - invoke LIB tool automaticaly (requires -d option)\n");
    fprintf(stderr, "      -p     - print ordinals\n");
    fprintf(stderr, "      -o     - order by ordinals\n");
    fprintf(stderr, "      -u     - create underscored aliases\n");
    fprintf(stderr, "      -r     - create ununderscored aliases (remove underscore if present)\n");
    fprintf(stderr, "      -f     - use filename instead of internal module name\n");
   }

int main(int argc, char* argv[])
   {
    DWORD flags = 0;
    const char * name = 0; // DLL name
    const char * defFileName = 0;

    if (argc<2)
       {
        fprintf(stderr, "Error: missing parameter\n");
        printHelp();
        return 1;
       }

    for(int i=1; i!=argc; ++i)
       {
        char *pOpt = argv[ i ];
        if (*pOpt=='-' || *pOpt=='/') 
           {
            ++pOpt;
            if (*pOpt=='p')
               {
                flags |= PEEF_PRINT_ORDINALS;
               }
            else if (*pOpt=='o')
               {
                flags |= PEEF_ORDER_BY_ORDINALS;
               }
            else if (*pOpt=='u')
               {
                flags |= PEEF_MAKE_UNDERSCORE_ALIAS;
               }
            else if (*pOpt=='f')
               {
                flags |= PEEF_USE_FILENAME;
               }
            else if (*pOpt=='r')
               {
                flags |= PEEF_MAKE_NO_UNDERSCORE_ALIAS;
               }
            else if (*pOpt=='l')
               {
                flags |= PEEF_CALL_LIB;
               }
            else if (*pOpt=='v')
               {
                flags |= PEEF_VERBOSE;
               }
            else if (*pOpt=='d')
               {
                ++pOpt;
                if (!(*pOpt))
                   {
                    fprintf(stderr, "Error: invalid -d option value\n");
                    printHelp();
                    return 2;
                   }
                defFileName = pOpt;
               }
            else
               {
                fprintf(stderr, "Error: unknown option\n");
                printHelp();
                return 2;
               }
            }
        else
           {
            name = pOpt;
           }
       }

    if (!name)
       {
        fprintf(stderr, "Error: missing library name\n");
        printHelp();
        return 3;
       }

    if ((flags&PEEF_MAKE_NO_UNDERSCORE_ALIAS) && (flags&PEEF_MAKE_UNDERSCORE_ALIAS))
       {
        fprintf(stderr, "Error: options -u and -r are mutually exclusive\n");
        printHelp();
        return 4;
       }

    const char *pFilename = name;
    const char *tmp = pFilename;
    for(; *tmp; ++tmp )
       {
        if (*tmp=='\\')
           {
            ++tmp;
            if (*tmp) pFilename = tmp;
           }
       }

    DWORD *pDll = (DWORD*)LoadLibrary(name);
    if (!pDll)
       {
        fprintf(stderr, "Error: failed to load library %s, code: %d\n", name, ::GetLastError() );
        return 5;
       }


    FILE *fDef = stdout;
    if (defFileName)
       {
        fDef = fopen( defFileName, "wt");
        if (!fDef)
           {
            fprintf(stderr, "Error: failed to open output def file %s\n", defFileName );
            return 6;
           }
       }

    printPeExports( fDef, pFilename, pDll, flags );

    fclose(fDef);

    if (flags & PEEF_CALL_LIB)
       {
        if (!defFileName)
           {
            fprintf(stderr, "Error: -l option requires -d option too\n" );
            return 6;
           }
        char buf[4096] = "LIB";
        if (flags&PEEF_VERBOSE)
           strcat(buf," /VERBOSE");
        else
           strcat(buf," /NOLOGO");
        strcat(buf," /MACHINE:IX86");
 
        strcat(buf," /DEF:");
        strcat(buf,defFileName);
    
        if (flags&PEEF_VERBOSE)
           fprintf(stderr, "Executin LIB tool: %s\n", buf );
    
        system( buf );
       }

    return 0;
   }
