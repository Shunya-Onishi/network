// 標準入力から読み込んだ文字列を，ファイルに書き出すプログラムの作成
#include <stdio.h> //printf
#include <string.h> //strlen

#include <errno.h> //error

#include <sys/types.h>  //open
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h> //read, write, close

int main(int args, char *argv[]){

  int fd = 0;
  char buf[BUFSIZ] = {"¥0"};
  ssize_t rnum = 0;
  ssize_t wnum = 0;

  if(args != 2){
    printf("Error");
    return -1;
  }

  fd = open(argv[1], 
	    O_CREAT|O_WRONLY|O_TRUNC,
	    S_IRWXU/*|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH*/);
  if(fd < 0){
    printf("Error: open(%d) %s\n", errno, strerror(errno));
    return -1;
  }
  
  rnum = read(0, buf, sizeof(buf)); //標準入力0からbufに読み込み
  if(rnum < 0){
    printf("Error: read(%d) %s\n", errno, strerror(errno));
    return -1;
  }

  wnum = write(fd,buf,rnum); //bufからfdの指す場所へrnumバイト書き込み
  if(wnum < 0){
    printf("Error: write(%d) %s\n", errno, strerror(errno));
    return(-1);
  }

  if(rnum != wnum){
    printf("Error: readwrite(%d) %s\n", errno, strerror(errno));
    return(-1);
  }

  if(close(fd) < 0){
    printf("Error: close(%d) %s\n", errno, strerror(errno));
    return(-1);
  }

  return 0;

}
