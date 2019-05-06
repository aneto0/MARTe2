/**
 * @file udpReceiver.cpp
 * @brief Source file for class udpReceiver
 * @date 19 apr 2019
 * @author pc
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

 * @details This source file contains the definition of all the methods for
 * the class udpReceiver (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

/*
 * CfgUploaderDev.cpp
 *
 *  Created on: Jan 19, 2018
 *      Author: gferro
 */

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>

#define PORT 7
#define ADDR "192.168.1.10"

int main() {

    struct sockaddr_in other;
    int udpFile = socket(AF_INET, SOCK_DGRAM, 0);

    if (udpFile < 0) {
        printf("error socket\n");
        return 0;
    }

    other.sin_family = AF_INET;
    other.sin_port = htons(PORT);
    other.sin_addr.s_addr = inet_addr(ADDR);
    if (connect(udpFile, (struct sockaddr*) &other, sizeof(other)) == -1) {

        printf("error connect\n");
    }

    char buf[1024];
    int dc[]={0,0,0,0,0,0,0,0,0,0,0,0,512};
	//int dc;
    while(1){
        memset(buf, 0, 1024);
	printf("Enter duty cycle: ");
	scanf("%d", &dc[12]);
	printf("\n");
        sendto(udpFile, &dc, sizeof(int)*13, 0,  (struct sockaddr *)&other, sizeof(other));
        //recv(udpFile, buf, sizeof buf, 0);
        printf("send %d\n", dc[12]);
    }
    return 0;
}
