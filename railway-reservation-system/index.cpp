#include<bits/stdc++.h>
using namespace std;

class Seat{
    public:
    int seatNo;
    string seatType;

    vector<pair<char,char>> currSeatBookedFromTo; // { {'A', 'C'}, {'C', 'E'} } => current seat is booked from A->C && C->E

    Seat(int seatNo, string seatType) {
        this->seatNo=seatNo;
        this->seatType=seatType;
        currSeatBookedFromTo={};
    }
    void bookThisSeat(char from, char to) {
        currSeatBookedFromTo.push_back({from, to});
    }

    bool cancelThisSeat(char from, char to) {
        vector<pair<char,char>> temp;
        for(auto it: currSeatBookedFromTo) {
            if(it.first==from && it.second==to) continue;
            else temp.push_back({it.first, it.second});
        }
        currSeatBookedFromTo=temp;
        return true;
    }
};

class Ticket{
    public:
    static int cnt;
    static int getTicketNo() {
        return cnt++;
    }
    
    char from;
    char to;
    int ticketId;
    string ticketType;
    int numberOfPeoples;
    vector<Seat*> bookedSeats;
    vector<Seat*> waitingSeats;

    Ticket(char from, char to, string ticketType, int nop, vector<Seat*>& bookedSeats, vector<Seat*>& waitingSeats) {
        this->ticketId=Ticket::getTicketNo();
        this->from=from;
        this->to=to;
        this->ticketType=ticketType;
        this->numberOfPeoples=nop;
        this->bookedSeats=bookedSeats;
        this->waitingSeats=waitingSeats;
    }
};

class Train{
    public: 
    static int cnt;
    static int getTrainNo() {
        return cnt++;
    }

    static vector<int> cnfSeatsIds; //just for to store the confirmed Seats
    static vector<int> wlSeatsIds; // just to store the id of waiting seats

    int trainNo;
    char from;
    char to;
    vector<char> stops;

    unordered_map<int, Seat*> totalAvailableSeats;
    unordered_map<int, Seat*> totalWaitingSeats;
    unordered_map<int, Ticket*> totalTickets; //pnr with ticket link;

    Train() {
        this->trainNo=Train::getTrainNo();
        this->from='A';
        this->to='E';
        stops={'A','B','C', 'D', 'E'};

        for(int i: cnfSeatsIds) {
            totalAvailableSeats[i]= new Seat(i, "CNF");
        }
        for(int i: wlSeatsIds) {
            totalWaitingSeats[i]= new Seat(i, "WL");
        }
    }

    private: 
    Seat* bookSingleSeat(char from, char to) { //books 1 person seat at a time;
        for(auto it: totalAvailableSeats) {
            int seatNo = it.first;
            Seat* st   =it.second;
            
            //check if current seat available from-to or not
            vector<pair<char,char>> currSeatBookedFromTo= st->currSeatBookedFromTo;
            if(currSeatBookedFromTo.size() > 0) {
                bool canIBook= true;
                for(auto currSeat: currSeatBookedFromTo) { //partially booking taking care of
                    if(from>=currSeat.first && to<=currSeat.second) {
                        canIBook=false;
                        break;
                    }
                }
                if(canIBook) {
                    st->bookThisSeat(from, to);
                    return st;
                }
            } else {
                st->bookThisSeat(from, to);
                return st;
            }
        }

        for(auto it: totalWaitingSeats) {
            int seatNo = it.first;
            Seat* st   =it.second;
            
            //check if current seat available from-to or not
            vector<pair<char,char>> currSeatBookedFromTo= st->currSeatBookedFromTo;
            if(currSeatBookedFromTo.size() >0 ) { 
                bool canIBook= true;
                for(auto currSeat: currSeatBookedFromTo) { //partially booking taking care of
                    if(from>=currSeat.first && to<=currSeat.second) {
                        canIBook=false;
                        break;
                    }
                }
                if(canIBook) {
                    st->bookThisSeat(from, to);
                    return st;
                }
            } else {
                st->bookThisSeat(from, to);
                return st;
            }
        }
        return nullptr;
    }

    public:
    //! partial booking available
    Ticket* bookSeat(char from, char to, int numberOfPeoples) { // books more than 1 seat;
        vector<Seat*> cnfSeats;
        vector<Seat*> wlSeats;

        while(numberOfPeoples--) {
            Seat* s= bookSingleSeat(from, to);
            if(s==nullptr) { 
                //! ask if some seats booked either CNF/WL and rest seat are not available what should I do?
                //! no seats available CNF & WL both are filled write a logic to revert back booked ticket
                // TODO roll back logic to be implemented if required;

                cout<<"!!!!!!!!! Ops looks like some seat are failing to book the ticket !!!!!! \n\n"; 
                return nullptr;
            } else if(s->seatType=="CNF") {
                cnfSeats.push_back(s);
            } else {
                wlSeats.push_back(s);
            }
        }

        string ticketType= "";
        if(cnfSeats.size()>0 && wlSeats.size()>0) {
            ticketType="PCNF";
        } else if(cnfSeats.size()==0 && wlSeats.size()==0) {
            ticketType="NOT_BOOKED";
        } else if(cnfSeats.size()==0) {
            ticketType="WL";
        } else {
            ticketType="CNF";
        }

        if(ticketType=="NOT_BOOKED") {
            return nullptr;
        }

        Ticket* currTicket= new Ticket(from, to, ticketType, numberOfPeoples, cnfSeats, wlSeats);
        totalTickets[currTicket->ticketId] = currTicket;
        return currTicket;

    }

    void confirmWaitingTickets() {

        for(auto it: totalTickets) {
            int ticketId= it.first;
            Ticket* ticket= it.second;

            if(ticket->ticketType=="CNF") continue;

            vector<Seat*> ticketWaiting= ticket->waitingSeats;

            vector<Seat*> newWaitingSeats;


            for(int i=0;i<ticketWaiting.size();i++) {
                Seat* currSeat= ticketWaiting[i];
                Seat* confirmedSeat= bookSingleSeat(ticket->from, ticket->to);

                if(confirmedSeat) { //seat has been booked;
                    currSeat->cancelThisSeat(ticket->from, ticket->to); //free the waiting least seat
                    ticket->bookedSeats.push_back(confirmedSeat);
                } else{
                    newWaitingSeats.push_back(currSeat);
                }
            }

            ticket->waitingSeats=newWaitingSeats;
            if(newWaitingSeats.size()==0){
                ticket->ticketType="CNF";
            }
        }
    }

    //! partial cancelation available;
    bool cancelTicket(Ticket* ticket, vector<int> cancelOnlyTheseSeats) {
        if(ticket==nullptr) return false;

        // char from;
        // char to;
        // int ticketId;
        // string ticketType;
        // int numberOfPeoples;
        // vector<Seat*> bookedSeats;
        // vector<Seat*> waitingSeats;
        vector<Seat*> newBookedSeats;
        vector<Seat*> newWaitingSeats;

        for(Seat* singleBookedSeat: ticket->bookedSeats) {
            int currSeatId= singleBookedSeat->seatNo;

            auto isCancelable= false;

            //check can i cancel this seat or not;
            for(int i: cancelOnlyTheseSeats) {
                if(i==currSeatId) isCancelable=true;
            }

            if(!isCancelable) {
                newBookedSeats.push_back(singleBookedSeat);
                continue;
            }

            singleBookedSeat->cancelThisSeat(ticket->from, ticket->to);
            // remove this SeatId from ticket->bookedSeats;
            
        }

        for(Seat* singleBookedSeat: ticket->waitingSeats) {
            int currSeatId= singleBookedSeat->seatNo;

            auto isCancelable= false;

            //check can i cancel this seat or not;
            for(int i: cancelOnlyTheseSeats) {
                if(i==currSeatId) isCancelable=true;
            }

            if(!isCancelable) {
                newWaitingSeats.push_back(singleBookedSeat);
                continue;
            }
            
            singleBookedSeat->cancelThisSeat(ticket->from, ticket->to);
            // remove this SeatId from ticket;

        }

        //delete the ticket if no seat available;
        if(newBookedSeats.size()==0 && newWaitingSeats.size()==0) 
            delete totalTickets[ticket->ticketId];
        else {
            ticket->bookedSeats=newBookedSeats;
            ticket->waitingSeats=newWaitingSeats;
        }

        //!removed a booked ticket now time to book all the waiting list tickets
        confirmWaitingTickets();

        return true;
    }

    void printChart() { //print the chart of current train;
        cout<<" "<<"  ";
        for(char i='A';i<='E';i++) cout<<i<<" ";
        cout<<endl;

        for(auto it: totalAvailableSeats) {
            int seatId=it.first;
            Seat* seat= it.second;

            cout<<seatId<<"  ";
            map<char, int> mp;
            mp['A']=0;
            mp['B']=0;
            mp['C']=0;
            mp['D']=0;
            mp['E']=0;


            vector<pair<char,char>> currSeatBookedFromto= seat->currSeatBookedFromTo;
            for(auto [from, to]: currSeatBookedFromto) {
                for(char i=from;i<=to;i++) {
                    mp[i]++;
                }
            }

            for(auto it: mp) {
                if(it.second>0) cout<<"* ";
                else cout<<"- ";
            }
            cout<<endl;
        }

        cout<<"WL start"<<endl;

        for(auto it: totalWaitingSeats) {
            int seatId=it.first;
            Seat* seat= it.second;

            cout<<seatId<<"  ";
            map<char, int> mp;
            mp['A']=0;
            mp['B']=0;
            mp['C']=0;
            mp['D']=0;
            mp['E']=0;


            vector<pair<char,char>> currSeatBookedFromto= seat->currSeatBookedFromTo;
            for(auto [from, to]: currSeatBookedFromto) {
                for(char i=from;i<=to;i++) {
                    mp[i]++;
                }
            }

            for(auto it: mp) {
                if(it.second>0) cout<<"* ";
                else cout<<"- ";
            }
            cout<<endl;
        }
    } 


};

vector<int> Train::cnfSeatsIds= {1,2,3,4,5,6,7,8};
vector<int> Train::wlSeatsIds= {9,10};
int Train::cnt=1;
int Ticket::cnt=1;


int main() {
    Train* t= new Train();

    // Ticket * t1= t->bookSeat('B','E', 10);
    Ticket * t2= t->bookSeat('A', 'B', 10);
    Ticket * t3= t->bookSeat('B', 'E', 3);
    
    cout<<t2->ticketType<<" "<<t3->ticketType<<endl;

    t->cancelTicket(t2, {7, 8,6});

    cout<<t2->ticketType<<" "<<t3->ticketType<<endl;

    // Ticket * t1= t->bookSeat('A','B', 8);

    // cout<<<<endl;
    // cout<<t1->bookedSeats[0]->seatNo<<' '<<t1->bookedSeats[1]->seatNo<<endl;

    // t->cancelTicket(t1, {t1->bookedSeats[0]->seatNo, t1->bookedSeats[1]->seatNo});
    // cout<<'a'<<endl;
    // t->bookSeat('B', 'E', 2);
    // Ticket * t2= t->bookSeat('B', 'E', 3);
    // Ticket * t3= t->bookSeat('A','B', 1);

    // Ticket * t4= t->bookSeat('B','E', 6);


    

    // cout<<t1->ticketType<<" "<<t2->ticketType<<" "<<t3->ticketType<<" ";
    // Ticket * t5= t->bookSeat('A','B', 1);
    
    // if(t5==nullptr) {
    //     cout<<"TICKET NOT BOOKED";
    // } else {
    //     cout<<t5->ticketType<<" ";
    // }

    // cout<<"\n\n\n"<<endl;


    t->printChart();

}