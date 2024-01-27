/*
There is n number of taxies. For simplicity, assume 4. But it should work for any number of taxies<

    1. They are 6 points(a,b,c,d,e,f)
    2. All the points are in a straight line, and each point is 15kms away from the adjacent points
    3. Call Taxi takes 60 mins to travel from one point to another
    4. Each taxi charges Rs.100 minimum for the first 5 kilometers and Rs.10 for the subsequent kilometers
    5. For simplicity, time can be entered as absolute time. Ex: 9hrs, 15hrs, etc
    6. All taxies are initially stationed at A

    7. When a customer books a Taxi, a free taxi at that point is allocated
    8. If no free taxi is available at that point, a free taxi at the nearest point is allocated
    9. If two taxies are free at the same point, one with lower earning is allocated
    10. Note that the taxi only charges the customer from the pickup point to the drop point. Not the distance it travels from an adjacent point to pick up the customer
    
    11. If no taxi is free at that time, booking is rejected

*/
#include<bits/stdc++.h>
using namespace std;

class JourneyDetails {
    public:
    static int id;
    static int getId() {
        return ++id;
    }

    int bookingId;
    int customerId;
    int taxiNumber;
    char pickupPoint;
    char dropPoint;
    int pickupTime;
    int dropTime;
    int earnings;
    
    JourneyDetails(int customerId, int taxiNumber, char pickupPoint, char dropPoint, int pickupTime, int dropTime, int earnings) {
        this->bookingId=getId();
        this->customerId=customerId;
        this->taxiNumber=taxiNumber;
        this->pickupPoint=pickupPoint;
        this->dropPoint=dropPoint;
        this->pickupTime=pickupTime;
        this->dropTime=dropTime;
        this->earnings=earnings;
    }
};

class Taxi {
    public:

    static int id;
    static int getId() {
        return ++id;
    }

    int taxiNumber;
    char currPosition;
    int freeTime;
    int totalEarned;

    //all the journey travelled by this taxi;
    vector<JourneyDetails*> taxiJourneyDetails;


    Taxi() {
        this->taxiNumber=getId();
        this->currPosition='A';
        this->freeTime=0;
        this->totalEarned=0;
    }

    // void updateThisTaxiDetails(char newPos, int freeTime, int totalEarned) {
    //     this->currPosition=newPos;
    //     this->freeTime=freeTime;
    //     this->totalEarned=totalEarned;
    // }

    void bookThisTaxi(int customerId, char pickupPoint, char dropPoint, int pickupTime) {
        int droptime = pickupTime + abs(pickupPoint-dropPoint); //in hour
        int totalDistance= abs(pickupPoint-dropPoint) * 15; //total distance in km;
        int earnings= 100 + (totalDistance-5)*10 ; //100 first 5 km 10rs/km for rest km

        JourneyDetails* currJourneyDetails=new JourneyDetails(customerId, this->taxiNumber, pickupPoint, dropPoint, pickupTime, droptime, earnings);
        taxiJourneyDetails.push_back(currJourneyDetails);

        //updating the current taxi details after reaching the destination;
        this->currPosition=dropPoint;
        this->freeTime=droptime;
        this->totalEarned+=earnings;

    }

};

int Taxi::id=0;
int JourneyDetails::id=0;


// booking condition;
/*
    7. When a customer books a Taxi, a free taxi at that point is allocated
    8. If no free taxi is available at that point, a free taxi at the nearest point is allocated
    9. If two taxies are free at the same point, one with lower earning is allocated

    //! catch 
    10. Note that the taxi only charges the customer from the pickup point to the drop point. Not the distance it travels from an adjacent point to pick up the customer
    
    11. If no taxi is free at that time, booking is rejected
*/ 

void bookTaxi(int customerId, char pickupPoint, char dropPoint, int pickupTime, map<int, Taxi*> &taxiDetails) {
    // check for free taxi and allocate if there is;
    bool isTaxiBooked=false;

    for(auto &[taxiNo, taxi]: taxiDetails) {
        int dis= abs(taxi->currPosition-pickupPoint) * 15; //pickupPoint - taxi's position distance;
        int taxiTimeTaken= dis/15;

        int taxiWillBeAtPickupPointOn= taxi->freeTime + taxiTimeTaken;

        if(taxi->currPosition==pickupPoint && taxiWillBeAtPickupPointOn <= pickupTime) {
            taxi->bookThisTaxi(customerId, pickupPoint, dropPoint, pickupTime);
            isTaxiBooked=true;
            cout<<"taxi booked with Taxi No: "<<taxi->taxiNumber<<" from: "<<pickupPoint<<" to: "<<dropPoint<<" at Time: "<<pickupTime<<endl;
            break;
        }
    }
    if(isTaxiBooked) return;

    int minDistance= INT_MAX;
    vector<Taxi*> availableTaxis; //these taxi can be booked;

    for(auto &[taxiNo, taxi]: taxiDetails) {
        int dis= abs(taxi->currPosition-pickupPoint) * 15; //pickupPoint - taxi's position distance;
        int taxiTimeTaken= dis/15;

        int taxiWillBeAtPickupPointOn= taxi->freeTime + taxiTimeTaken;

        if(dis < minDistance && taxiWillBeAtPickupPointOn <= pickupTime) {
            availableTaxis.push_back(taxi);
            minDistance=dis;
        }
    }

    if(availableTaxis.size()>0) {
        sort(availableTaxis.begin(), availableTaxis.end(), [](Taxi* a, Taxi* b) {
            return a->totalEarned < b->totalEarned;
        });
        availableTaxis[0]->bookThisTaxi(customerId, pickupPoint, dropPoint, pickupTime);
        cout<<"Taxi No: "<<availableTaxis[0]->taxiNumber<<" from: "<<pickupPoint<<" to: "<<dropPoint<<" at Time: "<<pickupTime<<endl;
        isTaxiBooked=true;
    }

    if(isTaxiBooked) return;
    // check for taxi nearest to the point;

        // check if it is possible for taxi to come at that point;
        //  check if we have more that 1 taxi available then allocate taxi who has earning


    cout<<"No taxi booking possible"<<endl;
}

void printAllBookingDetails(map<int, Taxi*> &taxiDetails) {
    cout<<"BookingId \t CustomerId \t TaxiNo \t PickupPoint \t DropPoint \t PickupTime \t DropTime \t Earnings"<<endl;

    for(auto &[taxiNo, taxi]: taxiDetails) {
        for(JourneyDetails* s: taxi->taxiJourneyDetails) {
            cout<<s->bookingId<<" \t         "<<s->customerId<<" \t          "<<s->taxiNumber<<" \t         "<<s->pickupPoint<<" \t          "<<s->dropPoint<<" \t          "<<s->pickupTime<<" \t          "<<s->dropTime<<" \t          "<<s->earnings<<endl;
        }
        cout<<endl;
    }
}

void printTaxiDetails(map<int, Taxi*> &taxiDetails) {
    for(auto &[taxiNo, taxi]: taxiDetails ) {
        cout<<"-------------------------------------------"<<endl;
        cout<<"TaxiNo: \t"<<taxiNo<<"\t\t current position: "<<taxi->currPosition<<endl;
        cout<<"____________________________________________"<<endl;
        cout<<"BookingId \t CustomerId \t TaxiNo \t PickupPoint \t DropPoint \t PickupTime \t DropTime \t Earnings"<<endl;
        for(JourneyDetails* s: taxi->taxiJourneyDetails) {
            cout<<s->bookingId<<" \t         "<<s->customerId<<" \t         "<<s->taxiNumber<<" \t         "<<s->pickupPoint<<" \t         "<<s->dropPoint<<" \t         "<<s->pickupTime<<" \t         "<<s->dropTime<<" \t         "<<s->earnings<<endl;
        }
        cout<<"Total Earnings: "<<taxi->totalEarned<<endl;
    }
}

int main () {
    map<int, Taxi*> taxiDetails; 
    for(int i=1;i<=4;i++) { //4 taxi;
        taxiDetails[i]=new Taxi();
    }

    int customerId=1;
    cout<<"____________CALL TAXI BOOKING______________\n";
    while(true) {
        cout<<"1. Booking\n2. Booking details\n3. Taxi details\n4. Exit"<<endl;
        int choice;
        cin>>choice;

        switch (choice) {
        case 1:
            char pickupPoint, dropPoint;
            int time;
            cout<<"Enter PickUp point[a-f] && drop Point[a-f] && pickup time: "; 
            cin>>pickupPoint>>dropPoint>>time;

            bookTaxi(customerId, pickupPoint, dropPoint, time, taxiDetails);
            customerId++;
            break;
        case 2: 
            printAllBookingDetails(taxiDetails);
            break;
        case 3: 
            printTaxiDetails(taxiDetails);
            break;
        default:
            return 1;
        }
    }
}