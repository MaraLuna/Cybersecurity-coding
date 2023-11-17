/*Entropy of Passwords */


#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int approximate_alphabet(char *str);
double information_content(int alphabet_size, size_t length);

int main(){

    char str[101];
    printf("Enter the password string: ");
    scanf("%100s", str);

    int count = approximate_alphabet(str);
    return 0;
}
int approximate_alphabet(char *str){
    int length = 0;
    int alphabet = 0;
    int digits =10;
    int lower =26;
    int upper = 26;
    int symbols = 32;

    for (int i =0; str[i] != '\0'; ++ i){
        if (isdigit(str[i])){
            ++ length;
            alphabet = alphabet + digits;
            digits =0;
        }else if (islower(str[i])){
            ++ length;
            alphabet = alphabet + lower;
            lower =0;
        }else if (isupper(str[i])){
            ++length;
            alphabet = alphabet + upper;
            upper = 0;
        }else{
            ++length;
            alphabet = alphabet + symbols;
            symbols =0;
        }
        }
    printf("Approximate alphabet:  %d\n", alphabet);
    printf("Length: %d\n", length);
    information_content (alphabet,length);
    return 0;
    }

double information_content(int alphabet_size, size_t length){
    float entropy;
    entropy = length * (log2(alphabet_size));
    printf("Information Content Entropy: %.2f\n",(entropy));
    return (entropy);
}

