#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_NAME_SIZE 30
#define MAX_DATE_SIZE 15
#define MAX_TIME_SIZE 10
#define MAX_STATE_SIZE 15
#define MAX_SIZE 20

#define Passengers_file "passengers.txt"
#define Flights_file "flights.txt"
int FlightGlobalFlag=0;
int passengerGlobalFlag=0 ;

// struct passenger (single linked list)
struct Passenger {
    char name[MAX_NAME_SIZE];
    char passportNumber[MAX_SIZE];
    struct Passenger* Next;
};
//Struct flight node used in queues and linked list
struct Flight
{
    char flightID [MAX_SIZE] ;
    char state [MAX_STATE_SIZE] ;
    char date [MAX_DATE_SIZE] ;
    char time [MAX_TIME_SIZE] ;
    struct Passenger* passengersList;
    struct Flight* next ;
};

struct Queue
{
    struct Flight* front;
    struct Flight* rear;
};

//Passenger Linked List Prototypes
struct Passenger* MakeEmptyPassengerList(struct Passenger* L);
struct Passenger* FindPassenger(char passport[], struct Passenger* L);
struct Passenger* FindPreviousPassenger(char passport[], struct Passenger* L);
struct Passenger* FindLastPassenger(struct Passenger* L);
void InsertPassenger(char name[], char passport[], struct Passenger* L, struct Passenger* P);
void DeletePassenger(char passport[], struct Passenger* L);
void DeletePassengerList(struct Passenger* L);
bool IsEmpy(struct Passenger* L);
bool IsLast(struct Passenger* P);
void PrintListPassengers(struct Passenger* L);
int CountPassengers(struct Passenger* L);
void DisposePassengerLinkedList(struct Passenger * L) ;


//Flight Linked List Prototypes
struct Flight* MakeEmptyFlightList();
void InsertFlight(struct Flight* L, struct Flight* F);
bool IsEmpyFlight(struct Flight* L);
bool IsLastFlight(struct Flight* P);
void PrintList(struct Flight* L);
void DisposeFlightLinkedList(struct Flight* L);
struct Flight* FindFlight(char id[], struct Flight* L) ;

//Queue Prototypes
void initQueue(struct Queue* Q);
bool isEmptyQueue(struct Queue* Q);
void enqueueFlight(struct Queue* Q, struct Flight* F);
struct Flight* dequeueFlight(struct Queue* Q);
void printQueue(struct Queue* Q, char stateOfFlight[]);
struct Flight* findFlightInQueue(struct Queue* Q, char id[]);
struct Flight* FindFlightInAllQueue(struct Queue* arrivalQ, struct Queue* departureQ, struct Queue* emergencyQ, char id[]);
void DisposeQueue(struct Queue* Q);

//Project Functions Prototypes
void PrintMenu();
void ManagePassengersMenu();

//Flight Operations
void ReadFlights(struct Queue* arrivalQ, struct Queue* departureQ, struct Queue* emergencyQ);
void printFlightDetail(struct Queue* arrivalQ,struct Queue* departureQ,struct Queue* emergencyQ);
void AddNewFlight (struct Queue* arrivalQ,struct Queue* departureQ,struct Queue* emergencyQ);
void PrintPassengerOfAFlight(struct Queue* arrivalQ,struct Queue* departureQ,struct Queue* emergencyQ ,struct Flight* cancelledLinkedList, struct Flight* departedLinkedList ) ;
void LandFlight (struct Queue* arrivalQ,struct Queue* emergencyQ ) ;
void RemovePassengerFromFlight(struct Queue* arrivalQ,struct Queue* departureQ,struct Queue* emergencyQ) ;
void DepartFlight (struct Queue* departureQ , struct Flight* departedLinkedList) ;
void CancelFlight(struct Queue* departureQ, struct Flight* cancelledLinkedList) ;
int checkState(char state[]);

//passenger operation
void ReadPassengers(struct Queue* arrivalQ, struct Queue* departureQ, struct Queue* emergencyQ);
void AddPassengerToFlight (struct Queue* arrivalQ,struct Queue* departureQ,struct Queue* emergencyQ ) ;
void RemovePassengerFromFlight(struct Queue* arrivalQ,struct Queue* departureQ,struct Queue* emergencyQ) ;
void DepartFlight (struct Queue* departureQ , struct Flight* departedLinkedList) ;
void CancelFlight (struct Queue* departureQ , struct Flight* cancelledLinkedList );
void DisplayStatus (struct Queue* arrivalQ,struct Queue* departureQ,struct Queue* emergencyQ ,struct Flight* cancelledLinkedList, struct Flight* departedLinkedList ) ;


int main()
{
    struct Queue arrivalQ, departureQ, emergencyQ;
    initQueue(&arrivalQ);
    initQueue(&departureQ);
    initQueue(&emergencyQ);
    struct Flight* departedLinkedList = MakeEmptyFlightList();
    struct  Flight* cancelledLinkedList=MakeEmptyFlightList();

    int choice;
    printf("\nWelcome to an airports air traffic control\n");

    do {
        PrintMenu();
        printf("\nPlease enter your Choice:");

        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            ReadFlights(&arrivalQ, &departureQ, &emergencyQ);
            ReadPassengers(&arrivalQ, &departureQ, &emergencyQ);
            break;
        case 2:
            printQueue(&emergencyQ, "Emergency Flights");
            printQueue(&arrivalQ, "Arrival Flights");
            printQueue(&departureQ, "Departure Flights");
            break;
        case 3:
            printFlightDetail(&arrivalQ, &departureQ, &emergencyQ);
            break;
        case 4:
            AddNewFlight(&arrivalQ, &departureQ, &emergencyQ);
            break;
        case 5:
            LandFlight(&arrivalQ, &emergencyQ);
            break;
        case 6:
            DepartFlight(&departureQ, departedLinkedList);
            break;
        case 7:
           CancelFlight( &departureQ ,cancelledLinkedList );
            break;
        case 8:
             DisplayStatus (&arrivalQ,&departureQ,&emergencyQ ,cancelledLinkedList, departedLinkedList ) ;

            break;
        case 9:
        {
            char managePassengerChoice;
            ManagePassengersMenu();
            printf("\nPlease enter your Choice: ");
            scanf(" %c", &managePassengerChoice);
            switch (managePassengerChoice)
            {
                case 'a':
                    PrintPassengerOfAFlight(&arrivalQ, &departureQ, &emergencyQ ,cancelledLinkedList,departedLinkedList );
                    break;
                case 'b':
                    AddPassengerToFlight(&arrivalQ, &departureQ, &emergencyQ);
                    break;
                case 'c':
                    RemovePassengerFromFlight(&arrivalQ, &departureQ, &emergencyQ);
                    break;
                case 'd':
                    break;
                default:
                    printf("\nEnter A Valid Choice Please.\n");
            }
            break;
        }
            case 10:
            printf("\nThank You for using Our System, GoodBye!\n");
            break;
        default:
            printf("\nInvalid choice, please try again.\n");
        }
    } while (choice != 10);
    DisposeQueue(&arrivalQ);
    DisposeQueue(&departureQ);
    DisposeQueue(&emergencyQ);

    DisposeFlightLinkedList(cancelledLinkedList);
    DisposeFlightLinkedList(departedLinkedList);

    return 0;
}

// Queu operation
void initQueue(struct Queue* Q) {
    Q->front = NULL;
    Q->rear = NULL;
}

bool isEmptyQueue(struct Queue* Q) {
    return (Q->front == NULL);
}

void enqueueFlight(struct Queue* Q, struct Flight* flightInput) {

    flightInput->next = NULL;

    if (Q->rear == NULL) {
        Q->front = flightInput;
        Q->rear = flightInput;
    }
    else {
        Q->rear->next = flightInput;
        Q->rear = flightInput;
    }
}
struct Flight* dequeueFlight(struct Queue* Q) {

    if (isEmptyQueue(Q)) {
        return NULL;
    }
    struct Flight* temp = Q->front;

    Q->front = Q->front->next;

    if (Q->front == NULL) {
        Q->rear = NULL;
    }
    temp->next = NULL;
    return temp;
}

void printQueue(struct Queue* Q , char stateOfFlight[]) {
        printf("\n%s:\n" , stateOfFlight) ;
    if (isEmptyQueue(Q)) {
        printf("%s is Empty\n" ,stateOfFlight );
        return;
    }
    struct Queue tempQ;//to store original queue
    initQueue(&tempQ);

    struct Flight* F;

    while ((F = dequeueFlight(Q)) != NULL) {
        printf("Flight: %s \t State: %s \t Date: %s \t Time: %s\n", F->flightID, F->state, F->date, F->time);
        printf("number of passengers:%d\n" , CountPassengers(F->passengersList)) ;
        enqueueFlight(&tempQ, F);
    }
    while ((F = dequeueFlight(&tempQ)) != NULL) {
        enqueueFlight(Q, F);
    }
}
void DisposeQueue(struct Queue* Q)
{
    struct Flight* F;
    while (!isEmptyQueue(Q)) {
        F = dequeueFlight(Q);
        DeletePassengerList(F->passengersList);
        free(F->passengersList);
        free(F);
    }
    Q->front = NULL;
    Q->rear = NULL;
}

struct Flight* createFlight(char id[], char state[], char date[], char time[]){
    struct Flight* F = (struct Flight*)malloc(sizeof(struct Flight));

    if (F == NULL){
        printf("Out of memory!\n");
        return NULL;
    }
    strcpy(F->flightID, id);
    strcpy(F->state, state);
    strcpy(F->date, date);
    strcpy(F->time, time);

    F->passengersList = MakeEmptyPassengerList(NULL);
    F->next = NULL;
    return F;
}

//linked list (passenger)
int CountPassengers(struct Passenger* L) {
    int count = 0;
    struct Passenger* P = L->Next;
    while (P != NULL) {
        count++;
        P = P->Next;
    }
    return count;
}
void DeletePassengerList(struct Passenger* L){
    struct Passenger* P = L->Next;
    struct Passenger* temp;
    L->Next = NULL;
    while (P != NULL){
        temp = P->Next;
        free(P);
        P = temp;
    }
}
struct Passenger* MakeEmptyPassengerList(struct Passenger* L){
    L = (struct Passenger*)malloc(sizeof(struct Passenger));

    if (L == NULL){
        printf("Out of memory\n");
        return NULL;
    }
    L->Next = NULL;
    return L;
}

struct Passenger* FindPassenger(char passport[], struct Passenger* L){
    struct Passenger* P = L->Next;

    while (P != NULL && strcmp(P->passportNumber, passport) != 0){
        P = P->Next;
    }
    return P;
}

struct Passenger* FindPreviousPassenger(char passport[], struct Passenger* L){
    struct Passenger* P = L;

    while (P->Next != NULL && strcmp(P->Next->passportNumber, passport) != 0){
        P = P->Next;
    }

    return P;
}

void InsertPassenger(char name[], char passport[], struct Passenger* L, struct Passenger* P){
    struct Passenger* temp = (struct Passenger*)malloc(sizeof(struct Passenger));

    if (temp == NULL){
        printf("Out of memory\n");
        return;
    }

    strcpy(temp->name, name);
    strcpy(temp->passportNumber, passport);

    temp->Next = P->Next;
    P->Next = temp;
}

void DeletePassenger(char passport[], struct Passenger* L){
    struct Passenger* P = FindPreviousPassenger(passport, L);

    if (P->Next != NULL){
        struct Passenger* temp = P->Next;
        P->Next = temp->Next;
        free(temp);
    }
    else{
        printf("Passenger not found.\n");
    }
}

struct Passenger* FindLastPassenger(struct Passenger* L){

    struct Passenger* P = L;
    while (P->Next != NULL)
        P = P->Next;
    return P;
}

bool  IsEmpy ( struct Passenger* L ){
    return L-> Next ==  NULL ;
}

bool IsLast ( struct Passenger* P){
    return P-> Next == NULL ;
}

void PrintListPassengers ( struct Passenger* L){

    struct Passenger* P=L ;
    if (IsEmpy(L)){
        printf("This flight does not have any Passenger\n") ;
    }
    else{
        printf("Passenger Name\tPassenger Id\n");
        do {
            P=P->Next ;
            printf("%s\t\t%s " ,P->name , P->passportNumber );
        }while (!IsLast(P)) ;
        printf("\n") ;
    }
}
void DisposePassengerLinkedList(struct Passenger * L) {
    struct Passenger* P, *temp;;
    P = L ;
    while (P != NULL) {
        temp = P->Next;
        free(P);
        P = temp;
    }
    free(L) ;
}
//Flight linked list Implementation
struct Flight* MakeEmptyFlightList(){
    struct Flight* L = (struct Flight*)malloc(sizeof(struct Flight));

    if (L == NULL){
        printf("Out of memory!\n");
        return NULL;
    }

    L->next = NULL;
    return L;
}

void InsertFlight(struct Flight* L, struct Flight* F){
    struct Flight* p = L;
    while (p->next != NULL)
        p = p->next;
    F->next = NULL;
    p->next = F;
}

bool  IsEmpyFlight ( struct Flight* L ){
    return L->next ==  NULL ;
}

bool IsLastFlight ( struct Flight* P){
    return P-> next == NULL ;
}
void PrintList(struct Flight* L)
{
    struct Flight* P;
    P = L;
    if (IsEmpyFlight(L)){
        printf("No flights\n");
    }else{
        printf("Flight ID\tState\t\tDate\t\tTime\t\tPassengers\n");
        do {
            P = P->next;
            printf("%s\t%s\t%s\t%s\t%d\n",P->flightID,P->state,P->date,P->time,CountPassengers(P->passengersList));
        } while (!IsLastFlight(P));
    }
}
void DisposeFlightLinkedList(struct Flight* L) {
    struct Flight* P = L;
    struct Flight* temp;

    while (P != NULL) {
        if (P->passengersList != NULL) {
            DisposePassengerLinkedList(P->passengersList);
        }
        temp = P->next;
        free(P);
        P = temp;
    }
     free(L);
}
struct Flight* FindFlight(char id[], struct Flight* L) {
    struct Flight* P = L->next;

    while (P != NULL && strcmp(P->flightID, id) != 0) {
        P = P->next;
    }
    return P;
}
//read flight  and passenger file
struct Flight* findFlightInQueue(struct Queue* Q, char id[]){
    if (isEmptyQueue(Q))
    {
        return NULL ;
    }
    struct Queue tempQ;
    initQueue(&tempQ);

    struct Flight* found = NULL;
    struct Flight* F;
    bool doneComparing = false;

    while ((F = dequeueFlight(Q)) != NULL){
        if (!doneComparing){
            if (strcmp(F->flightID, id) == 0){
                found = F;
                doneComparing = true;
            }
        }
        enqueueFlight(&tempQ, F);
    }
    while ((F = dequeueFlight(&tempQ)) != NULL){
        enqueueFlight(Q, F);
    }
    return found;
}

struct Flight* FindFlightInAllQueue(struct Queue* arrivalQ,struct Queue* departureQ,struct Queue* emergencyQ,char id[]){
    struct Flight* f;

    f = findFlightInQueue(emergencyQ, id);
    if (f != NULL)
        return f;

    f = findFlightInQueue(arrivalQ, id);
    if (f != NULL)
        return f;

    return findFlightInQueue(departureQ, id);
}

void ReadPassengers(struct Queue* arrivalQ,struct Queue* departureQ, struct Queue* emergencyQ){

    if (passengerGlobalFlag==0){
    FILE* f = fopen(Passengers_file, "r");
    if (f == NULL) {
        printf("Error opening passengers file.\n");
        return;
    }
    char temp[200];

    while (fgets(temp, sizeof(temp), f) != NULL){
        char* token = strtok(temp, ";");
        if (token == NULL)
            continue;

        char name[MAX_NAME_SIZE];
        strcpy(name, token);

        token = strtok(NULL, ";");
        if (token == NULL)
            continue;

        char passport[MAX_SIZE];
        strcpy(passport, token);

        token = strtok(NULL, ";");
        if (token == NULL)
            continue;

        char flightID[MAX_SIZE];
        strcpy(flightID, token);


        flightID[strcspn(flightID, "\n")] = 0;

        struct Flight* F = FindFlightInAllQueue(arrivalQ, departureQ, emergencyQ, flightID);

        if (F == NULL){
            printf("\nNote: Passenger %s ignored because Flight id %s not found.\n", name, flightID);
            continue;
        }
      InsertPassenger(name, passport, F->passengersList,FindLastPassenger(F->passengersList));
    }
    printf("\nPassengers file read successfully.\n");
    fclose(f);
    passengerGlobalFlag++ ;
    }
    else
    printf("\nPassengers file has already been read.\n");
}

void ReadFlights(struct Queue* arrivalQ,struct Queue* departureQ,struct Queue* emergencyQ){
    if (FlightGlobalFlag==0){  //check if file has already been loaded
    FILE* f = fopen(Flights_file, "r");
    if (f == NULL) {
        printf("Error opening file.\n");
        return;
    }

    char temp[200];

    while (fgets(temp, sizeof(temp), f) != NULL){//split the words in the file to fill the list with Flights

        char* token = strtok(temp, ";");
        if (token == NULL)
            continue;

        char id[MAX_SIZE];
        strcpy(id, token);

        token = strtok(NULL, ";");

        if (token == NULL)//insures that there is an id
            continue;

        char state[MAX_STATE_SIZE];
        strcpy(state, token);

        token = strtok(NULL, ";");

        if (token == NULL) //insures that there is state
            continue;

        char date[MAX_DATE_SIZE];
        strcpy(date, token);

        token = strtok(NULL, ";"); //insures that there is date

        if (token == NULL)
            continue;

        char time[MAX_TIME_SIZE];
        strcpy(time, token);

        time[strcspn(time, "\n")] = 0;
        int checkStateInput = checkState(state);
        struct Flight* F = createFlight(id, state, date, time);

         switch (checkStateInput){
            case 1 :
                enqueueFlight(arrivalQ, F);
                break ;
            case 2 :
                enqueueFlight(departureQ, F);
                break ;
            case 3:
                enqueueFlight(emergencyQ, F);
                break ;
          }
    }
    printf("\n Flight file read successfully.\n") ;
    fclose(f);
    FlightGlobalFlag++;
    }else
        printf("\nFile has already been read.\n");
}

//menu project function implementation
void PrintMenu(){
    printf("\nMenu");
    printf("\n1. Read flight and passengers files");
    printf("\n2. Print flights");
    printf("\n3. Print flight detail");
    printf("\n4. Add a new flight");
    printf("\n5. Land a flight");
    printf("\n6. Depart plane");
    printf("\n7. Cancel flight");
    printf("\n8. Display flight status");
    printf("\n9. Manage Passengers");
    printf("\n10. Exit");
}
void ManagePassengersMenu(){
    printf("\nMenu");
    printf("\na. print passengers of given flight") ;
    printf("\nb. Add a passenger to a flight") ;
    printf("\nc. Remove a passenger from a flight") ;
    printf("\nd. Back to main menu") ;
}

void printFlightDetail(struct Queue* arrivalQ,struct Queue* departureQ,struct Queue* emergencyQ){

    char flightIdInput[MAX_SIZE] ;
    printf("\nPlease Enter Flight Id") ;
    scanf("%s" ,flightIdInput ) ;
    struct Flight* F=FindFlightInAllQueue(arrivalQ , departureQ, emergencyQ , flightIdInput );//search by flight id
   if (F==NULL)
   {
       printf("Flight with %s not Found" , flightIdInput) ;
       return ;
   }
    printf("Flight ID: %s\n", F->flightID);
    printf("State: %s\n", F->state);
    printf("Date: %s\n", F->date);
    printf("Time: %s\n", F->time);
   PrintListPassengers(F->passengersList) ;

}
int checkState(char state[]) {

    for (int i = 0; state[i] != '\0'; i++)
        state[i] = tolower(state[i]);
    if (strcmp(state, "arrival") == 0)
        return 1;
     if (strcmp(state, "departure") == 0)
     return 2;
     if (strcmp(state, "emergency") == 0)
    return 3;

    return 0;
}
void AddNewFlight (struct Queue* arrivalQ,struct Queue* departureQ,struct Queue* emergencyQ){

    char idFlightInput[MAX_SIZE] , stateInput[MAX_STATE_SIZE] ,dateInput[MAX_DATE_SIZE] ,timeInput[MAX_TIME_SIZE] ;

    printf("\nFill these information to add a Flight:\n");
    printf("Please Enter Flight Id:\n") ;
    scanf("%s" , idFlightInput) ;

    struct Flight* checkFlight =FindFlightInAllQueue(arrivalQ,departureQ,emergencyQ ,idFlightInput ) ;//ensures that there isn't repeated id flight by searching

    if (checkFlight!=NULL){

        printf("\nFlight with Id %s already exists, try again.\n" , idFlightInput) ;
    }else{

        printf("\nPlease Enter Flight State: (Arrival Departure or  Emergency): ") ;
        scanf  ("%s" , stateInput) ;

        int checkStateInput = checkState(stateInput) ;

        if (checkStateInput==0){
            printf("Invalid flight state. Please try again.\n");
            return ;
        }

        printf("Enter Flight Date: ");
        scanf("%s", dateInput);

        printf("Enter Flight Time: ");
        scanf("%s", timeInput);

        struct Flight* F = createFlight(idFlightInput, stateInput, dateInput, timeInput);
          switch (checkStateInput){
            case 1 :
                enqueueFlight(arrivalQ, F);
                break ;
            case 2 :
                enqueueFlight(departureQ, F);
                break ;
            case 3:
                enqueueFlight(emergencyQ, F);
                break ;
          }
              printf("Flight added successfully.\n") ;
    }
}
void LandFlight (struct Queue* arrivalQ,struct Queue* emergencyQ ){

    struct Flight* f = NULL;

    if (!isEmptyQueue(emergencyQ)) //Priority that Emergency flights land first
    {
        f= dequeueFlight(emergencyQ) ;
    }else if (!isEmptyQueue(arrivalQ)){

        f= dequeueFlight(arrivalQ) ;
    }else {

    printf("\nNo flights waiting to land.\n");
    return ;
    }
        printf("%s %s Landed " , f->flightID , f->state) ;
        DeletePassengerList(f->passengersList);
        free(f->passengersList); //Free all passengers before deleting the flight
        free(f);

}

void DepartFlight (struct Queue* departureQ , struct Flight* departedLinkedList)
{
    struct Flight* f = NULL ;
    if (!isEmptyQueue( departureQ)){
        f= dequeueFlight(departureQ) ;
    }else{
        printf("\nNo Flight wait to depart\n") ;
        return ;
    }
    printf("%s Departed\n" , f->flightID) ;
    InsertFlight(departedLinkedList , f) ;
}

void CancelFlight(struct Queue* departureQ, struct Flight* cancelledLinkedList)
{
    if (isEmptyQueue(departureQ)){
        printf("no flight \n") ;
        return ;
    }
    char idFlightInput[MAX_SIZE];
    printf("Please Enter Departure Flight Id you want to cancel:\n");
    scanf("%s", idFlightInput);

    struct Flight* check = findFlightInQueue(departureQ, idFlightInput);

    if (check == NULL) {
        printf("\nFlight with Id %s does not exist in departure flight\n",idFlightInput);
        return;
    }
    struct Queue tempQ;
    initQueue(&tempQ);

    struct Flight* F;

    while (!isEmptyQueue(departureQ)) {
        F = dequeueFlight(departureQ);

        if (strcmp(F->flightID, idFlightInput) == 0) {
            continue;
        }
        enqueueFlight(&tempQ, F);
    }

    while (!isEmptyQueue(&tempQ)) {
        enqueueFlight(departureQ, dequeueFlight(&tempQ));
    }
    InsertFlight(cancelledLinkedList, check);
    printf("Flight cancelled .\n");
}


void DisplayStatus (struct Queue* arrivalQ,struct Queue* departureQ,struct Queue* emergencyQ ,struct Flight* cancelledLinkedList, struct Flight* departedLinkedList )
{
    printQueue(emergencyQ ,"Emergency Flights");
    printQueue(arrivalQ , "Arrival Flights") ;
    printQueue(departureQ ,"Departure Flights" ) ;
    printf("\nCancelled Flights:\n");
    PrintList(cancelledLinkedList) ;
    printf("\nDeparted Flights:\n");
    PrintList(departedLinkedList) ;
}
//passenger
void PrintPassengerOfAFlight(struct Queue* arrivalQ,struct Queue* departureQ,struct Queue* emergencyQ ,struct Flight* cancelledLinkedList, struct Flight* departedLinkedList ){////////عدلي يقدر يطبع معلومات مسافر من linked list
    char flightIdInput[MAX_SIZE] ;
    printf("\nPlease Enter Flight Id\n") ;
    scanf("%s" ,flightIdInput ) ;
    struct Flight* check=FindFlightInAllQueue(arrivalQ , departureQ, emergencyQ , flightIdInput );//check in 3 Queue

   if (check == NULL) {
    check = FindFlight(flightIdInput, cancelledLinkedList);//check in cancelled Flight
    }
   if (check == NULL) {
    check = FindFlight(flightIdInput, departedLinkedList);//check in departed Flight
    }
    if (check == NULL) {
    printf("Flight with %s not Found\n", flightIdInput);
    return;
}
   PrintListPassengers(check->passengersList) ;

}
void AddPassengerToFlight (struct Queue* arrivalQ,struct Queue* departureQ,struct Queue* emergencyQ )
{
    char flightIdInput[MAX_SIZE] ;
    char passportInput[MAX_SIZE];
    char nameInput[MAX_NAME_SIZE];


    printf("\nPlease Enter Flight Id you want to add passenger on it\n") ;
    scanf("%s" ,flightIdInput ) ;
       struct Flight* F=FindFlightInAllQueue(arrivalQ , departureQ, emergencyQ , flightIdInput );
   if (F==NULL)
   {
       printf("Flight with %s not Found" , flightIdInput) ;
       return ;
   }else{

    printf("Passport Number: ");
    scanf("%s", passportInput);
    struct Passenger* check=FindPassenger(passportInput ,F->passengersList ) ;
    if (check == NULL)
    {
        getchar();
        printf("Passenger Name: ");
        gets(nameInput) ;
        InsertPassenger(nameInput , passportInput , F->passengersList, FindLastPassenger(F->passengersList)) ;
        printf("\nPassenger has been successfully added.\n") ;
    }else{
        printf("\nPassenger with passport %s already exists, please try again.\n", passportInput);
    }
   }
}
void RemovePassengerFromFlight(struct Queue* arrivalQ,struct Queue* departureQ,struct Queue* emergencyQ)
{
    char flightIdInput[MAX_SIZE] ,passportInput[MAX_SIZE] ;

    printf("\nPlease Enter Flight Id you want to Remove passenger on it\n") ;
    scanf("%s" ,flightIdInput ) ;

       struct Flight* F=FindFlightInAllQueue(arrivalQ , departureQ, emergencyQ , flightIdInput );
   if (F==NULL){
       printf("Flight with %s not Found" , flightIdInput) ;
       return ;
   }else{
    printf("Passport Number: ");
    scanf("%s", passportInput);
    struct Passenger* check=FindPassenger(passportInput ,F->passengersList ) ;
    if(check==NULL){
        printf("Passenger not found ") ;
    }else {
        DeletePassenger(passportInput , F->passengersList) ;
        printf("\nPassenger with passport %s have been deleted.\n", passportInput);
    }
}
}



