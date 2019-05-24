#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "sha256.h"

/*
This program is designed specifically to target passwords with 4 or 6 characters.
If you want to include other length of passwords, please change it in the define
section down below.

The program will first start with comparing each sha256 with a list of common
passwords.It will then moveon to check every possibility of alphabets with small
caps. This is due to the fact that most passwords are in small capital. After
checking it with smaller capital words, it will then try to find variants of
discovered passwords. This is because the password generation suggests that the
passwords with random symbols are just variants of found passwords. Finally, it
will use a recursive function to generate every possible password to find the password.

In addition to just checking with given common password the program will also
check whatever that was inside. For an example, if the password is "landon" and
the common password is "123landon324"; the program will first check "123lan", then
"23land", "3lando", "landon". Hence, covering all different possibility of the
password.
*/

#define PASS_FILE_LENGTH 2 // amount of password files
#define ASCII_START 33 // start of ascii that makes sense
#define ASCII_END 126 // end of ascii that makes sense
#define ASCII_NUM_START 48 // start of ascii with numbers
#define ASCII_NUM_END 58 // end of ascii with numbers
#define ASCII_ALPHA_SMALL_START 97 //start of ascii with small char
#define ASCII_ALPHA_SMALL_END 123 // end of ascii with small char
#define MINIMUM_START 6 // use 1 for more realistic approach/ for "binning" in compare_words
#define MAXIMUM_STOP 6 // use strlen(word) for more realistic approach for "binning" in compare_words
#define SKIP 2 // use 1 for more realistic approach for "binning", in compare_words
#define END_WORD_LEN 6 //ending of the word generation in extend_search_min
#define INSERTNUM -1 // used for inserting number in insert_num
#define CHEAT_FILE "rockyou.txt" // my cheat file
#define KNOWN_PASS "known_passlist.txt" // known password file
#define START_WORD "abcdef" // to start off the search
#define VARIANTS_INT 3

typedef struct Point_t{
  char* password;
  int pos;
  struct Point_t* next;
}Point;

typedef struct List_t{
  Point* head;
  Point* tail;
}List;

typedef struct Point_p{
  BYTE* sha_pass;
  int pos;
  struct Point_p* next;
  struct Point_p* before;
}Point_pass;

typedef struct List_p{
  Point_pass* head;
  Point_pass* tail;
}List_pass;

//creates a new word based on conditions
char* get_new_word(char* word, int num, int pos);
//creating a linked list for given passwords to be find
Point* create_point();
List* create_list();
//creating a linked list for passwords that needs to be found
Point_pass* create_point_pass();
List_pass* create_list_pass();
//comparing the words with the hash value to figure out passwords
bool compare_words(char* word, List_pass*pass_list_sha, List* known_pass_list);
//check if there is an existing password file *skips previous searches*
bool check_existing(char* filename, List_pass* pass_list_sha, List* known_pass_list);
//ensure that the word does not have repetitive letters
bool check_variants(char* word, int num);
//changing the letters inside of each word
int check_words(char* word, List_pass* pass_list_sha, List* known_pass_list, int pos);
//changing the end of each word
int words_check(char* word, List_pass* pass_list_sha, List* known_pass_list, int pos);
//attack designed specifically based on known passwords; most passwords with symbols are variation of found passwords
int extend_search(char* filename, List_pass* pass_list_sha, List* known_pass_list);
//attack designed specifically for the project, because most found passwords are small case letters, find them first.
void extend_search_min(List_pass* pass_list_sha, List* known_pass_list, int length);
// basically go through all possibilities
void extend_search_final(List_pass* pass_list_sha, List* known_pass_list);
// get the given list passwords
void get_list(char* filename, List_pass* list_pass);
//comparing the password file with the passwords-to-be-crack files
void compare_files(char* pass_file, char* sha_file);
//does requirements for 1,2,3 argc
void one_argc(char* filename, List_pass* pass_list_sha, List* known_pass_list);
void two_argc(char* filename, List_pass*pass_list_sha, List* known_pass_list);
void three_argc(char* filename, List_pass* pass_list_sha, List* known_pass_list);
//converts str to byte
void convert_str_byte(char* input, BYTE* output);
//converts string into small case
void convert_str_lower(char* input);
//insert into password file for later use
void insert_pass_file(List* known_pass_list);
//cause submittion needs it *I am not going to change the whole structure*
void insert_submition_file(List* known_pass_list);
//insert found passwords into the linked list
void insert_pass_list(char* word, int pos, List* list);
//insert byte passwords into a linked list
void insert_sha_pass(BYTE* sha_256_pass, List_pass* list_pass);
//deleting the byte passwords after found
void delete_sha_pass(BYTE* sha_256_pass, List_pass* list_pass);
//for debugging
void traverse_sha(List_pass* list_pass);
//changes all into only small case characters
void numeric_words(char* words, List_pass* pass_list_sha, List* known_pass_list, int length);
//option for numeric_words if it wants to inset numbers variants as well
void insert_num(char* word, List_pass* pass_list_sha, List* known_pass_list, int pos);

long int num_tries;
long int counter;

int main(int argc, char* argv[]){
  List_pass* pass_list_sha;
  List* known_pass_list;
  char* given_list[2]={"unc_pass/pwd4sha256","unc_pass/pwd6sha256"};
  char* cheat=CHEAT_FILE;
  char* k_pass=KNOWN_PASS;

  counter=0;
  num_tries=-1;

  known_pass_list=create_list();
  pass_list_sha=create_list_pass();

  if(argc==1){
    for(int i=0; i<PASS_FILE_LENGTH; i++){
      get_list(given_list[i], pass_list_sha);
    }
    //check if there are any existing cracked passwords, if there is, skip scanning the retrieved files
    if(!check_existing(k_pass, pass_list_sha, known_pass_list)){
      one_argc(cheat, pass_list_sha, known_pass_list);
    }
  }

  else if(argc==2){
    for(int i=0; i<PASS_FILE_LENGTH; i++){
      get_list(given_list[i], pass_list_sha);
    }
    num_tries=strtol(argv[1], NULL, 10);
    //check if there are any existing cracked passwords, if there is, skip scanning the retrieved files
    if(!check_existing(k_pass, pass_list_sha, known_pass_list)){
      two_argc(cheat, pass_list_sha, known_pass_list);
    }
  }

  else if(argc==3){
    //change common password file to something else
    num_tries=-2;
    cheat=argv[1];
    get_list(argv[2], pass_list_sha);
    three_argc(cheat, pass_list_sha, known_pass_list);
  }

  //skipping search again if it is found previously assuming that its for testing
  extend_search_min(pass_list_sha, known_pass_list, END_WORD_LEN);

  //check gained passwords variants
  insert_pass_file(known_pass_list);
  while(extend_search(k_pass, pass_list_sha, known_pass_list)){
    insert_pass_file(known_pass_list);
  }

  //insert found passwords into a file
  insert_pass_file(known_pass_list);
  //search through everything
  extend_search_final(pass_list_sha, known_pass_list);
  //update known lists;
  insert_pass_file(known_pass_list);
  return 0;
}

//creates a word variant of the previous word
char* get_new_word(char* word, int num, int pos){
  char* new_word;

  new_word=malloc(sizeof(char)*strlen(word));
  strcpy(new_word,word);
  new_word[pos]=(char)num;
  return new_word;
}

//linked list structure
Point* create_point(){
  Point* new_point=malloc(sizeof(Point));
  new_point->pos=0;
  new_point->password=NULL;
  new_point->next=NULL;
  return new_point;
}

Point_pass* create_point_pass(){
  Point_pass* new_pass=malloc(sizeof(Point_pass));
  new_pass->sha_pass=malloc(sizeof(BYTE)*32);
  new_pass->pos=0;
  new_pass->next=NULL;
  new_pass->before=NULL;
  return new_pass;
}

List* create_list(){
  List* new_list=malloc(sizeof(List));
  new_list->head=NULL;
  new_list->tail=NULL;
  return new_list;
}

List_pass* create_list_pass(){
  List_pass* new_list_pass=malloc(sizeof(List_pass));
  new_list_pass->head=NULL;
  new_list_pass->tail=NULL;
  return new_list_pass;
}

//function to compare words
bool compare_words(char* word, List_pass* pass_list_sha, List* known_pass_list){
  Point_pass* next_point;

  char temp_text[32];
  char* original_word;

  BYTE* b_text;
  SHA256_CTX ctx;
  BYTE buff[SHA256_BLOCK_SIZE];

  original_word=word;

  if(!pass_list_sha||(counter>=num_tries&&num_tries>0)){
    return false;
  }

  next_point=pass_list_sha->head;
  b_text=malloc(sizeof(BYTE)*strlen(word));
  //go through each possibilities inside the word, ie. length of 4 and end at length of 6
  //configured to check only password length 4, or 6 for now
  for(int wordlen=MINIMUM_START; wordlen<=MAXIMUM_STOP; wordlen+=SKIP){
    //go through each string base on the length of the word
    while(strlen(word)>=wordlen){
      strncpy(temp_text, word, wordlen);
      temp_text[wordlen]='\0';
      //encrypting with sha256
      convert_str_byte(temp_text, b_text);
      sha256_init(&ctx);
      sha256_update(&ctx, b_text, wordlen);
      sha256_final(&ctx, buff);
      //loop through stored hashes and compare
      while(next_point){
        if(!memcmp(buff,next_point->sha_pass,32)){
          //insert the found password into a list again
          insert_pass_list(temp_text, next_point->pos, known_pass_list);
          //check the different argc value
          if(num_tries==-1){
              printf("%s %d\n", temp_text, next_point->pos);
          }
          if(num_tries>0){
            printf("%s\n", temp_text);
          }
          //removing from the search, so does not need to search anymore
          delete_sha_pass(buff, pass_list_sha);
          return true;
        }
        //moving the linked list
        next_point=next_point->next;
        //adding the counter
        counter++;
        if(counter>num_tries && num_tries>0){
          return false;
        }
      }
      //restart list
      next_point=pass_list_sha->head;
      //move along the word
      word++;
    }
    //restart word
    word=original_word;
  }
  free(b_text);
  return false;
}

//check for existing password lists; to skip steps)
bool check_existing(char* filename, List_pass* pass_list_sha, List* known_pass_list){
  FILE* fp;

  char temp_text[32];

  fp=fopen(filename,"r");

  //checking file pointer
  if(!fp){
    return false;
  }

  //loop through the texts and compare them
  while(fgets(temp_text, 32, fp)){
    compare_words(temp_text, pass_list_sha, known_pass_list);
  }

  fclose(fp);
  return true;
}

//check for repetitive chars
bool check_variants(char* word, int num){
  int word_len= strlen(word);
  int count=0;
  for(int i=0; i<word_len;i++){
    if(word[i]==word[(i+1)%word_len]){
      count++;
    }
    if(count>=num){
      return true;
    }
  }
  return false;
}

//recursive function that replaces different char values from known passwords, specially designed attack
int check_words(char* word, List_pass* pass_list_sha, List* known_pass_list, int pos){
  int text_len=strlen(word);
  int new_pass=0;
  char* comp_word;

  if(pos<0){
    return new_pass;
  }

  //go through each position in the word
  for(int i=pos; i<text_len;i++){
    //go through each posibile ascii characters
    for(int j=ASCII_START; j<ASCII_END; j++){
      //replace the characters into the word to compare
      comp_word=get_new_word(word,j,i);
      //recursive stage
      new_pass+=check_words(comp_word, pass_list_sha, known_pass_list, pos-1);
      if(compare_words(comp_word, pass_list_sha, known_pass_list)){
        new_pass++;
      }
      free(comp_word);
    }
  }
  //returns the value of new found passwords
  return new_pass;
}

//basically check_words but only for the end of the word value; used to test password generation patterns
int words_check(char* word, List_pass* pass_list_sha, List* known_pass_list, int pos){
  int text_len=strlen(word);
  int position=text_len-pos;
  int new_pass=0;
  char* comp_word;

  if(pos<0){
    return new_pass;
  }

  for(int i=position; i<text_len;i++){
    for(int j=ASCII_START; j<ASCII_END; j++){
      comp_word=get_new_word(word,j,i);
      new_pass+=words_check(comp_word, pass_list_sha, known_pass_list, pos-1);
      if(compare_words(comp_word, pass_list_sha, known_pass_list)){
        new_pass++;
      }
      free(comp_word);
    }
  }
  return new_pass;
return new_pass;
}

//used to initialize check_words
int extend_search(char* filename, List_pass* pass_list_sha, List* known_pass_list){
  FILE* fp;

  char temp_text[32];
  int new_pass=0;

  fp=fopen(filename, "r");

  if(!fp){
    return 0;
  }

  //go through each texts in the found password file
  while(fgets(temp_text, sizeof(temp_text), fp)){
    temp_text[strlen(temp_text)-1]='\0';

    new_pass+=check_words(temp_text, pass_list_sha, known_pass_list, 1);
  }
  fclose(fp);
  //returns new found passwords
  return new_pass;
}

//go through small caps alphabet, spefically designed attack due to password generation patterns
void extend_search_min(List_pass* pass_list_sha, List* known_pass_list, int length){
  char* word=malloc(sizeof(char)*length);
  strcpy(word,START_WORD);
  numeric_words(word, pass_list_sha, known_pass_list, length-1);
  free(word);
  return;
}

//go through every possibilities in a 6 password
void extend_search_final(List_pass* pass_list_sha, List* known_pass_list){
  char* start=malloc(sizeof(char)*strlen(START_WORD)+1);
  strcpy(start,START_WORD);
  if(counter>=num_tries && num_tries>0){
    return;
  }
  check_words(start, pass_list_sha, known_pass_list, END_WORD_LEN-1);
  return;
}

//places the byte file into a linked list
void get_list(char* filename, List_pass* list_pass){

  unsigned char byte[32];

  if(!list_pass){
    return;
  }

  FILE* fp;
  fp=fopen(filename, "rb");

  while(fread(byte,sizeof(byte),1,fp)){
    insert_sha_pass(byte, list_pass);
  }
  fclose(fp);
}

//different argc value arguments, for sorting
void one_argc(char* filename, List_pass*pass_list_sha, List* known_pass_list){
  FILE* fp;
  char temp_text[32];

  fp=fopen(filename, "r");

  while(fgets(temp_text, 32, fp)){
    convert_str_lower(temp_text);
    temp_text[strlen(temp_text)-1]='\n';
    compare_words(temp_text, pass_list_sha, known_pass_list);
  }

  fclose(fp);
  return;
}

void two_argc(char* filename, List_pass*pass_list_sha, List* known_pass_list){
  FILE* fp;
  char temp_text[32];

  fp=fopen(filename, "r");

  while(fgets(temp_text, 32, fp)){
    convert_str_lower(temp_text);
    compare_words(temp_text, pass_list_sha, known_pass_list);
    if(counter>num_tries && num_tries>0){
      break;
    }
  }
  fclose(fp);
  return;
}

void three_argc(char* filename, List_pass*pass_list_sha, List* known_pass_list){
  FILE* fp;
  char temp_text[10000];

  fp=fopen(filename, "r");

  while(fgets(temp_text, 32, fp)){
    convert_str_lower(temp_text);
    temp_text[strlen(temp_text)-1]='\0';
    compare_words(temp_text, pass_list_sha, known_pass_list);
  }

  fclose(fp);
  return;
}

//converts string to bytes
void convert_str_byte(char* input, BYTE* output){
  for(int i=0; i<strlen(input);i++){
    output[i]=input[i];
  }
}

//converts string to lower case, due to the nature of the password generation
void convert_str_lower(char* input){
  for(int i = 0; i<strlen(input); i++){
    input[i] = tolower(input[i]);
  }
}

//insert the found passwords into a file
void insert_pass_file(List* known_pass_list){
  FILE* fp;
  Point* records;
  char* temp_word;

  fp=fopen("known_passlist.txt","w");

  if(!known_pass_list || !known_pass_list->head){
    return;
  }

  records=known_pass_list->head;
  while(records){
    temp_word=malloc(sizeof(char)*strlen(records->password)+2);
    strcpy(temp_word,records->password);
    temp_word[strlen(temp_word)+1]='\0';
    temp_word[strlen(temp_word)]='\n';

    fprintf(fp, "%s", temp_word);
    free(temp_word);
    records=records->next;
  }
  fclose(fp);
  insert_submition_file(known_pass_list);

  return;
}

//because the assignment said so
void insert_submition_file(List* known_pass_list){
  FILE* fp;
  Point* records;
  char* temp_word;
  char* temp_num;

  fp=fopen("found_pwds.txt","w");

  if(!known_pass_list || !known_pass_list->head){
    return;
  }


  records=known_pass_list->head;

  while(records){
    temp_num=malloc(sizeof(char)*(records->pos/10)+1);
    temp_word=malloc(sizeof(char)*strlen(records->password)+(records->pos/10)+1+3);
    strcpy(temp_word,records->password);
    strcat(temp_word, " ");
    sprintf(temp_num,"%d",records->pos);
    strcat(temp_word, temp_num);
    temp_word[strlen(temp_word)+1]='\0';
    temp_word[strlen(temp_word)]='\n';

    fprintf(fp, "%s", temp_word);
    free(temp_word);
    records=records->next;
    free(temp_num);
  }
  fclose(fp);
  return;
}

//insert found passwords into a list
void insert_pass_list(char* word, int pos, List* list){
  Point* pass_rec;
  Point* records;

  pass_rec=create_point();
  pass_rec->password=malloc(sizeof(char*)*strlen(word));
  pass_rec->pos=pos;
  strcpy(pass_rec->password, word);

  if(!list){
    list=create_list();
  }
  if(!list->head){
    list->head=list->tail=pass_rec;
    return;
  }

  records=list->head;
  while(records){
    if(!strcmp(records->password,word)){
      break;
    }
    if(!records->next){
      records->next=pass_rec;
      list->tail=pass_rec;
      break;
    }
    records=records->next;
  }
  return;
}

//insert the BYTE from get_list into linked list
void insert_sha_pass(BYTE* sha_256_pass, List_pass* list_pass){
  Point_pass* new_point;
  Point_pass* next_point;

  int position=1;

  new_point=create_point_pass();
  memcpy(new_point->sha_pass, sha_256_pass, 32);

  if(!list_pass){
    return;
  }

  if(!list_pass->head){
    list_pass->head=list_pass->tail=new_point;
    return;
  }

  next_point=list_pass->head;
  while(next_point){
    if(!next_point->next){
      next_point->pos=position;
      next_point->next=new_point;
      new_point->before=next_point;
      list_pass->tail=new_point;
      break;
    }
    next_point=next_point->next;
    position++;
  }

  return;
}

//for deleting found passwords, ensures that theres no duplicate
void delete_sha_pass(BYTE* sha_256_pass, List_pass* list_pass){
  Point_pass* next_point;

  if(!list_pass|| !list_pass->head){
    return;
  }

  next_point=list_pass->head;

  while(next_point){
    if(!memcmp(sha_256_pass,next_point->sha_pass,32)){
      if(!next_point->before && !next_point->next){
        list_pass->head=list_pass->tail=NULL;
        free(next_point);
        break;
      }
      if(!next_point->before){
        next_point->next->before=NULL;
        list_pass->head=next_point->next;
        free(next_point);
        break;
      }
      if(!next_point->next){
        next_point->before->next=NULL;
        list_pass->tail=next_point->before;
        free(next_point);
        break;
      }
      next_point->before->next=next_point->next;
      next_point->next->before=next_point->before;
      free(next_point);
      break;
    }
    next_point=next_point->next;
  }
  return;
}

//for testing, basically prints out the sha value
void traverse_sha(List_pass* list_pass){
  Point_pass* next_point;
  if(!list_pass|| !list_pass->head){
    return;
  }

  next_point=list_pass->head;

  while(next_point){
    printf("%02x\n",next_point->sha_pass[0]);
    next_point=next_point->next;
  }
  return;
}

//loop through every posibility based on small caps letters, can be used to test numbers as well
void numeric_words(char* word, List_pass* pass_list_sha, List* known_pass_list, int length){
  //ensures that there is no same 3 letter;
  if(check_variants(word, VARIANTS_INT)){
    word[length+1]=(char)((int)word[length+1]-1);
    return;
  }

  if(length<0){
    //tests the password generated
    compare_words(word, pass_list_sha, known_pass_list);
    //only used when it is specified
    insert_num(word, pass_list_sha, known_pass_list, INSERTNUM);
    return;
  }

  for(int i=ASCII_ALPHA_SMALL_START;i<ASCII_ALPHA_SMALL_END;i++){
    word[length]=(char)i;
    //recursively generates the passwords
    numeric_words(word, pass_list_sha, known_pass_list, length-1);
  }
  return;
}

//used by numeric_words to tests numbers as well
void insert_num(char* word, List_pass* pass_list_sha, List* known_pass_list, int pos){
  int word_len;
  char* comp_word;

  word_len=strlen(word);

  if(pos<0){
    return;
  }
  for(int i=pos; i<word_len; i++){
    for(int j=ASCII_NUM_START; j<ASCII_NUM_END; j++){
      comp_word=get_new_word(word, j, i);
      //recursively call for other positions
      insert_num(comp_word, pass_list_sha, known_pass_list, pos-1);
      compare_words(comp_word, pass_list_sha, known_pass_list);
      free(comp_word);
    }
  }
  return;
}
