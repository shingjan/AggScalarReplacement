//
// Created by Marco Siracusa on 3/7/19.
//

struct s1 {
    double d1;
    int i1;
    //short as1[3];
    //float *pf1;
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

    //struct s3 s32;

    //s32 = s31;

    return kernel(&s31);// + kernel(&s31);
}