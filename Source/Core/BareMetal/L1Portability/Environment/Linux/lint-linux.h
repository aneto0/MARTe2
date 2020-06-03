/**
 * @file lint-linux.h
 * @brief Header file for class lint-linux
 * @date 05/07/2015
 * @author André Neto
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This header file lists all the operating system function calls
 * and aims at supporting the linting process. Unfortunately lint
 * struggles to parse some of the operating system headers (in particular string.h)
 * and generates a large amount of false positives when these are used by the Linux
 * implementation.
 */

#ifndef LINUX_LINT_LINUX_H_
#define LINUX_LINT_LINUX_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>
#include <pthread.h>
#include "../../GeneralDefinitions.h"

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

extern void *malloc(size_t size);

extern void free(void* ptr);

extern void *realloc(void* ptr, size_t size);

extern char *strdup(const char *s);

extern void *memset(void *s, int c, size_t n);

extern void *memcpy(void *dest, const void *src, size_t n);

extern int memcmp(const void *ptr1, const void *ptr2, size_t n);

extern const void * memchr ( const void * ptr, int value, size_t num );

extern void * memmove ( void * destination, const void * source, size_t num );

extern void * memset ( void * ptr, int value, size_t num );

extern void free();

extern char *strcpy(char *dest, const char *src);

extern char *strncpy(char *dest, const char *src, size_t n);

extern char *strcat(char *dest, const char *src);

extern char *strncat(char *dest, const char *src, size_t n);

extern char *strchr(const char *s, int c);

extern char *strrchr(const char *s, int c);

extern int strcmp(const char *s1, const char *s2);

extern int strncmp(const char *s1, const char *s2, size_t n);

extern int strncasecmp(const char *s1, const char *s2, size_t n);

extern size_t strlen(const char *s);

extern size_t strspn(const char *s, const char *accept);

extern size_t strcspn(const char *s, const char *reject);

extern char *strstr(const char *haystack, const char *needle);

extern char *strcasestr(const char *haystack, const char *needle);

extern char *strpbrk(const char *s, const char *accept);

extern double atof(const char *nptr);

extern float strtof(const char *nptr, char **endptr);

extern size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);

#endif /* LINUX_LINT_LINUX_H_ */
