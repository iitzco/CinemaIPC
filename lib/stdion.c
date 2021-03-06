#include "stdion.h"
#include <errno.h>

int readn(int fd, void *vptr, int n)
{
      int  nleft;
      int nread;
      char   *ptr;

      ptr = vptr;
      nleft = n;
     while (nleft > 0) {
       if ( (nread = read(fd, ptr, nleft)) < 0) {
           if (errno == EINTR)
               nread = 0;      /* and call read() again */
           else
               return (-1);
     } else if (nread == 0)
       break;              /* EOF */
      nleft -= nread;
       ptr += nread;
     }
     return (n - nleft);         /* return >= 0 */
}

int writen(int fd, const void *vptr, int n)
{
    int nleft;
    int nwritten;
    const char *ptr;

    ptr = vptr;
    nleft = n;
    while (nleft > 0) {
         if ( (nwritten = write(fd, ptr, nleft)) <= 0) {
           if (nwritten < 0 && errno == EINTR)
               nwritten = 0;   /* and call write() again */
         else
             return (-1);    /* error */
        }
        nleft -= nwritten;
      ptr += nwritten;
     }
     return (n);
}