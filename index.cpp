#include<bits/stdc++.h>
using namespace std;

class Animal{
    public:

    string breed;
    int age;
    string ability;
    static int legs;
    Animal() {
        this->breed="unknown";
        this->age=0;
        this->ability="unknown";
    }

    Animal(string breed, int age, string ability) {
        this->breed=breed;
        this->age=age;
        this->ability=ability;
    }

    virtual void sound() {
        cout<<" i animal you fool"<<endl;
    }

    void setLegs(int leg) {
        Animal::legs=leg;
    }

};

class Human: public Animal {
    public:
    
    string name;
    int age;

    Human(){
        this->name="unknown";
        this->age=0;
    }

    Human(string name, int age) : Animal(name, age, "human") {
        this->name=name;
        this->age=age;
    }

    void sound() {
        cout<<" i am human you fool"<<endl;
    }

    static void sayHello() {
        cout<<"hello sarvesh"<<endl;
    }

};


int Animal::legs=10; //defining the default value for static member;

int main() {
    Animal *cow= new Animal("cow", 10, "milk");
    cout<<cow->breed<<" "<<cow->age<<" "<<cow->ability<<" "<<cow->legs<<endl;

    cow->setLegs(4);

    Human *human= new Human("human", 22);
    human->setLegs(2);

    human->sound();
    cow->sound();

    cout<<endl<<endl;


    // runtime polymorphism
    Animal *cow2;
    Human human2;
    cow2= &human2;

    human2.sound();
    cow2->sound();


    Human::sayHello();
}