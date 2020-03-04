#include <stdio.h>

struct s1 {
    double d1;
    int i1;
    short short1[3];
};

int fun1(struct s1 stct) {
    return stct.i1++;
}

int func2() {
    return 3;
}

int main(void) {

    int i1 = 12;
    int i2 = 12;
    int i3 = 12;
    struct s1 stct;
    stct.d1 = 123e+20;
    stct.i1 = func2();
    stct.short1[0] = 0;
    stct.short1[1] = 1;
    stct.short1[2] = 2;
    float float1 = 1.1;
    
    for( int i = 0; i < 10; i++) {
        i1++;
    }
	
    return func1(stct);
}
