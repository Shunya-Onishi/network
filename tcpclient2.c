#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h> //close
#include <fcntl.h>

#define PORT_NO 10016

int main(int argc, char *argv[]){

  /*通信相手のIPアドレスの取得*/

  struct hostent* hostname;
  if(argv < 0){
    printf("Error : arguments number\n");
  }

  hostname = gethostbyname(argv[1]);
  if(hostname == NULL){
    printf("Error : hostname is NULL\n");
  }

  /*ソケットを作成する*/

  int sockfd;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0){
    printf("Error : can't make socket\n");
    return(-1);
  }

  /*コネクションを確立する*/

  struct sockaddr_in client_addr;

  // memset((char*)&client_addr.sin_addr, 0, sizeof(client_addr.sin_addr));

  memset((char*)&client_addr, 0, sizeof(client_addr));

  client_addr.sin_family = hostname -> h_addrtype;
  memcpy((char*)&client_addr.sin_addr, (char*)hostname -> h_addr, hostname -> h_length);
  client_addr.sin_port = htons(PORT_NO);

  if(connect(sockfd, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0){
    printf("Error : can't connect\n");
    return(-1);
  }

  /*メッセージを送信する*/

  char str[] = "akutagawa kouhei\r\n";
  if(send(sockfd, str, sizeof(str), 0) < 0){
    printf("Error : can't send\n");
    return(-1);
  };

  /*メッセージを受信する*/

  char ItoC[100];
  if(recv(sockfd, ItoC, sizeof(ItoC), 0) < 0){
    printf("Error : can't recv\n");
    return(-1);
  };

  printf("%s\n", ItoC);

  /*ソケットを削除する*/

  if(close(sockfd) < 0){
    printf("Error : can't close\n");
    return(-1);
  };

  return 0;

}
