#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <strings.h>
#include <unistd.h> //close
#include <fcntl.h>

#define PORT_NO 10016

int main(){

  /*ソケットを作成する*/

  int sockfd;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0){
    printf("Error : can't make socket\n");
    return(-1);
  }

  /*ソケットに名前をつける*/

  struct sockaddr_in reader_addr;

  memset((char*)&reader_addr, 0, sizeof(reader_addr));

  reader_addr.sin_family = AF_INET; /*インターネットドメイン*/
  reader_addr.sin_addr.s_addr = htonl(INADDR_ANY); /*任意のIPアドレスを受付*/
  reader_addr.sin_port = htons(PORT_NO); /*接続待ちのポート番号を設定*/

  int name;
  name = bind(sockfd, (struct sockaddr *)&reader_addr, sizeof(reader_addr));
  if(name < 0){
    printf("Error : bind\n");
    return(-1);
  }

  /*接続要求を待つ*/

  int wait;
  wait = listen(sockfd,5);
  if(wait < 0){
    printf("Error : listen\n");
    close(sockfd);
    return(-1);
  }

  /*接続要求を受け付ける*/

  struct sockaddr_in client;
  
  int new_s;
  socklen_t len = sizeof(client);
  new_s = accept(sockfd, (struct sockaddr *)&client, &len);
  if(new_s < 0){
    printf("Error : accept\n");
    return(-1);
  }

  /*メッセージを受信する*/

  char buf[9999];
  recv(new_s, buf, sizeof(buf), 0);

  printf("%s\n", buf);

  /*メッセージの長さを計算する*/

  int charlen;
  char ItoC[100];
  charlen = strlen(buf)-2;
  sprintf(ItoC, "%d", charlen);

  /*メッセージの送信*/

  send(new_s, ItoC, sizeof(ItoC), 0);

  /*ソケットの削除*/

  close(new_s);

  return 0;

}
