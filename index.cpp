#include<bits/stdc++.h>
using namespace std;

class Vehical {
    public:
    static int cnt;
    void virtual sound() {
        cout<<" i am vehical "<<cnt<<endl;
    }
};
int Vehical::cnt=10;

class TwoWhiler: public Vehical{
    public:
    void sound() {
        cout<<" I am two wheler vehical"<<endl;
    }
};
class ThreeWhiler: public Vehical{
    public:
    void sound() {
        cout<<" I am three wheler vehical"<<endl;
    }
};

int main() {
    Vehical* a= new Vehical();
    TwoWhiler* b= new TwoWhiler();
    ThreeWhiler* c= new ThreeWhiler();
    
    a->sound();
    b->sound();
    c->sound();
    
    cout<<endl<<endl;

    a= new TwoWhiler();
    a->sound();
    b->sound();
    c->sound();

}