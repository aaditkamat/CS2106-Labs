/*************************************
* Lab 1 Exercise 3
* Name     : Aadit Kamat
* Matric No: A0164761B  
* Lab Group: 02
*************************************/

#include <stdio.h>

//Datatype Declarations
typedef int (*arithFuncPtr)(int, int);


//Function Prototypes
int lumos(int x, int y);
int alohomora(int x, int y);
int expelliarmus(int x, int y);
int sonorus(int x, int y);
typedef struct SPELL {
    char name[13];
    arithFuncPtr func;
} spell;

int main()
{
    int a, b, optype, res;
    spell types[4] = {{"lumos", lumos}, {"alohomora", alohomora}, {"expelliarmus", expelliarmus}, {"sonorus", sonorus}};

    while (scanf("%i %i %i", &optype, &a, &b) == 3) {
        arithFuncPtr func = types[optype - 1].func;
        res = (*func)(a, b);
        printf("%s %i\n", types[optype - 1].name, res);
    }
   
    return 0;
}

int alohomora(int x, int y)
{
    if (x == 0)
        return y;
    if (y == 0)
        return x;
    return alohomora(y, x % y);
}

int sonorus(int x, int y) {
    int result = 1;
    for (int i = 1; i <= y; i++) {
        result *= x;
    }
    return result;
}

int lumos(int x, int y) {
    int result = 0;
    for (int i = x; i <= y; i++) {
        result += i;
    }
    return result;
}

int expelliarmus(int x, int y) {
    while (x % y == 0) {
        x /= y;
    }
    return x;
}
