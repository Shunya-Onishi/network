/*[1]*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>


#define MAX_LINE_LEN 1024
#define MAXSTR 69
#define MAXPRO 10000
#define MAX_ID_LEN 31
#define MAX_BIRTH_LEN 10


int back = 0;
int ditems;
int mark = 0;
int flag =0;

/*[2]*/
struct date {
  int y;
  int m;
  int d;
};

/*[3]*/
struct profile {
  int id;
  char name[MAXSTR+1];
  struct date birthday;
  char home[MAXSTR+1];
  char *comment;
};

/*[4]*/
struct profile profile_data_store[MAXPRO];
int profile_data_nitems = 0; 

void parse_line(char *line, int new_s);

/*[5]*/
int subst(char *str, char c1, char c2)
{
  int n = 0;

  while (*str) {
    if (*str == c1) {
      *str = c2;
      n++;
    }
    str++;
  }
  return n;
}

/*[6]*/
int split(char *str, char *ret[], char sep, int max)
{
  int cnt = 0;

  ret[cnt++] = str;

  while (*str && cnt < max) {
    if (*str == sep){
      *str = '\0';
      ret[cnt++] = str + 1;
    }
    str++;
  }
 return cnt;
}

/*[7]*/
int get_line(FILE *fp,char *line)
{
  if (fgets(line, MAX_LINE_LEN + 1, fp) == NULL)
    return 0;

  subst(line, '\n','\0');

  return 1;
}

/*[8]*/
struct date *new_date(struct date *d, char *str)
{
  char *ptr[3];

  if (split(str, ptr, '-', 3) != 3)
    return NULL;

  d->y = atoi(ptr[0]);
  d->m = atoi(ptr[1]);
  d->d = atoi(ptr[2]);

  return d;
}

/*[9]*/
struct profile *new_profile(struct profile *p, char *csv){
  char *ptr[5];

  if (split(csv, ptr, ',', 5) != 5)
    return NULL;

  p->id = atoi(ptr[0]); 

  strncpy(p->name, ptr[1], MAXSTR);
  p->name[MAXSTR] = '\0';

  if (new_date(&p->birthday, ptr[2]) == 0)
    return 0;

  strncpy(p->home, ptr[3], MAXSTR);
  p->home[MAXSTR] = '\0';

  p->comment = (char *)malloc(sizeof(char) * (strlen(ptr[4]) +1));
  strcpy(p->comment, ptr[4]);

  flag = 1;
  return p;
}

/*[10]*/
void cmd_quit(char *param)
{
  if(flag==0) exit(0);
  if(*param == 'a') exit(0);
  else {
  printf("入力されたデータが保存されていません．\n");
  printf("%%Q a でこのまま終了します．\n");
  }
}
/*[11]*/
void cmd_check(int new_s)
{
  //printf("登録件数は%d件です．\n", profile_data_nitems);
  char s[MAX_LINE_LEN + 1] = {'\0'};
  snprintf(s, MAX_LINE_LEN, "登録件数は%d件です．", profile_data_nitems);
  send(new_s, s, sizeof(s), 0);
}
/*[12]*/
char *date_to_string(char buf[], struct date *date)
{
  sprintf(buf, "%04d-%02d-%02d", date->y, date->m, date->d); /*文字列の中に入れる*/
  return buf;
}

/*[13]*/
void print_profile(struct profile *p)
{
  char date[11];

  printf("Id    : %d\n", p->id);
  printf("Name  : %s\n", p->name);
  printf("Birth : %s\n", date_to_string(date, &p->birthday));
  printf("Addr  : %s\n", p->home);
  printf("Com.  : %s\n", p->comment);
}

/*[14]*/
void cmd_print(int nitems)
{
  int i, end = profile_data_nitems;

  if(nitems == 0){
    for(i=0;i<end;i++){
      print_profile(&profile_data_store[i]);
      printf("\n");
  }
  }else if(0 < nitems){
    if(nitems > end) nitems = end;
    for(i=0;i<nitems;i++){
      print_profile(&profile_data_store[i]);
      printf("\n");
    }
  }else if(nitems < 0){
    end=end+nitems;
    if(end< 0) end = 0;
    for(i=end;i < profile_data_nitems;i++){
      print_profile(&profile_data_store[i]);
      printf("\n");
    }
  }
}

/*[15]*/
void cmd_read(char *filename, int new_s)
{
  char buffer[MAX_LINE_LEN + 1];
  int a,b;
  FILE *fp;
  char s[MAX_LINE_LEN + 1] = "\0";

  a = profile_data_nitems;
  fp = fopen(filename, "r");

  if(fp == NULL) {
    //fprintf(stderr,"ファイルがありません，ファイル名を確認してください．\n");
    snprintf(s, MAX_LINE_LEN, "ファイルがありません，ファイル名を確認してください．\n");
    send(new_s, s, sizeof(s), 0);
    return;
  }
  while(get_line(fp ,buffer))
    {
      parse_line(buffer, new_s);
    }	
  b = profile_data_nitems;
  fclose(fp);

  ditems = b - a;
  back = 1;
  // printf("読み込みが完了しました．%%C等で確認してください．\n");
  snprintf(s, MAX_LINE_LEN, "読み込みが完了しました．%%C等で確認してください．\n");
  send(new_s, s, sizeof(s), 0);
}

/*[16]*/
void fprint_profile_csv(int i, FILE *fp)
{
  fprintf(fp,"%d,", profile_data_store[i].id);
  fprintf(fp,"%s,", profile_data_store[i].name);
  fprintf(fp,"%04d-%02d-%02d,", profile_data_store[i].birthday.y
	  ,profile_data_store[i].birthday.m, profile_data_store[i].birthday.d);
  fprintf(fp,"%s,", profile_data_store[i].home);
  fprintf(fp,"%s\n", profile_data_store[i].comment);
}

/*[17]*/
void cmd_write(char *filename, int new_s)
{
  int i;
  FILE *fp;
  char *file = "writefile.csv";
  char s[MAX_LINE_LEN + 1] = "\0";

  if(*filename == 0) fp = fopen(file,"w");
  else fp = fopen(filename, "w");

  for(i = 0; i < profile_data_nitems; i++){
    fprint_profile_csv(i,fp);
  }

  fclose(fp);

  flag = 0;

  //printf("書き込みが完了しました．ファイルを確認してください．\n");
  snprintf(s,MAX_LINE_LEN,"書き込みが完了しました．ファイルを確認してください．\n");
  send(new_s, s, sizeof(s), 0);
}

/*[18]*/
void cmd_find(char *word)
{
  int i;
  struct profile *p;
  char id[MAX_ID_LEN+1];
  char date[MAX_BIRTH_LEN+1];
  
  for(i=0;i<profile_data_nitems;i++){
    p = &profile_data_store[i];
    sprintf(id, "%d", p->id);
    if(strcmp(id, word) == 0 ||
       strcmp(p->name, word) == 0 ||
       strcmp(date_to_string(date, &(p->birthday)), word) == 0 ||
       strcmp(p->home, word) == 0 ||
       strcmp(p->comment, word) == 0){
      printf("%d\n", i+1);
      print_profile(p);
      printf("\n");
    }
  }
  printf("検索が完了しました．該当するデータがなかった場合，何も表示されません．\n");
}

/*[19]*/
void swap(struct profile *a, struct profile *b)
{
  struct profile tmp;
  
  tmp = *a;
  *a = *b;
  *b = tmp;
}

/*[20]*/
int compare_date(struct date *d1, struct date *d2)
{
  if (d1->y != d2->y) return d1->y - d2->y;
  if (d1->m != d2->m) return d1->m - d2->m;
  return d1->d - d2->d;
}

/*[21]*/
int profile_compare(struct profile *p1, struct profile *p2, int column)
{
  switch (column){
  case 1:
    return p1->id - p2->id; break;
  case 2:
    return strcmp(p1->name,p2->name); break;
  case 3:
    return compare_date(&(p1->birthday),&(p2->birthday)); break;
  case 4:
    return strcmp(p1->home,p2->home); break;
  case 5:
    return strcmp(p1->comment,p2->comment); break;
  }
  return 0; 
}


/*[22]*/
void cmd_sort(int param)
{
  int i, j;
  struct profile *p;

  if(0< param && param <6){
  for (i = 0; i < profile_data_nitems -1; i++) {
    for (j = 0; j < profile_data_nitems -1; j++) {
      p = &profile_data_store[j];
      
      if (profile_compare(p, p+1, param) > 0)
	swap(p, p+1);
    }
  }
  back = 0;
  printf("ソートが完了しました．%%Pなどで確認してください．\n");
  }else{
    printf("有効な引数は1~5です．正しい引数を入力してください．\n");
  }
}

/*[23]*/
void ndelete(int nitems)
{
  int i;
  for(i=0;i<nitems;i++){
    free(profile_data_store[profile_data_nitems-1].comment);
    profile_data_nitems--;
  }
}

/*[24]*/
void cmd_delete(int param)
{
  int i;
  FILE *fp;

  fp = fopen("backup.txt", "w");
  mark = 0;

  if(param == 0){
    fprint_profile_csv(profile_data_nitems-1,fp);
    ndelete(1);
  }
  else if(param > 0 && param< profile_data_nitems + 1){
    for(i=0;i<param;i++)
      fprint_profile_csv(profile_data_nitems-param+i,fp);
    ndelete(param);
  }
  else if(param < 0 && -profile_data_nitems -1 <  param){
    param = -param;
    fprint_profile_csv(param-1,fp);
    for(i=0;i<(profile_data_nitems -param);i++){
      swap(&profile_data_store[param-1+i]
	   ,&profile_data_store[param+i]);
    }
    ndelete(1);
    mark = param;
  } else {
    fprintf(stderr,"保存件数は%d件です\n正しい引数を入力してください\n"
	    ,profile_data_nitems);
    return;
  }
  fclose(fp);
  back = 2;
}

/*[25]*/
void cmd_add(param)
{
  int i;
  char line[MAX_LINE_LEN+1];
  struct profile *p;
  mark = -param;

  if(0 < param && param< profile_data_nitems){
  printf("CSV形式で名簿データを入力してください．\n");

  get_line(stdin,line);
  //  parse_line(line); たぶんnew_sもわたす

  p = &profile_data_store[profile_data_nitems-1];

  for(i=0;i<(profile_data_nitems - param); i++){
    swap(p-i-1,p-i);
  }

  back = 3;
  printf("登録が完了しました．\n");
  }else{
    printf("登録件数は%d件です．正しい引数を入力してください．\n",profile_data_nitems);
  }
}

/*[26]*/
void cmd_back(int new_s)
{
  int i;
  struct profile *p;

  switch(back){

  case 0:
    printf("%%Bコマンドは，%%R,%%D,%%Aコマンド実行後しか使用できません．\n"); break;
    
  case 1:
    ndelete(ditems); 
    printf("%%Rコマンド実行前の状態に戻りました．\n"); 
    break;

  case 2:
    cmd_read("backup.txt", new_s);
    p = &profile_data_store[profile_data_nitems-1];
    if(mark == 0){
      for(i=0;i<profile_data_nitems-mark;i++)
	swap(p-1-i,p-i);
    }
    printf("%%Dコマンド実行前の状態に戻りました．\n");
    break;
    
  case 3:
    cmd_delete(mark); 
    printf("%%Aコマンド実行前の状態に戻りました．\n");
    break;
   
  }

  mark=0;
  back=0;
}

/*[27]*/
void cmd_man(int new_s)
{
  /*
  printf("\n");
  printf("このプログラムは標準入力から「ID，氏名，年月日，住所，備考」からなるコンマ区切り形式(CSV形式)の名簿データを受け付けて，それらを名簿中に登録する名簿管理プログラムである．\n");
  printf("下記では，%%で始まる各コマンド入力の仕様を説明している．\n");
  printf("\n");
  printf("%%Q      |終了(Quit)\n");
  printf("%%C      |登録件数などの表示(Check)\n");
  printf("%%P n    |先頭からn件表示(Print)\n");
  printf("%%R file |fileから読み込み(Read)\n");
  printf("%%W file |fileへ書き出し(Write)\n");
  printf("%%F word |wordを検索(Find)\n");
  printf("%%S n    |データをn番目の項目で整列(Sort)\n");
  printf("%%D n    |データをn件削除(Delete)\n");
  printf("%%A n    |n番目にデータを登録(Add)\n");
  printf("%%B      |直前の状態に戻る(Back)\n");
  printf("%%M      |各コマンドの仕様(Manual)\n");
  printf("\n");
  */

  //printf("登録件数は%d件です．\n", profile_data_nitems);
  char s[MAX_LINE_LEN + 1] = {'\0'};
  snprintf(s, MAX_LINE_LEN, "\nこのプログラムは標準入力から「ID，氏名，年月日，住所，備考」からなるコンマ区切り形式(CSV形式)の名簿データを受け付けて，それらを名簿中に登録する名簿管理プログラムである．\n下記では，%%で始まる各コマンド入力の仕様を説明している．\n\n%%Q      |終了(Quit)\n%%C      |登録件数などの表示(Check)\n%%P n    |先頭からn件表示(Print)\n%%R file |fileから読み込み(Read)\n%%W file |fileへ書き出し(Write)\n%%F word |wordを検索(Find)\n%%S n    |データをn番目の項目で整列(Sort)\n%%D n    |データをn件削除(Delete)\n%%A n    |n番目にデータを登録(Add)\n%%B      |直前の状態に戻る(Back)\n%%M      |各コマンドの仕様(Manual)\n\n");
  send(new_s, s, sizeof(s), 0);

  

}

/*[28]*/
void exec_command(char cmd, char *param, int new_s) //全てのコマンドにnew_sわたす　いまはcだけ
{
  switch (cmd) {
  case 'Q': cmd_quit(param); break;
  case 'C': cmd_check(new_s); break;
  case 'P': cmd_print(atoi(param)); break;
  case 'R': cmd_read(param, new_s); break;
  case 'W': cmd_write(param, new_s); break;
  case 'F': cmd_find(param); break;
  case 'S': cmd_sort(atoi(param)); break;
  case 'D': cmd_delete(atoi(param)); break;
  case 'A': cmd_add(atoi(param)); break;
  case 'B': cmd_back(new_s); break;
  case 'M': cmd_man(new_s); break;
  default:
    fprintf(stderr, "%cは登録されていないコマンドです.%%Mなどでコマンドを確認してください．\n",  cmd); //表示をsendに変えてclientにおくる
    break;
  }
}



/*[29]*/
void parse_line(char *line, int new_s)
{
  if(line[0] == '%') {
    exec_command(line[1], &line[3], new_s);
  } else if (new_profile(&profile_data_store[profile_data_nitems], line)!=NULL){
      profile_data_nitems++;
      back = 1;
      ditems = 1;
  } else {
    fprintf(stderr,"入力形式が違います．\n");
  }
}

/*[30]
int main()
{
  char line[MAX_LINE_LEN + 1];
  printf("CSV形式でデータを登録，もしくは%%から始まるコマンドを入力してください．\n");
  while (get_line(stdin ,line)) {
    //   parse_line(line);
  }
  return 0;
}
*/
