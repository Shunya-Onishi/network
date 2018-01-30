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
void cmd_quit(char *param, int new_s)
{
  char s[MAX_LINE_LEN + 1]={'\0'};
  if(flag==0){
    snprintf(s, MAX_LINE_LEN, "終了します。\n");
    send(new_s, s, sizeof(s), 0);
    // exit(0);
      }
  if(*param == 'a'){
    snprintf(s, MAX_LINE_LEN, "終了します。\n");
    send(new_s, s, sizeof(s), 0);
    // exit(0);
      }

  else {
    //    snprintf(s, MAX_LINE_LEN,"入力されたデータが保存されていません．\n%%Q a でこのまま終了します．\n");
    snprintf(s, MAX_LINE_LEN, "終了します。\n");
    send(new_s, s, sizeof(s), 0);
  }
}
/*[11]*/
void cmd_check(int new_s)
{
  //printf("登録件数は%d件です．\n", profile_data_nitems);
  char s[MAX_LINE_LEN + 1] = {'\0'};
  snprintf(s, MAX_LINE_LEN, "登録件数は%d件です．\n", profile_data_nitems);
  send(new_s, s, sizeof(s), 0);
}
/*[12]*/
char *date_to_string(char buf[], struct date *date)
{
  sprintf(buf, "%04d-%02d-%02d", date->y, date->m, date->d); /*文字列の中に入れる*/
  return buf;
}

/*[13]*/
void print_profile(struct profile *p, int new_s)
{
  char date[11];
  char s[MAX_LINE_LEN+1] = {'\0'};

  snprintf(s, MAX_LINE_LEN,"Id    : %d\nName  : %s\nBirth : %s\nAddr  : %s\nCom.  : %s\n", 
	   p->id, p->name, date_to_string(date, &p->birthday), p->home, p->comment);
  send(new_s, s, sizeof(s), 0);

}

/*[14]*/
void cmd_print(int nitems, int new_s)
{
  int i, end = profile_data_nitems;
  char s[MAX_LINE_LEN +1]={'\0'};

  if(nitems == 0){
    snprintf(s, MAX_LINE_LEN, "%d", end);
    send(new_s, s, sizeof(s), 0);
    for(i=0;i<end;i++){
      print_profile(&profile_data_store[i], new_s);
      //     printf("\n");
  }
  }else if(0 < nitems){
    if(nitems > end) nitems = end;
    snprintf(s, MAX_LINE_LEN, "%d", nitems);
    send(new_s, s, sizeof(s), 0);
    for(i=0;i<nitems;i++){
      print_profile(&profile_data_store[i], new_s);
      //     printf("\n");
    }
  }else if(nitems < 0){
    end=end-(-nitems);
    if(end< 0) end = 0;    
    snprintf(s, MAX_LINE_LEN, "%d", -nitems);
    send(new_s, s, sizeof(s), 0);
    for(i=end;i < profile_data_nitems;i++){
      print_profile(&profile_data_store[i], new_s);
      //      printf("\n");
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
    snprintf(s, MAX_LINE_LEN, "ファイルがありません，ファイル名を確認してください．\n");
    send(new_s, s, sizeof(s), 0);
    return;
  }
  while(get_line(fp ,buffer))
    {
    new_profile(&profile_data_store[profile_data_nitems], buffer);
    profile_data_nitems++;
    back = 1;
    ditems = 1;
    }	
  b = profile_data_nitems;
  fclose(fp);

  ditems = b - a;
  back = 1;
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

  snprintf(s,MAX_LINE_LEN,"書き込みが完了しました．ファイルを確認してください．\n");
  send(new_s, s, sizeof(s), 0);
}

/*[18]*/
void cmd_find(char *word, int new_s)
{
  int i;
  int count = 0;
  struct profile *p;
  char id[MAX_ID_LEN+1];
  char date[MAX_BIRTH_LEN+1];
  char s[MAX_LINE_LEN +1] = {'\0'};

  for(i=0;i<profile_data_nitems;i++){
    p = &profile_data_store[i];
    sprintf(id, "%d", p->id);
    if(strcmp(id, word) == 0 ||
       strcmp(p->name, word) == 0 ||
       strcmp(date_to_string(date, &(p->birthday)), word) == 0 ||
       strcmp(p->home, word) == 0 ||
       strcmp(p->comment, word) == 0
       ){
      count++;
    }
  }
  snprintf(s, MAX_LINE_LEN, "%d", count);
  send(new_s, s, sizeof(s), 0);

  for(i=0;i<profile_data_nitems;i++){
    p = &profile_data_store[i];
    sprintf(id, "%d", p->id);
    if(strcmp(id, word) == 0 ||
       strcmp(p->name, word) == 0 ||
       strcmp(date_to_string(date, &(p->birthday)), word) == 0 ||
       strcmp(p->home, word) == 0 ||
       strcmp(p->comment, word) == 0
       ){
      snprintf(s, MAX_LINE_LEN,
	       "Id    : %d\nName  : %s\nBirth : %s\nAddr  : %s\nCom.  : %s\n", 
	       p->id, p->name, date_to_string(date, &p->birthday), p->home, p->comment);
      send(new_s, s, sizeof(s), 0);
    }
  }
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
void cmd_sort(int param, int new_s)
{
  int i, j;
  char s[MAX_LINE_LEN + 1] = {'\0'};
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
  snprintf(s, MAX_LINE_LEN, "ソートが完了しました．%%Pなどで確認してください．\n");
  send(new_s, s, sizeof(s), 0);
  }else{
    snprintf(s, MAX_LINE_LEN, "有効な引数は1~5です．正しい引数を入力してください．\n");
    send(new_s, s, sizeof(s), 0);
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
void cmd_delete(int param, int new_s)
{
  int i;
  FILE *fp;
  char s[MAX_LINE_LEN + 1]={'\0'};
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
    snprintf(s, MAX_LINE_LEN ,"保存件数は%d件です\n正しい引数を入力してください\n"
	    ,profile_data_nitems);
    send(new_s, s, sizeof(s), 0);
    return;
  }
  fclose(fp);
  back = 2;
  snprintf(s, MAX_LINE_LEN ,"delete ok\n");
  send(new_s, s, sizeof(s), 0);
}

/*[25]*/
void cmd_add(int param, int new_s)
{
  int i;
  //  char line[MAX_LINE_LEN+1];
  char s[MAX_LINE_LEN +1] = {'\0'};
  struct profile *p;
  mark = -param;

  if(0 < param && param< profile_data_nitems){
    snprintf(s, MAX_LINE_LEN, "CSV形式で名簿データを入力してください．\n");
    send(new_s, s, sizeof(s), 0);

    //    get_line(stdin,line);
    bzero(&s, sizeof(s));
    recv(new_s, s, sizeof(s), 0);
     parse_line(s, new_s); 


    p = &profile_data_store[profile_data_nitems-1];

    for(i=0;i<(profile_data_nitems - param); i++){
      swap(p-i-1,p-i);
    }

    back = 3;
    /* snprintf(s, MAX_LINE_LEN, "登録が完了しました．\n"); */
    /* send(new_s, s, sizeof(s), 0); */

  }else if(param == profile_data_nitems){

    snprintf(s, MAX_LINE_LEN, "CSV形式で名簿データを入力してください．\n");
    send(new_s, s, sizeof(s), 0);

    //   get_line(stdin,line);
    bzero(&s, sizeof(s));
    recv(new_s, s, sizeof(s), 0);
    parse_line(s, new_s); 

    p = &profile_data_store[profile_data_nitems-1];

    swap(p-1,p);

    back = 3;
    /* snprintf(s, MAX_LINE_LEN, "登録が完了しました．\n"); */
    /* send(new_s, s, sizeof(s), 0); */

  }else{
    snprintf(s, MAX_LINE_LEN, "登録件数は%d件です．正しい引数を入力してください．\n",profile_data_nitems);
    send(new_s, s, sizeof(s), 0);
  }
}

/*[26]*/
void cmd_back(int new_s)
{
  int i;
  char s[MAX_LINE_LEN +1]={'\0'};
  struct profile *p;

  switch(back){

  case 0:
    snprintf(s, MAX_LINE_LEN,"%%Bコマンドは，%%R,%%Aコマンド実行後しか使用できません．\n");
    send(new_s, s,sizeof(s), 0);
    break;
    
  case 1:
    ndelete(ditems); 
    snprintf(s, MAX_LINE_LEN,"%%Rコマンド実行前の状態に戻りました．\n"); 
    send(new_s, s,sizeof(s), 0);
    break;

  case 2:
    /* cmd_read("backup.txt", new_s); */
    /* p = &profile_data_store[profile_data_nitems-1]; */
    /* if(mark == 0){ */
    /*   for(i=0;i<profile_data_nitems-mark;i++) */
    /* 	swap(p-1-i,p-i); */
    /* } */
    snprintf(s, MAX_LINE_LEN,"%%Dコマンド実行前の状態に戻りました．\n");
    send(new_s, s,sizeof(s), 0);
    break;
    
  case 3:
    cmd_delete(mark, new_s); 
    // snprintf(s,MAX_LINE_LEN,"%%Aコマンド実行前の状態に戻りました．\n");
    // send(new_s, s,sizeof(s), 0);
    break;
   
  }

  mark=0;
  back=0;
}

/*[27]*/
void cmd_man(int new_s)
{
  char s[MAX_LINE_LEN + 1] = {'\0'};
  snprintf(s, MAX_LINE_LEN, "\nこのプログラムは標準入力から「ID，氏名，年月日，住所，備考」からなるコンマ区切り形式(CSV形式)の名簿データを受け付けて，それらを名簿中に登録する名簿管理プログラムである．\n下記では，%%で始まる各コマンド入力の仕様を説明している．\n\n%%Q      |終了(Quit)\n%%C      |登録件数などの表示(Check)\n%%P n    |先頭からn件表示(Print)\n%%R file |fileから読み込み(Read)\n%%W file |fileへ書き出し(Write)\n%%F word |wordを検索(Find)\n%%S n    |データをn番目の項目で整列(Sort)\n%%D n    |データをn件削除(Delete)\n%%A n    |n番目にデータを登録(Add)\n%%B      |直前の状態に戻る(Back)\n%%M      |各コマンドの仕様(Manual)\n\n");
  send(new_s, s, sizeof(s), 0);
}

/*[28]*/
void exec_command(char cmd, char *param, int new_s) //全てのコマンドにnew_sわたす　いまはcだけ
{
  char s[MAX_LINE_LEN + 1] = {'\0'};
  switch (cmd) {
  case 'Q': cmd_quit(param, new_s); break; //-----------------[10] -aコマンド以外ok
  case 'C': cmd_check(new_s); break; //-----------------------[11] ok
  case 'P': cmd_print(atoi(param), new_s); break; //----------[14] ok
  case 'R': cmd_read(param, new_s); break; //-----------------[15] ok
  case 'W': cmd_write(param, new_s); break; //----------------[17] ok
  case 'F': cmd_find(param, new_s); break; //-----------------[18] ok
  case 'S': cmd_sort(atoi(param), new_s); break; //-----------[22] ok
  case 'D': cmd_delete(atoi(param), new_s); break; //---------[24] ok 
  case 'A': cmd_add(atoi(param), new_s); break; //------------[25] ok
  case 'B': cmd_back(new_s); break; //------------------------[26] case2(D)以外ok
  case 'M': cmd_man(new_s); break; //-------------------------[27] ok
  default:
    snprintf(s, MAX_LINE_LEN, "%cは登録されていないコマンドです. %%Mなどでコマンドを確認してください．\n",  cmd);
    send(new_s, s, sizeof(s), 0);
    break;
  }
}



/*[29]*/
void parse_line(char *line, int new_s)
{
  char s[MAX_LINE_LEN +1] = {'\0'};
  if(line[0] == '%') {
    exec_command(line[1], &line[3], new_s);
  } else {
    new_profile(&profile_data_store[profile_data_nitems], line);
    profile_data_nitems++;
    back = 1;
    ditems = 1;
    snprintf(s,MAX_LINE_LEN, "New Data added\n");
    send(new_s, s, sizeof(s), 0);
  }/* else if (new_profile(&profile_data_store[profile_data_nitems], line)!=NULL){
      profile_data_nitems++;
      back = 1;
      ditems = 1;
      
      send(new_s, s, sizeof(s), 0);
      } else {
      fprintf(stderr,"入力形式が違います．\n");
      }*/
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
