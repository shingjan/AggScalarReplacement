#include <stdio.h>

struct s1 {
    double d1;
    int i1;
};

struct s2 {
    int i1;
    double d1;
};

int func1(struct s1 *s11) {

    struct s1 s12;
    s12.i1 = s11->i1;
    s12.d1 = s11->d1;

    return s12.i1;
}

int func2(struct s1 *s11, struct s2 *s21, int *s11i1, struct s1 *s12, struct s1 *s13) {

    s11->i1 = s12->i1 + s13->i1 + *s11i1;
    func1(s11);
    func1(s12);
    return s11->i1;
}

int main(void) {

    struct s1 s11;
    struct s1 s12;
    s11.d1 = 123e+20;
    s11.i1 = 11;

    struct s2 s21;
    s21.d1 = 123e+20;
    s21.i1 = 11;
    s12 = s11;

    return func2(&s11, &s21, &s11.i1, &s12, &s11);
}
