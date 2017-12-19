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

  rnum = read(0, buf, sizeof(buf));
  if(rnum < 0){
    printf("Error: read(%d) %s\n", errno, strerror(errno));
    return -1;
  }
  wnum = write(1,buf,rnum);
  if(wnum < 0){
    printf("Error: write(%d) %s\n", errno, strerror(errno));
    return(-1);
  }

  if(rnum != wnum){
    printf("Error: readwrite(%d) %s\n", errno, strerror(errno));
    return(-1);
  }


  return 0;

}
