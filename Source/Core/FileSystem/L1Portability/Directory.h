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
 * $Id: Directory.h 3 2012-01-15 16:26:07Z aneto $
 *
**/

/** 
 * @file 
 * Directory support (e.g. scan directories, ...)
 */
#if !defined(DIRECTORY)
#define DIRECTORY

#include "System.h"
#include "LinkedListHolder.h"
#if (defined(_LINUX) || defined(_SOLARIS) || defined(_MACOSX))
#include <fnmatch.h>
#endif

class Directory;


#if (defined(_LINUX) || defined(_SOLARIS) || defined(_MACOSX))
/**This function is called by the scandir and it is used for file filtering*/
static char *fileFilterSearchMask;
#if defined(_MACOSX)
static int fileFilter (struct dirent *de){
#else
static int fileFilter (const struct dirent *de){
#endif
    return (fnmatch(fileFilterSearchMask, de->d_name, 0) == 0);
}
#endif

/** it represents a file or a subdirectory */
class DirectoryEntry: public LinkedListable{
    /** the file or directory name */
    const char *fname;

    friend class Directory;
#if (defined (_VXWORKS) || defined(_RTAI) || defined(_LINUX) || (defined _SOLARIS) || defined(_MACOSX))
    /** attributes of the file */
    struct stat fileStatistics;
#elif defined(_WIN32)
    /** all the data about the file or directory */
    WIN32_FIND_DATA fileData;
#else
    /** all the data about the file or directory */
    _finddata_t info;
#endif
    /** the container */
    Directory *holder;
public:
    /** constructor */
    DirectoryEntry(Directory *holder, const char *fname = NULL){
        this->holder= holder;
        this->fname = strdup(fname);
    }

    /** */
    ~DirectoryEntry(){
#if (defined (_VXWORKS)||defined(_RTAI)|| defined(_LINUX) || defined(_SOLARIS) || defined(_MACOSX))
        if (fname != NULL) free((void *&)fname);
        fname = NULL;
#endif
    }

    /** file name */
    const char *Name(){
#if (defined (_VXWORKS)||defined(_RTAI)|| defined(_LINUX) || defined(_SOLARIS) || defined(_MACOSX))
        return fname;
#elif defined(_WIN32)
        return fileData.cFileName;
#else
        return info.name;
#endif
    }

    /** is it a directory */
    bool IsDirectory(){
#if (defined (_VXWORKS)||defined(_RTAI)|| defined(_LINUX) || defined(_SOLARIS) || defined(_MACOSX))
        return S_ISDIR(fileStatistics.st_mode);
#elif defined(_WIN32)
        return ((fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!=0);
#else
        return ((info.attrib & _A_SUBDIR)!=0);
#endif
    }

    /** is it a file */
    bool IsFile(){
#if (defined (_VXWORKS)||defined(_RTAI)|| defined(_LINUX) || defined(_SOLARIS) || defined(_MACOSX))
        return S_ISREG(fileStatistics.st_mode);
#else
        return IsDirectory() == False;
#endif
    }

    /** is it read only */
    bool ReadOnly(){
#if (defined (_VXWORKS)||defined(_RTAI)|| defined(_LINUX) || defined(_SOLARIS) || defined(_MACOSX))
/// NOT IMPLEMENTED
        return False;
#elif defined(_WIN32)
        return ((fileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)!=0);
#else
        return ((info.attrib & _A_RDONLY)!=0);
#endif
    }

    /** the file size */
    int64 Size(){
#if (defined (_VXWORKS)||defined(_RTAI)|| defined(_LINUX) || defined(_SOLARIS) || defined(_MACOSX))
        return  fileStatistics.st_size;
#elif defined(_WIN32)
        uint64 sz = fileData.nFileSizeHigh;
        sz <<= 32;
        sz += fileData.nFileSizeLow;
        return sz;
#else
        return info.size;
#endif
    }

    /** last write time */
    time_t Time(){
#if (defined (_VXWORKS)||defined(_RTAI)|| defined(_LINUX) || defined(_SOLARIS) || defined(_MACOSX))
        return fileStatistics.st_mtime;
#elif defined(_WIN32)
        uint64 t = *((uint64 *)&fileData.ftLastWriteTime);
        uint64 t2;
        uint64 div = 10000000;
        t2 = t / div;
        t2 -= 0x2B61082f0;
        uint32 *p2 = (uint32 *)&t2;
        return (time_t)p2[0];
#else
        return info.time_write;
#endif
    }

    /** last access time */
    time_t LastAccessTime(){
#if (defined (_VXWORKS)||defined(_RTAI)|| defined(_LINUX) || defined(_SOLARIS) || defined(_MACOSX))
        return fileStatistics.st_atime;
#elif defined(_WIN32)
        uint64 t = *((uint64 *)&fileData.ftLastAccessTime);
        uint64 t2;
        uint64 div = 10000000;
        t2 = t / div;
        t2 -= 0x2B61082f0;
        uint32 *p2 = (uint32 *)&t2;
        return (time_t)p2[0];
#else
        if (info.time_access != -1) return info.time_access;
        return Time();
#endif
    }


};

#if (defined(_VXWORKS)|| defined(_LINUX) || defined(_SOLARIS) || defined(_MACOSX))
    #define DIRECTORY_SEPARATOR '/'
#else
    #define DIRECTORY_SEPARATOR '\\'
#endif

/** contains information about a directory and all its content */
class Directory: public LinkedListHolder {
    friend class DirectoryEntry;
    /** which directory */
    char *baseAddress;
    /** total content size */
    uint64 size;
public:
    /** constructor */
    Directory(const char *address,const char *fileMask="*",SortFilterFn *sorter=NULL){
        size = 0;
        baseAddress = NULL;

        if ((address != NULL)&&(strlen(address)>0)){
            baseAddress = (char *)malloc(strlen(address)+2);
            strcpy(baseAddress,address);
        }

        if (baseAddress == NULL){
            baseAddress = (char *)malloc(2);
            baseAddress[0]   = DIRECTORY_SEPARATOR;
            baseAddress[1] = 0;
        } else {
            uint32 l = strlen(baseAddress);
            if ((address[l-1] != '\\') || (address[l-1] != '/')){
                baseAddress[l]   = DIRECTORY_SEPARATOR;
                baseAddress[l+1] = 0;
            }
        }

#ifdef _RTAI            //Have to implement filter based on LINUX

        char statAddr[512];
        int d = opendir((char *)address);
        if (d > 0){
            struct dirent *de = (struct dirent *)malloc(sizeof(struct dirent));
            while(readdir(d, de) > 0){
                DirectoryEntry *entry;
                if (sorter==NULL)ListInsert((entry = new DirectoryEntry(this)));
                else             ListInsert((entry = new DirectoryEntry(this)),sorter);
                entry->fname = strdup(de->d_name);
                memset(statAddr, 0, 512);
                strcat(statAddr, address);
                if(address[strlen(address) - 1] != DIRECTORY_SEPARATOR)
                    strcat(statAddr, "/");

                strcat(statAddr, entry->fname);
                stat(statAddr,&entry->fileStatistics);
                size += entry->Size();
            }
            free((void *&)de);
            closedir(d);
        }
        free((void *&)baseAddress);
        baseAddress = NULL;
#elif (defined(_LINUX) || defined(_MACOSX))
        if(fileMask == NULL){
            fileMask = "*";
        }

        fileFilterSearchMask = (char *)malloc(strlen(fileMask) + 1);
        strcpy(fileFilterSearchMask, fileMask);

        char statAddr[512];
        struct dirent **namelist;
        int n = scandir(baseAddress, &namelist, fileFilter, alphasort);

        if(n > 0){
            while (n--){
                DirectoryEntry *entry;
                if (sorter==NULL)ListInsert((entry = new DirectoryEntry(this)));
                else             ListInsert((entry = new DirectoryEntry(this)),sorter);
                entry->fname = strdup(namelist[n]->d_name);
                memset(statAddr, 0, 512);
                strcat(statAddr, address);
                if(address[strlen(address) - 1] != DIRECTORY_SEPARATOR)
                    strcat(statAddr, "/");

                strcat(statAddr, entry->fname);
                stat(statAddr,&entry->fileStatistics);
                size += entry->Size();
            }
        }
        free((void *&)fileFilterSearchMask);
        free((void *&)baseAddress);
        baseAddress = NULL;
#elif defined(_SOLARIS)
        uint32 bal=0;
        if (baseAddress == NULL){
            baseAddress = (char *)malloc(2);
            baseAddress[0]   = '\\';
            baseAddress[1] = 0;
            bal = 1;
        } else {
            bal = strlen(baseAddress);
            if ((address[bal-1] != '\\') || (address[bal-1] != '/')){
                baseAddress[bal]   = '\\';
                baseAddress[bal+1] = 0;
            }
        }
        char statAddr[512];
        DIR *d = opendir((char *)address);
        if (d != NULL){
            struct dirent *de;
            while((de = readdir(d)) != NULL){
                DirectoryEntry *entry;
                ListInsert((entry = new DirectoryEntry(this)));
                entry->fname = strdup(de->d_name);
                memset(statAddr, 0, 512);
                strcat(statAddr, address);
                if(address[strlen(address) - 1] != '/')
                    strcat(statAddr, "/");

                strcat(statAddr, entry->fname);
                stat(statAddr,&entry->fileStatistics);
                size += entry->Size();
            }            
            closedir(d);
        }
        free((void *&)baseAddress);
        baseAddress = NULL;
#elif defined(_VXWORKS)
        char statAddr[512];
        DIR *d = opendir((char *)address);
        if (d != NULL){
            struct dirent *de;
            while((de = readdir(d)) != NULL){
                DirectoryEntry *entry;
                ListInsert((entry = new DirectoryEntry(this)));
                entry->fname = strdup(de->d_name);
                memset(statAddr, 0, 512);
                strcat(statAddr, address);
                if(address[strlen(address) - 1] != DIRECTORY_SEPARATOR)
                    strcat(statAddr, "/");

                strcat(statAddr, entry->fname);
                stat(statAddr,&entry->fileStatistics);
                size += entry->Size();
            }            
            closedir(d);
        }
        free((void *&)baseAddress);
        baseAddress = NULL;
#elif defined(_WIN32)
        char *searchMask = NULL;
        if (fileMask != NULL){
            searchMask = (char *)malloc(strlen(baseAddress)+strlen(fileMask)+2);
            strcpy(searchMask,baseAddress);
            strcat(searchMask,fileMask);
        } else searchMask = strdup(baseAddress);

        WIN32_FIND_DATA lpFindFileData;
        HANDLE h = FindFirstFile(searchMask,&lpFindFileData);
        if (h == INVALID_HANDLE_VALUE){
            free((void *&)searchMask);
            free((void *&)baseAddress);
            return;
        }

        DirectoryEntry *entry;
        if (sorter==NULL)ListInsert((entry = new DirectoryEntry(this)));
        else             ListInsert((entry = new DirectoryEntry(this)),sorter);
        entry->fileData = lpFindFileData;
        size += entry->Size();

        while (FindNextFile(h,&lpFindFileData)){
            if (sorter==NULL)ListInsert((entry = new DirectoryEntry(this)));
            else             ListInsert((entry = new DirectoryEntry(this)),sorter);

            entry->fileData = lpFindFileData;
            size += entry->Size();
        }

        free((void *&)searchMask);
        free((void *&)baseAddress);
        baseAddress = NULL;
        FindClose(h);
#else
        uint32 bal=0;
        if (baseAddress == NULL){
            baseAddress = (char *)malloc(2);
            baseAddress[0]   = '\\';
            baseAddress[1] = 0;
            bal = 1;
        } else {
            bal = strlen(baseAddress);
            if ((address[l-1] != '\\') || (address[l-1] != '/')){
                baseAddress[l]   = '\\';
                baseAddress[l+1] = 0;
            }
        }

        char *searchMask = NULL;
        if (fileMask != NULL){
            searchMask = (char *)malloc(strlen(baseAddress)+strlen(fileMask)+2);
            strcpy(searchMask,baseAddress);
            strcat(searchMask,fileMask);
        } else searchMask = strdup(baseAddress);

        DirectoryEntry *entry = new DirectoryEntry(this);
        long handle;
        handle = _findfirst(searchMask,&entry->info);
        if (handle == -1){
            delete entry;
            free((void *&)searchMask);
            free((void *&)baseAddress);
            return;
        }
        int ret = 0;
        while(ret!=-1){
            if (sorter==NULL)ListInsert((entry = new DirectoryEntry(this)));
            else             ListInsert((entry = new DirectoryEntry(this)),sorter);
            size += entry->Size();

            entry = new DirectoryEntry(this);
            ret = _findnext( handle, &entry->info );
        }
        delete entry;
        free((void *&)searchMask);
        free((void *&)baseAddress);
#endif
    }

    /** destructor */
    ~Directory(){
        if (baseAddress!=NULL) free((void *&)baseAddress);
    }

    /** Fills the data in the object*/
    static bool Create(const char *address){
#if defined(_WIN32)
        return (CreateDirectory(address,NULL)==TRUE);
#elif (defined(_LINUX) || defined(_MACOSX))
        return (mkdir(address, 0777) == 0);
#else
// NOT IMPLEMENTED
        return False;
#endif
    };

    /** check for directory existance */
    static bool DirectoryExists(const char *address){
#if defined(_WIN32)
        ULONG attrs = GetFileAttributes(address);
        if (attrs == 0xFFFFFFFF) return False;
        return (attrs & FILE_ATTRIBUTE_DIRECTORY);
#elif (defined(_LINUX) || defined(_MACOSX))
        struct stat fileStats;
        stat(address, &fileStats);
        return S_ISDIR(fileStats.st_mode);
#else
// NOT IMPLEMENTED
        return False;
#endif
    };

    /** total size of files in the directory. not recursive */
    uint64 TotalFileSize(){
        return size;
    }

};

#endif

