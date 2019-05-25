
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define TEXT_SIZE 1000000 // Note, the longer the text the more likely you will get a good 'decode' from the start.
#define ALEN 26         // Number of chars in ENGLISH alphabet
#define CHFREQ "ETAONRISHDLFCMUGYPWBVKJXQZ" // Characters in order of appearance in English documents.
#define ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZ"


char upcase(char ch){
    if(islower(ch))
        ch -= 'a' - 'A';
    return ch;
}

int findMax(int countf[]){ // To find theMaxium index for current subtest and return it
    int max = -1;
    int index =0;
    int j = 0;
    for(; j<26; j++){
        if(max<countf[j]){
            max = countf[j];
            index = j;
        }
    }
    countf[index]=-2; // make it -2 so it will not be the maxium again for next time
    return index;
}


int main(int argc, char **argv){
    
    char* text = (char*)malloc(sizeof(char)*TEXT_SIZE+1);
    char ch;
    int n, i;
    
    if(argc > 1 && (n = atoi(argv[1])) > 0);
    else{ fprintf(stderr,"Malformed argument, use: crack [n], n > 0\n"); exit(-1);}
    
    for(i = 0, ch = fgetc(stdin); i < TEXT_SIZE && !feof(stdin); i++, ch = fgetc(stdin)){
        text[i] = (ch = (isalpha(ch)?upcase(ch):ch));
    }
    text[i] = '\0';
    for(int a = 0; a < n; a++){ // n is the number of key , a is number of key in using
        int num_key = a+1;
        int STL = (TEXT_SIZE/num_key); // set the subtext array length
        int count = 0; // the curcor to read through the cyther text
        char dtext[num_key][STL]; // create the subtext for 2d array
        memset(dtext,0, sizeof(dtext)); // initialize the 2d array

        for(int k = 0; k< STL; k++ ){
            for(int j=0; j<num_key&&count<TEXT_SIZE; j++){
                
                dtext[j][k] = text[count]; // put the cyther text into 2D array
                count++;
            }
            
        }
        count = 0;
        
        int counts[num_key][26]; // create a table to store the frequency for each array
        memset(counts,0, sizeof(counts));
        
        for(int l = 0; l < num_key; l++){ // to calutlate the frequency of every alphabet for each subtext
            
            for(int nn = 0; nn<strlen(dtext[l]); nn++){
                
                if(isalpha(dtext[l][nn])){
                    counts[l][(int)(dtext[l][nn]-'A')]+=1;//storage in alphabet order
                }
                
            }
        }
        
        char counts2[num_key][26]; //this table is for store the fixed table
        memset(counts2,0, sizeof(counts2));
        
        // printf("index: ");
        for(int i = 0; i<num_key; i++){
            for(int j = 0; j < 26; j++){
                int index = findMax(counts[i]);
                counts2[i][index]=CHFREQ[j]; //make the fixed table (get the char)
                
            }
        }
      
        int len = STL;
        for(int np = 0; np<len&&count<strlen(text); np++){
            int l = 0;
            for(int m = 0; m < num_key; m++ ){
                if(isalpha(dtext[m][np])){
                    dtext[m][np] = counts2[l][(int)dtext[m][np]-'A'];//storage in alphabet one by one by using the fixed table
                }
                l++;
                if(l==num_key){l=0;}
               
               
                printf("%c",dtext[m][np]); // print out the table which is fixed
                count++;
            }
        }
        
    }
    return 0;
    // Your code here...
}
