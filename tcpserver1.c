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

#define PORT_NO 10001

/*関数の準備*/

void StoL(char str[]){
  int i=0;
  for(i=0;i<=strlen(str);i++){
    if(str[i]>=97 && str[i]<=122) str[i]=str[i]-32;
  }
}

int main(){

  /*ソケットを作成する*/

  int s;
  s = socket(AF_INET, SOCK_STREAM, 0);
  if(s<0){
    printf("Error : can't make socket\n");
    return(-1);
  }

  /*ソケットに名前をつける*/

  struct sockaddr_in sa;
  sa.sin_family = AF_INET;
  sa.sin_port = htons(PORT_NO);
  sa.sin_addr.s_addr = INADDR_ANY;

  int name;
  name = bind(s, (struct sockaddr *) &sa, sizeof(sa));
  if(name < 0){
    printf("Error : bind\n");
    return(-1);
  }

  /*接続要求を待つ*/

  int wait;
  wait = listen(s,5);
  if(wait < 0){
    printf("Error : listen\n");
    return(-1);
  }

  /*接続要求を受け付ける*/

  struct sockaddr_in client;
  
  int new_s;
  socklen_t len = sizeof(client);
  new_s = accept(s, (struct sockaddr *)&client, &len);
  if(new_s < 0){
    printf("Error : accept\n");
    return(-1);
  }

  /*メッセージを受信する*/

  char buf[9999];
  recv(new_s, buf, sizeof(buf), 0);

  printf("%s\n", buf);

  /*小文字から大文字へ変換*/

  StoL(buf);

  /*メッセージの送信*/

  send(new_s, buf, sizeof(buf), 0);

  /*ソケットの削除*/

  close(new_s);

  return 0;

}


