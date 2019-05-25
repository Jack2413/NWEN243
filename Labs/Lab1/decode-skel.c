
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/* Program developed for NWEN243, Victoria University of Wellington
 Author: Kris Bubendorfer (c) 2014-15.
 Uses a modified caeser I & II (in 2015)
 
 Compile:  gcc -o decode decode.c
 
 See encode for  examples on using it
 */

char upcase(char ch){
    if(islower(ch))
        ch -= 'a' - 'A';
    return ch;
}




bool isduplicate2(char ch, char* s){
    int c;
    for (c = 0; c < 26; c++){
        if(ch== s[c]){
            return true;
        }
    }
    return false;
}

bool isduplicate(int num, char* s){
    for(int i= 0; i < num ; i++){
        if(s[num]== s[i]){
            return true;
        }
    }
    return false;
}
char* fixkey(char* s){
    int i, j;
    char plain[26]; // assume key < length of alphabet, local array on stack, will go away!
    
    for(i = 0, j = 0; i < strlen(s); i++){
        if(isalpha(s[i])&&!isduplicate(i,s)){
            plain[j++] = upcase(s[i]);
        }
    }
    plain[j] = '\0';
    return strcpy(s, plain);
}


int in(char c, char* s, int pos){
    // Assume everything is already in the same case
    int i;
    
    for(i = 0; i < pos; i++)
        if(c == s[i]) return 1;
    
    return 0;
}

void getNextChar(int i, char* encode){
    
    int next = 0;
    char final = '\0';
    
    do{
        next++;
        if(i==0){
            if((encode[25]+next)>'Z'){ // encode with the alphabet order if is larger then Z
                encode[i]=encode[25]+next-26;   // then will go back 26 alphabet
            }else{
                encode[i]=encode[25]+next; // otherwise just go to next char
                //printf("%c",encode[i]);
                return;
            }
        }else{
            if((encode[i-1]+next)>'Z'){ // encode with the alphabet order if is larger then Z
                final=encode[i-1]+next-26;   // then will go back 26 alphabet
            }else{
                final=encode[i-1]+next; // otherwise just go to next char
                //printf("%c",encode[i]);
            }
        }
    }while (isduplicate2(final,encode)); //to check before i is duplicate or not
    
    encode[i] = final;
}



void buildtable (char* key, char* decode){ // this changed from encode
    
    // This function needs to build an array of mappings in 'encode' from plaintext characters
    // to encihered characters.
    
    // You are implementing a Caesar 1 & 2 combo Cypher as given in the lab handout.
    // Your code here:
    
    // probably need to declare some stuff here!
    int shift = strlen(key)-1;
    int count = 0;
    int j = 0;
    int i = shift;
    
    // fix the key, i.e., uppercase and remove whitespace and punctuation
    
    // the simplest way to do this is to do exactly the same as you did when creating the
    // encode table, and then look up the encode table to get the translations, and build the
    // decode table from this.  This isn't the most efficient approach, but it will get the
    // job done unless you want to be fancy.
    char encode[27];
    for(int i = 0; i<26; i++){
        encode[i]=' ';
        decode[i]=' ';
    }
    encode[26]='\0';
    decode[26]='\0';
    
    fixkey(key); // fix the key, i.e., uppercase and remove whitespace and punctuation
    int newlen = strlen(key);
    
    while(true){ // start with the key, depends on how mush it shifted
        //printf("count: %d\n",count);
        if(j<newlen){
            encode[i]=key[j++];
        }else{
            getNextChar(i,encode);
        }
        i++;
        if(i>25){
            i=0;
        }
        count++;
        if(count==26){// if the table is filled 26 alphabet break the loop
            break;
        }
    }
    
    for(int i = 0; i < 26; i++){
        decode[encode[i]-'A']=('A'+i); //to find the index of decode fill iin with  alphabet order
        //printf("%c",encode[i]);
    }
    //printf("\n");
    /*
    for(int j = 0; j < 26; j++){
        printf("%c",decode[j]);
      
    }
    */
    // Do some stuff here to make a translation between plain and cypher maps.
    
    
    
}

int main(int argc, char **argv){
    // format will be: 'program' key {encode|decode}
    // We'll be using stdin and stdout for files to encode and decode.
    
    // first allocate some space for our translation table.
    
    char* decode = (char*)malloc(sizeof(char)*26); // this changed from encode
    char ch;
    
    if(argc != 2){
        fprintf(stderr,"format is: '%s' key", argv[0]);
        exit(1);
    }
    
    // Build translation tables, and ensure key is upcased and alpha chars only.
    
    buildtable(argv[1], decode); // this changed from encode
    
    // write the key to stderr (so it doesn't break our pipes)
    
    fprintf(stderr,"key: %s - %lu\n", decode, strlen(decode));
    
    
    // the following code does the translations.  Characters are read
    // one-by-one from stdin, translated and written to stdout.
    
    ch = fgetc(stdin);
    while (!feof(stdin)) {
        if(isalpha(ch))          // only decrypt alpha chars
            fputc(decode[ch-'A'], stdout);
        else
            fputc(ch, stdout);
        ch = fgetc(stdin);      // get next char from stdin
    }
}


