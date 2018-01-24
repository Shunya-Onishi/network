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
#include <errno.h>

#define PORT_NO 10016
#define MAX_LINE_LEN 1024

void parse_line(char *line, int new_s);

int main(){

  /*ソケットを作成する*/

  int sockfd;
  int yes = 1;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0){
    printf("Error : can't make socket\n");
    return(-1);
  }

  /* SO_REUSEADDR をつける*/

  int ret;
  ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,(const char *)&yes, sizeof(yes));
  if(ret < 0){
    printf("Error : can't opt");
    return 0;
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
    perror("bind");
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
  int new_s;
    while(1){

  struct sockaddr_in client;
  
  socklen_t len = sizeof(client);
  new_s = accept(sockfd, (struct sockaddr *)&client, &len);
  if(new_s < 0){
    printf("Error : accept\n");
    return(-1);
  }

  /*メッセージを受信する*/

  while(1){
    char buf[MAX_LINE_LEN + 1];
    printf("クライアントの入力待ち \n");
    recv(new_s, buf, sizeof(buf), 0);
    printf("入力 %s\n",buf);
    if(buf[0]=='%' && buf[1]=='Q'){
    parse_line(buf, new_s);
    printf("処理終了\n\n");
    break;
    }else{
      printf("サーバの処理開始 \n");

      parse_line(buf, new_s);

      printf("入力 after parse_line(): %s\n", buf);
      bzero(&buf, sizeof(buf));
      printf("処理終了\n\n");
    }
  }


    /* /\*メッセージの送信*\/ */
    /* memset(buf,0,sizeof(s)); */
    /* send(new_s, buf, sizeof(buf), 0); */

  }

  /*ソケットの削除*/

  close(new_s);

  //  } dekai while

  return 0;

}
