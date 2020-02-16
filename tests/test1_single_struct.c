//
// Created by Marco Siracusa on 2/27/19.
//

struct s1 {
    double d1;
    int i1;
    short as1[3];
    //float pf1;
};

int kernel(struct s1 s11) {
    return ++s11.i1;
}

int x() {
    return 3;
}
int main(void) {

    int i1 = 12;
    int i2 = 12;
    int i3 = 12;
    struct s1 s11;
    s11.d1 = 123e+20;
    s11.i1 = x();
    s11.as1[0] = 0;
    s11.as1[1] = 1;
    s11.as1[2] = 2;
    float bf1 = 1.1;
    //s11.pf1 = bf1;
    
    loop1: for( int i = 0; i < 10; i++) {
        i1++;
    }

    return kernel(s11);
}