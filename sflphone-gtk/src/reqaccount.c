/*
 *  Copyright (C) 2009 Jean Schurger <jean@schurger.org>
 *  and Savoir-Faire Linux inc.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*
 *  To test:
 *
 *  cc -o reqhttp reqhttp.c -DBUILD_EXAMPLE
 * 
 */

#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <strings.h> /* bzero */
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

#include "reqaccount.h"

int req(char *host, int port, char *req, char *ret) {

  int s;
  struct sockaddr_in servSockAddr;
  struct hostent *servHostEnt;
  long hostAddr;
  long status;
  int length, i;
  FILE *f;
  char buf[1024];
  
  bzero(&servSockAddr, sizeof(servSockAddr));
  hostAddr = inet_addr(host);

  if ((long)hostAddr != (long)-1)
    bcopy(&hostAddr, &servSockAddr.sin_addr, sizeof(hostAddr));
  else {
    servHostEnt = gethostbyname(host);
    if (servHostEnt == NULL) {
      strcpy(ret, "gethostbyname");
      return -1;
    }
    bcopy(servHostEnt->h_addr, &servSockAddr.sin_addr, servHostEnt->h_length);
  }
  servSockAddr.sin_port = htons(port);
  servSockAddr.sin_family = AF_INET;
  
  if ((s = socket(AF_INET,SOCK_STREAM,0)) < 0) {
    strcpy(ret, "socket");
    return -1;
  }
  
  if(connect(s, (const struct sockaddr *) &servSockAddr,
	     (socklen_t) sizeof(servSockAddr)) < 0 ) {
    strcpy(ret, "connect");
    return -1;
  }
  
  f = fdopen(s, "r+");

  fprintf(f, "%s HTTP/1.1\r\n", req);
  fprintf(f, "Host: %s\r\n", host);
  fputs("User-Agent: SFLphone\r\n", f);
  fputs("\r\n", f);

  while (strncmp(fgets(buf, sizeof(buf), f), "\r\n", 2)) {
    const char *len_h = "content-length";
    const char *status_h = "HTTP/1.1";
    if (strncasecmp(buf, len_h, strlen(len_h)) == 0)
      length = atoi(buf + strlen(len_h) + 1);
    if (strncasecmp(buf, status_h, strlen(status_h)) == 0)
      status = atoi(buf + strlen(status_h) + 1);
  }
  for (i = 0; i < length; i++)
    ret[i] = fgetc(f);

  if (status != 200) {
    sprintf(ret, "http error: %ld", status);
    return -1;
  }

  fclose(f);
  shutdown(s, 2);
  close(s);
  return 0;
}

rest_account get_rest_account(char *host) {
  char ret[4096];
  rest_account ra;
  bzero(ret, sizeof(ret));
  if (req(host, 80, "GET /rest/accountcreator", ret) != -1) {
    strcpy(ra.user, strtok(ret, "\n"));
    strcpy(ra.passwd, strtok(NULL, "\n"));\
    ra.success = 1;
  } else {
    ra.success = 0;
    strcpy(ra.reason, ret);
  }
  return ra;
}


#ifdef BUILD_EXAMPLE

int main (void) {
  rest_account acc = get_rest_account("sip.sflphone.org");
  if (acc.success) {
    puts(acc.user);
    puts(acc.passwd);
  } else {
    printf("FAILED: %s\n", acc.reason);
  }
}
#endif