//
// Created by Marco Siracusa on 3/7/19.
//

struct s1 {
    double d1;
    int i1;
    //short as1[3];
    //float *pf1;
};

struct s2 {
    int i1;
    //float *pf1;
    //short as1[3];
    double d1;
};

int f1(struct s1 *s11) {

    struct s1 s12;
    s12.i1 = s11->i1;
    s12.d1 = s11->d1;

    return s12.i1;
}

int kernel(struct s1 *s11, struct s2 *s21, int *s11i1, struct s1 *s12, struct s1 *s13) {

    s11->i1 = s12->i1 + s13->i1 + *s11i1;
    f1(s11);
    f1(s12);
    return s11->i1;
}

int main(void) {

    struct s1 s11;
    struct s1 s12;
    s11.d1 = 123e+20;
    s11.i1 = 11;
    //s11.as1[0] = 0;
    //s11.as1[1] = 1;
    //s11.as1[2] = 2;
    //float bf1 = 1.1;
    //s11.pf1 = &bf1;

    struct s2 s21;
    s21.d1 = 123e+20;
    s21.i1 = 11;
    //s21.as1[0] = 0;
    //s21.as1[1] = 1;
    //s21.as1[2] = 2;
    //float bf2 = 1.1;
    //s21.pf1 = &bf2;

    s12 = s11;

    return kernel(&s11, &s21, &s11.i1, &s12, &s11);
}