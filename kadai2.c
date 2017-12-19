// ファイルから読み込んだ内容を，標準出力に出力するプログラムの作成
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
	    /*O_CREAT|*/O_RDONLY/*|O_TRUNC*/,
	    S_IRWXU/*|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH*/);
  if(fd < 0){
    printf("Error: open(%d) %s\n", errno, strerror(errno));
    return -1;
  }
  
  rnum = read(fd, buf, sizeof(buf));
  if(rnum < 0){
    printf("Error: read(%d) %s\n", errno, strerror(errno));
    return -1;
  }

  wnum = write(1,buf,rnum);
  if(wnum < 0){
    printf("Error: write(%d) %s\n", errno, strerror(errno));
    return(-1);
  }

  if(close(fd) < 0){
    printf("Error: close(%d) %s\n", errno, strerror(errno));
    return(-1);
  }

  return 0;

}
