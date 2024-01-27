#include<bits/stdc++.h>
using namespace std;


class Flight{
    public: 
    static int ticketId;
    int getTicketId() {
        return ++ticketId;
    }

    int flightId;
    int seatCount;
    int ticketCost;

    vector<int> ticketIds;
    vector<int> passangerIds;
    vector<int> numberOfSeatsBooked;

    Flight(int flightId) {
        this->flightId=flightId;
        this->seatCount=50;
        this->ticketCost=5000;
    }

    bool bookTicket(int passangerId, int numberOfSeats) {
        if(this->seatCount - numberOfSeats < 0 ) {
            cout<<"Avaliable Seat: "<<seatCount<<", Required Seat: "<<numberOfSeats<<" -> booking not possible"<<endl;
            return false;
        }
        this->seatCount-=numberOfSeats;
        this->ticketCost+=200*numberOfSeats;

        this->ticketIds.push_back(getTicketId());
        this->passangerIds.push_back(passangerId);
        this->numberOfSeatsBooked.push_back(numberOfSeats);
        cout<<"Booked succefully"<<endl;
        return true;
    }

    bool cancelTicket(int passangerId) {
        int refundAmount=0;

        //get the index
        auto passangerIterator= std::find(passangerIds.begin(), passangerIds.end(), passangerId);
        if(passangerIterator==passangerIds.end()) {
            cout<<"passanger Id: "<< passangerId<< "not found in flight "<<flightId<<endl;
            return false;
        }

        int index= passangerIterator-passangerIds.begin();

        ticketIds.erase(find(ticketIds.begin(), ticketIds.end(), ticketIds[index]));
        passangerIds.erase(find(passangerIds.begin(), passangerIds.end(), passangerIds[index]));

        int seatBooked= this->numberOfSeatsBooked[index];
        this->numberOfSeatsBooked.erase(find(numberOfSeatsBooked.begin(), numberOfSeatsBooked.end(), numberOfSeatsBooked[index]));


        this->ticketCost-= seatBooked * 200;
        this->seatCount+=seatBooked;
        cout<<"cancelled succefully"<<endl;

        return true;
    }

    void printDetails() {
        cout<<"flight Id:" <<flightId<<" --  Remaining ticket: "<<seatCount<<" -- current ticket price: "<<ticketCost<<endl;
        for(int i=0;i<ticketIds.size();i++) {
            cout<<"passangerId: "<<passangerIds[i]<<" -- number of Ticket Booked: "<<numberOfSeatsBooked[i]<<" -- total cost "<<5000*numberOfSeatsBooked[i]<<endl;
        }
    }
};

int Flight::ticketId=0;

int main() {
    unordered_map<int, Flight*> flights;

    for(int i=1;i<=2;i++) {
        flights[i]=new Flight(i);
    }

    int passangerId=1;

    while(true) {
        cout<<"Enter flight id:"<<endl;
        int flighNumber; cin>>flighNumber;

        if(flighNumber>flights.size() || flighNumber<=0) {
            cout<<"Invalid flight number"<<endl;
            continue;
        }

        flights[flighNumber]->printDetails();
        cout<<endl;

        cout<<"Enter your choice \n 1. Book \t 2. Cancel \t 3.Print"<<endl;
        int choice; cin>> choice;

        switch (choice){
        case 1:
            cout<<"enter number of tickets: ";
            int numberOfTicets; cin>>numberOfTicets;

            flights[flighNumber]->bookTicket(passangerId, numberOfTicets);
            passangerId++;
            break;
        case 2: 
            cout<<"enter passanger id";
            int passangerId; cin>>passangerId;
            flights[flighNumber]->cancelTicket(passangerId);
            break;
        case 3:
            flights[flighNumber]->printDetails();
            break;
        default: 
            return 1;

    }



    //get flightName, passangerId, Seats

}
}