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
 
  // int no;
  unsigned short port = 5002; //port number
  int ssoc; //server socket
  int csoc; //client socket

  struct sockaddr_in srvAddr;
  struct sockaddr_in cliAddr;

  socklen_t cliAddrSize = sizeof(cliAddr);
  char buf[MAX_LEN];
  int rc;

  if(argc > 2){
    printf("Usage: server [port_no]\n");
    return -1;
  }
  
  //  port = (argc == 1) ? 5000 : atoi(argv[1]);

  memset(&srvAddr, 0, sizeof(srvAddr));
  srvAddr.sin_port = htons(port);
  srvAddr.sin_family = AF_INET;
  srvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  
  /*creat socket ソケットを作成する*/
  ssoc = socket(AF_INET, SOCK_STREAM, 0);
  if(ssoc == -1){
    fprintf(stderr, "Can not create socket \n");
    return -1;
  }

  /*bind socket ソケットに名前をつける*/
  if(bind(ssoc, (struct sockaddr*)&srvAddr, sizeof(srvAddr)) < 0){
    printf("Can not bind\n");
    return -1;
  };

  /*listen socket 接続要求を待つ*/
  listen(ssoc, 1);

  /*Accept 接続要求を受け付ける*/
  printf("Waiting for connection ...\n");
  if((csoc = 
      accept(ssoc, (struct sockaddr*)&cliAddr, &cliAddrSize)) < 0) {
    perror("accept");
    return 1;
  } 
  printf("Connected from %s\n", inet_ntoa(cliAddr.sin_addr));
  
  /*Receive data メッセージを受信する*/
    rc = recv(csoc, buf, sizeof(buf), 0);
    if(rc == 0){
      printf("client process end.\n");
    }
    
    if(rc == -1){
      perror("recv");
    }
    

  /*Send data メッセージを送信する*/
  send(csoc, buf, strlen(buf), 0);

  close(csoc);
  return 0;

}
