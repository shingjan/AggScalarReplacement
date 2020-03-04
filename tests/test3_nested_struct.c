#include <stdio.h>

struct s1 {
    double d1;
    int i1;
};

struct s3 {
    int i1;
    struct s1 s11;
};

int func1(struct s3 *s31) {

    s31->i1 = s31->i1 + s31->s11.i1;
    return s31->i1;
}

int main(void) {

    struct s3 s31;
    s31.i1 = 1;
    s31.s11.i1 = 11;
    s31.s11.d1 = 123e+10;

    return func1(&s31);
}
