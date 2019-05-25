
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define TEXT_SIZE 1000000 // Note, the longer the text the more likely you will get a good 'decode' from the start.
#define ALEN 26         // Number of chars in ENGLISH alphabet
#define CHFREQ "ETAONRISHDLFCMUGYPWBVKJXQZ" // Characters in order of appearance in English documents.
#define ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

/* Program developed for NWEN243, Victoria University of Wellington
 Author: Kris Bubendorfer, this extended version (c) 2015
 LAB: 2
 
 This program applies a basic frequency analysis on a cyphertext.  It has been extened over the 2014 version to
 solve polyalphabetic cyphers - by brute force.  In this case, it applies the frequency analysis for different
 numbers of n keys (polyalphabetic Caeser).  Obviously it will need a cypher of about n times
 the typical length for a monoalphabetic cypher.
 
 Program is used like this:
 
 Compile:  gcc -o crack crack.c
 
 Test file (ctext): JWRLS, XSSH PZK JH HES BJFV, UZU (this is not a realistic length piece of cypher text)
 
 crack n
 
 Argument:
 
 n number of keys to try
 
 ---
 
 % cat ctext | crack 1
 ALICE, MEET YOU AT THE PARK, BOB   <-- of course it won't be this correct.  Don't worry about that for the -d option.
 AMFDE, UEET LNH AT TIE RASC, ONO   <-- this is what it really looks like, a larger sample is better, this is short.
 */

char upcase(char ch){
    if(islower(ch))
        ch -= 'a' - 'A';
    return ch;
}

int findMax(int countf[]){
    int max = -1;
    int index =0;
    int j = 0;
    for(; j<26; j++){
        if(max<countf[j]){
            max = countf[j];
            index = j;
        }
    }
    // printf("frequency: %d ",countf[index]);
    countf[index]=-2;
    return index;
}

/*
 void MySorting(int count[]){ //Sorting and change the cipher to plain text
 
 int swap;
 for(int i = 0; i<26; i++){
 int pos = i;
 for(int j = i + 1; j < 26; j++)
 if(count[pos]>count[j]){
 pos = j;
 }
 if(pos != i){
 swap = count[i] ;
 count[i] = CHFREQ[i]; //change the highest frequency char in the text, by use the CHFREQ table
 count[pos]=swap;
 }
 }
 }
 */

int main(int argc, char **argv){
    
    // first allocate some space for our input text (we will read from stdin).
    //char ii =  'A';
    char* text = (char*)malloc(sizeof(char)*TEXT_SIZE+1);
    char ch;
    int n, i;
    
    if(argc > 1 && (n = atoi(argv[1])) > 0);
    else{ fprintf(stderr,"Malformed argument, use: crack [n], n > 0\n"); exit(-1);}
    
    // get the command line argument n
    
    // Now read TEXT_SIZE or feof worth of characters (whichever is smaller) and convert to uppercase as we do it.
    // Added: changed to count frequencies as we read it in
    
    for(i = 0, ch = fgetc(stdin); i < TEXT_SIZE && !feof(stdin); i++, ch = fgetc(stdin)){
        text[i] = (ch = (isalpha(ch)?upcase(ch):ch));
    }
    text[i] = '\0'; // terminate the string properly.
    
    /* At this point we have two things,
     *   1. The input cyphertext in "text"
     *   2. The maximum number of keys to try (n) - we'll be trying 1..n keys.
     *
     * What you need to do is as follows:
     *   1. create a for-loop that will check key lengths from 1..n
     *   2. for each i <= n, spit the cypher text into i sub-texts.  For i = 1, 1 subtext, for i = 2, 2 subtexts, of alternating characters etc.
     *   3. for each subtext:
     *          a. count the occurance of each letter
     *          b. then map this onto the CHFREQ, to create a map between the sub-text and english
     *          c. apply the new map to the subtext
     *   4. merge the subtexts
     *   5. output the 'possibly' partially decoded text to stdout.  This will only look OK if i was the correct number of keys
     *
     * what you need to output (sample will be provided) - exactly:
     * i maps -> stderr
     * i 'possible' translations
     *
     * You would be wise to make seperate functions that perform various sub-tasks, and test them incrementally.  Any other approach will likely
     * make your brain revolt.  This isn't a long program, mine is 160 lines, with comments (and written in a very verbose style) - if yours is
     * getting too long, double check you're on the right track.
     *
     */
    for(int a = 0; a < n; a++){
        int num_key = a+1;
        int STL = (TEXT_SIZE/num_key);
        int count = 0;
        char dtext[num_key][STL];
        memset(dtext,0, sizeof(dtext));
        //printf("count: %d STL: %d num_key: %d n:%d\n",count,STL,num_key,n);
        for(int k = 0; k< STL; k++ ){
            for(int j=0; j<num_key&&count<TEXT_SIZE; j++){
                //while(!isalpha(text[count])){
                //  count++;
                //}
                dtext[j][k] = text[count];
                //printf("indexJ: %d indexK: %d text: %c\n",j,k,dtext[j][k]);
                count++;
            }
            
        }
        count = 0;
        
        /*
         printf("dtext:\nnum_key: %d\n",num_key);
         for(int jj=0; jj<num_key&&count<TEXT_SIZE;jj++){
         for(int kk = 0; kk< STL; kk++ ){
         printf("%c",dtext[jj][kk]);
         }
         printf("(END)\n");
         }
         
         for (int pp = 0; pp<num_key; pp++){
         dtext[pp][strlen(dtext[pp])]='\0';
         }
         */
        
        
        int counts[num_key][26];
        memset(counts,0, sizeof(counts));
        
        for(int l = 0; l < num_key; l++){ // to calutlate the frequency of every alphabet for each subtext
            //for(int m = 0; m < num_key; m++ ){
            for(int nn = 0; nn<strlen(dtext[l]); nn++){
                
                if(isalpha(dtext[l][nn])){
                    counts[l][(int)(dtext[l][nn]-'A')]+=1;//storage in alphabet order
                }
                //char cha = dtext[l][n];
                //int cindex = (int)dtext[l][n]-'A';
                //printf("subtext:%d index:%d index:%d alpha:%c frequency:%d\n",l,n,cindex,cha,abc);
            }
            //}
        }
        
        char counts2[num_key][26];
        memset(counts2,0, sizeof(counts2));
        
        // printf("index: ");
        for(int i = 0; i<num_key; i++){
            for(int j = 0; j < 26; j++){
                //printf("Frequency: %d\n",counts[i][j]);
                int index = findMax(counts[i]); //find index of the max(frequence) in count[l] array
                //char txt = j+'A';
                //printf("SubText：%d Alpha：%c，Frequency：%d\n",i,txt,counts[i][j]);
                counts2[i][index]=CHFREQ[j]; //make the fixed table
                //printf("%d ",index);
                //printf("char: %c changeTo: %c\n",index+'A',CHFREQ[j]);
            }
            //printf("\n");
        }
        //printf("\n");
        
        //printf("C2table: \n");
        
        //for(int i = 0; i<num_key; i++){
        //  for(int j = 0; j < 26; j++){
        
        //printf("%c",counts2[i][j]);
        //   }
        //printf("\n");
        //}
        
        //printf("\n");
        //MySorting(counts[l]);
        //printf("The plain text: ");
        int len = STL;
        for(int np = 0; np<len; np++){
            int l = 0;
            for(int m = 0; m < num_key; m++ ){
                if(isalpha(dtext[m][np])){
                    dtext[m][np] = counts2[l][(int)dtext[m][np]-'A'];//storage in alphabet order
                }
                l++;
                if(l==num_key){l=0;}
               
                printf("%c",dtext[m][np]);
                
            }
        }
        
        //  printf("\n");
        
        //printf("The cipher text：");
        //for(int i = 0; i<strlen(text);i++){
        //  printf("%c",text[i]);
        
        //  }
        // printf("\n");
    }
    return 0;
    // Your code here...
}
