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

#define PORT_NO 10001

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

  int s;
  s = socket(AF_INET, SOCK_STREAM, 0);
  if(s < 0){
    printf("Error : can't make socket\n");
    return(-1);
  }

  /*コネクションを確立する*/

  struct sockaddr_in sa;
  sa.sin_family = hostname -> h_addrtype;
  sa.sin_port = htons(PORT_NO);
  bzero((char*)&sa.sin_addr, sizeof(sa.sin_addr));
  memcpy((char*)&sa.sin_addr, (char*)hostname -> h_addr, hostname -> h_length);

  if(connect(s, (struct sockaddr *) &sa, sizeof(sa)) < 0){
    printf("Error : can't connect\n");
    return(-1);
  }

  /*メッセージを送信する*/

  char str[] = "GET get GET get \r\n";
  send(s, str, sizeof(str), 0);

  /*メッセージを受信する*/

  char buf[9999];
  recv(s, buf, sizeof(buf), 0);

  printf("%s\n", buf);

  /*ソケットを削除する*/

  close(s);

  return 0;

}
