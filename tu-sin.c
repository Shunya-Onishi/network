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

  
  struct sockaddr_in server;
  int sock; 
  char buf[32];
  char *deststr;
  unsigned int **addrptr;

  /* gethostbyname */
  if (argc != 2) {
    printf("Usage : %s dest\n" argv[0]);
    return 1;
  }
  deststr = argv[1];

  /* socket ソケットの作成*/
  
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if(sock < 0) {
    perror("socket");
    return 1;
  }

  /* 接続先指定用の構造体の準備 */
  server.sin_family = AF_INET;
  server.sin_port = htons(12345);

  server.sin_addr.s_addr = inet_addr(deststr);
  if(server.sin_addr.s_addr == 0xffffffff) {
    struct hostent *host;
    
    host = gethostbyname(deststr);
    if(host == NULL) {
      if (h_errno == HOST_NOT_FOUND) {
	/* h_errorはexternで宣言されています */
	printf("host not found : %s\n", deststr);
      } else {
	printf("%s : %s\n", hstrerror(h_errno), deststr);
      }
      return 1;
    }

    addrptr = (unsigned int **)host -> h_addr_list;

  /* connect　サーバに接続 */
  connect(sock, (struct sockaddr*)&server, sizeof(server));
  
  /* サーバからデータを受信 */
  menset(buf, 0, sizeof(buf));
  n = read(sock, buf, sizeof(buf));

  /*  */
  send(int s, const void *buf, size_t len, int flags);
  /*  */
  recv(int s, void *buf, size_t len, int flags);

  /* socketの終了 */
  close(sock);

  return 0;


}
