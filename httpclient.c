#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h> //send
#include <sys/stat.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>  
#include <arpa/inet.h>
#include <netdb.h> //gethostbyname

#define MAX_LEN 2048

int main(int argc, char *argv[]){

  struct hostent* host;
  struct sockaddr_in sa;
  int soc;
  char text[] = "GET /index.html\r\n";
  char buf [MAX_LEN];
  int rec;
  int fd;
  int rc;
  ssize_t wnum;

  if(argc != 3){
    printf("Can not get host name\n");
    return -1;
  }

  /*gethost*/
  host = gethostbyname(argv[1]);
  if(host == NULL){
    fprintf(stderr, "Can not get address %s\n", argv[1]);
    return -1;
  }

  /*create socket*/
  soc = socket(AF_INET, SOCK_STREAM, 0);
  if(soc == -1){
    fprintf(stderr, "Can not create socket %s\n");
    return -1;
  }


  /*connect*/

  sa.sin_family = host -> h_addrtype;
  sa.sin_port = htons((unsigned short)80);
  bzero((char*)&sa.sin_addr, sizeof(sa.sin_addr));
  memcpy((char*)&sa.sin_addr,(char*)host -> h_addr, host -> h_length);


  rc = connect(soc, (struct sockaddr*)&sa, sizeof(sa));
  if(rc == -1){
    fprintf(stderr, "Can not connect \n");
    return -1;
  }
  
  /*send*/
  send(soc, text, sizeof(text), 0);

  /*recv*/
  rec = recv(soc, buf, sizeof(buf), 0);

  fd = open(argv[2], O_WRONLY|O_CREAT, S_IRWXU);
  if(fd < 0){
    printf("Error : open\n");
    return -1;
  }
  
  printf("%d\n", rec);
  wnum = write(1, buf, rec);
  if(wnum <0){
    printf("Error write\n");
    return -1;
  }

  /*close*/
  close(soc);
  return 0;

}
