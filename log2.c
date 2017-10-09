#include <stdio.h>

int stage (int n) {
    int pot = 1, a = 0; // 2
    
    while (pot < n) { // 1
        a ++; // lg n
        pot *= 2; // lg n        
    }
    return a; //1
}//T(n) = 2 * lg + 4

void main() {
    int n, r;
    r = scanf("%d", &n);
    
    printf("Estágios = %d\n", stage(n));
}