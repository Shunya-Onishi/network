// 標準入力から読み込んだ文字列を，ファイルに書き出すプログラムの作成
#include <stdio.h> //printf
#include <string.h> //strlen

#include <errno.h> //error

#include <sys/types.h>  //open
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h> //read, write, close

int main(void){

  int fd = 0;
  char buf[BUFSIZ] = {"¥0"};
  ssize_t rnum = 0;
  ssize_t wnum = 0;

  fd = open("test.txt", 
	    O_CREAT|O_WRONLY|O_RDWR,
	    S_IRWXU|S_IRUSR|S_IWUSR|S_IXUSR);
  if(fd < 0){
    printf("Error: open(%d) %s\n", errno, strerror(errno));
    return -1;
  }
  
  rnum = read(0, buf, sizeof(buf)); //標準入力0からbufに読み込み
  if(rnum < 0){
    printf("Error: open(%d) %s\n", errno, strerror(errno));
    return -1;
  }

  wnum = write(fd,buf,rnum); //bufからfdの指す場所へrnumバイト書き込み
  if(wnum < 0){
    printf("Error: open(%d) %s\n", errno, strerror(errno));
    return(-1);
  }

  if(rnum != wnum){
    printf("Error: open(%d) %s\n", errno, strerror(errno));
    return(-1);
  }

  if(close(fd) < 0){
    printf("Error: open(%d) %s\n", errno, strerror(errno));
    return(-1);
  }

  return 0;

}
