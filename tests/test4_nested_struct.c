//
// Created by Fabrizio Ferrandi on 3/12/19.
//

struct s1 {
    double d1;
    int i1;
    int i2;
    int o1;
};

struct s3 {
    int i1;
    struct s1 s11;
};

int kernel(struct s3 *s31) {

    s31->i1 = s31->i1 + s31->s11.i1;
    return s31->i1;
}

int main(void) {

    struct s3 s31;
    s31.i1 = 1;
    s31.s11.i1 = 11;
    s31.s11.d1 = 123e+10;
    int res1 = kernel(&s31);

    return s31.i1;
}
