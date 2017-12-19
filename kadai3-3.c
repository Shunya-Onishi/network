// 標準入力から読み込んだ文字列を標準出力に出力するプログラムの作成
#include <stdio.h> //printf
#include <string.h> //strlen

#include <errno.h> //error

#include <sys/types.h>  //open
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h> //read, write, close

int main(void){

  ssize_t rnum = 0;
  ssize_t wnum = 0;
  char buf[BUFSIZ] = {"¥0"};
  /*
  int fd = 0;
  char buf[10];
  ssize_t num = 0;
  ssize_t cnt = strlen(buf);
  */

  /*
  fd = open("data",O_RDWR|O_CREAT|O_APPEND, 0);
  if(fd < 0){
    printf("Error: open(%d) %s\n", errno, strerror(errno));
    return -1;
  }
  */

  rnum = read(0, buf, sizeof(buf));
  if(rnum < 0){
    printf("Error: open(%d) %s\n", errno, strerror(errno));
    return -1;
  }
  wnum = write(1,buf,rnum);
  if(wnum < 0){
    printf("Error: open(%d) %s\n", errno, strerror(errno));
    return(-1);
  }

  if(rnum != wnum){
    printf("Error: open(%d) %s\n", errno, strerror(errno));
    return(-1);
  }

  /*
  if(close(fd) < 0){
    printf("Error: open(%d) %s\n", errno, strerror(errno));
    return(-1);
  }
  */

  return 0;

}
