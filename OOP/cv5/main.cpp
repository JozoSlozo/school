using namespace std;
#include <iostream>

class Account;

#pragma region history
class historyElem
{
private:
    string type;
    double amount;
    Account *person;
public:
    historyElem(string t, double a);
    historyElem(string t, double a, Account* p);
    string GetType();
    double GetAmount();
    Account* GetPerson();
    string PP();
};
historyElem::historyElem(string t, double a)
{
    this->type = t;
    this->amount = a;
    this->person = nullptr;
}
historyElem::historyElem(string t, double a, Account* p)
{
    this->type = t;
    this->amount = a;
    this->person = p;
}
string historyElem::GetType(){
    return this->type;
}
double historyElem::GetAmount(){
    return this->amount;
}
Account* historyElem::GetPerson(){
    return this->person;
}
#pragma endregion

#pragma region Client
class Client
{
    private:
        int code;
        string name;
    public:
        Client(int c, string n);
        int GetCode();
        string GetName();
};
Client::Client(int c, string n){
    this->code = c;
    this->name = n;
}
int Client::GetCode(){
    return this->code;
}
string Client::GetName(){
    return this->name;
}
#pragma endregion

#pragma region Account
class Account
{
    private:
        int number;
        double balance;
        double interestRate;
        Client *owner;
        Client *partner;
        historyElem** history;
        int historyCount;
    public:
        ~Account();
        Account (int n, Client *c);
        Account (int n, Client *c, double ir);
        Account (int n, Client *c, Client *p);
        Account (int n, Client *c, Client *p, double ir);

        int GetNumber();
        double GetBalance();
        double GetInterestRate();
        int GetHistoryCount();
        void IncHistoryCount();
        historyElem** GetHistory();
        Client *GetOwner();
        Client *GetPartner();
        string historyPP(int i);

        bool Canwithdraw (double a);
        void Deposit (double a, bool l);
        bool Withdraw (double a, bool l); 
        void AddInterest();
};
#pragma region constructo and basic
Account::~Account(){
    for (int i = 0; i < this->historyCount; i++)
    {
        delete this->history[i];
    }
    delete[] this->history;
}

Account::Account(int n, Client *c){
    this->number = n;
    this->balance = 0;
    this->interestRate = 0;
    this->owner = c;
    this->partner = nullptr;
    this->history = new historyElem*[10];
    this->historyCount = 0;
}
Account::Account(int n, Client *c, double ir){
    this->number = n;
    this->balance = 0;
    this->interestRate = 0;
    this->owner = c;
    this->interestRate = ir;
    this->partner = nullptr;
    this->history = new historyElem*[10];
    this->historyCount = 0;
}
Account::Account(int n, Client *c, Client *p){
    this->number = n;
    this->balance = 0;
    this->interestRate = 0;
    this->owner = c;
    this->partner = p;
    this->history = new historyElem*[10];
    this->historyCount = 0;
}
Account::Account(int n, Client *c, Client *p, double ir){
    this->number = n;
    this->balance = 0;
    this->interestRate = 0;
    this->owner = c;
    this->partner = p;
    this->interestRate = ir;
    this->history = new historyElem*[10];
    this->historyCount = 0;
}
historyElem** Account::GetHistory(){
    return this->history;
}
int Account::GetHistoryCount(){
    return this->historyCount;
}
int Account::GetNumber(){
    return this->number;
}
double Account::GetBalance(){
    return this->balance;
}
double Account::GetInterestRate(){
    return this->interestRate;
}
Client* Account::GetOwner(){
    return this->owner;
}
Client* Account::GetPartner(){
    return this->partner;
}
#pragma endregion

string Account::historyPP(int i){
    char buff[100];
    historyElem* elem = this->GetHistory()[i];
    if (elem->GetPerson() != nullptr)
    {
        sprintf(buff, "Typ: %s, částka: %f, druhy ucet: %s", elem->GetType().c_str(), elem->GetAmount(), elem->GetPerson()->GetOwner()->GetName().c_str());
    }
    else{
        sprintf(buff, "Typ: %s, částka: %f", elem->GetType().c_str(), elem->GetAmount());
    }
    string temp = buff;
    return temp;
}

void Account::IncHistoryCount(){
    this->historyCount += 1;
}

bool Account::Canwithdraw(double a){
    if (a <= this->GetBalance())
    {
        return 1;
    }
    return 0;
}
void Account::Deposit(double a, bool l){
    this->balance += a + a * interestRate;
    if (l)
    {
        this->history[this->historyCount] = new historyElem("Deposit", a);
        this->historyCount += 1;
    }
}
bool Account::Withdraw(double a, bool l){
    if (this->Canwithdraw(a))
    {
        if (l)
        {
            this->history[this->historyCount] = new historyElem("Withdraw", a);
            this->historyCount += 1;
        }
        this->balance -= a;
        return 1;
    }
    return 0;
}
void Account::AddInterest(){
    this->balance += this->balance * this->interestRate;
}
#pragma endregion

#pragma region Bank
class Bank
{
    private:
        Client** clients;
        int clientsCount;
        Account** accounts;
        int accountsCount;
    public:
        Bank(int c, int a);
        ~Bank();

        Client* GetClient(int c);
        Account* GetAccount (int n);

        Client* CreateClient(int c, string n);
        Account* CreateAccount (int n, Client *c);
        Account* CreateAccount (int n, Client *c, double ir);
        Account* CreateAccount (int n, Client *c, Client *p);
        Account* CreateAccount (int n, Client *c, Client *p, double ir);
        void AddInterest();

};
#pragma region methods
Bank::Bank(int c, int a){
    this->clients = new Client*[c];
    this->accounts = new Account*[a];
    this->clientsCount = 0;
    this->accountsCount = 0;
}
Bank::~Bank(){
    for (int i = 0; i < this->accountsCount; i++)
    {
        delete this->accounts[i];
    }
    delete[] this->accounts;
    for (int i = 0; i < this->clientsCount; i++)
    {
        delete this->clients[i];
    }
    delete[] this->clients;
}
Client* Bank::GetClient(int c){
    for (int i = 0; i < this->clientsCount; i++)
    {
        if (this->clients[i]->GetCode() == c)
        {
            return this->clients[i];
        }
    }
    return nullptr;
}
Account* Bank::GetAccount(int n){
    for (int i = 0; i < this->accountsCount; i++)
    {
        if (this->accounts[i]->GetNumber() == n)
        {
            return this->accounts[i];
        }
    }
    return nullptr;
}
Client* Bank::CreateClient(int c, string n){
    Client* temp=  new Client(c, n);
    this->clients[this->clientsCount] = temp;
    this->clientsCount += 1;
    return temp;
}
Account* Bank::CreateAccount(int n, Client* c){
    Account* temp = new Account(n, c);
    this->accounts[this->accountsCount] = temp;
    this->accountsCount += 1;
    return temp;
}
Account* Bank::CreateAccount(int n, Client* c, double ir){
    Account* temp = new Account(n, c, ir);
    this->accounts[this->accountsCount] = temp;
    this->accountsCount += 1;
    return temp;
}
Account* Bank::CreateAccount(int n, Client* c, Client* p){
    Account* temp = new Account(n, c, p);
    this->accounts[this->accountsCount] = temp;
    this->accountsCount += 1;
    return temp;
}
Account* Bank::CreateAccount(int n, Client* c, Client* p, double ir){
    Account* temp = new Account(n, c, p, ir);
    this->accounts[this->accountsCount] = temp;
    this->accountsCount += 1;
    return temp;
}
void Bank::AddInterest(){
    for (int i = 0; i < this->accountsCount; i++)
    {
        this->accounts[i]->AddInterest();
    }   
}
#pragma endregion
#pragma endregion

#pragma region Transakce
class transakce
{
private:
    Account* sender;
    Account* reciever;
    double amount;
public:
    transakce(Account* s, Account* r, double a);
    bool maketransaction();
    string maketransactionPP();
};

transakce::transakce(Account* s, Account* r, double a)
{
    this->sender = s;
    this->reciever = r;
    this->amount = a;
}
bool transakce::maketransaction(){
    if (this->sender->Withdraw(this->amount, 0))
    {
        this->sender->GetHistory()[this->sender->GetHistoryCount()] = new historyElem("Transaction send", this->amount, this->reciever);
        this->sender->IncHistoryCount();
        this->reciever->Deposit(this->amount, 0);
        this->reciever->GetHistory()[this->reciever->GetHistoryCount()] = new historyElem("Transaction recieved", this->amount, this->sender);
        this->reciever->IncHistoryCount();
        return 1;
    }
    return 0;
}
string transakce::maketransactionPP(){
    if (this->maketransaction())
    {
        char buff[200];
        sprintf(buff, "transakce proběhla, částka: %f\nodesilatel: %s, stav účtu: %f->%f\npříjemce: %s, stav účtu: %f->%f",
        this->amount, 
        this->sender->GetOwner()->GetName().c_str(), this->sender->GetBalance()+this->amount, this->sender->GetBalance(), 
        this->reciever->GetOwner()->GetName().c_str(), this->reciever->GetBalance()-this->amount, this->reciever->GetBalance());
        return buff;
    }
    return "transakce neproběhla";
}
#pragma endregion

int main(){
    int N = 100;
    Bank* bank = new Bank(N, N);
    for (int i = 0; i < N; i++)
    {
        char buff[50];
        sprintf(buff, "Client%d", i);
        bank->CreateClient(i, buff);
        bank->CreateAccount(i, bank->GetClient(i), 0.05);
        bank->GetAccount(i)->Deposit(rand() % 1000, 1);
    }
    for (int i = 0; i < 10; i++)
    {
        bank->GetAccount(rand()%100)->Deposit(rand()%1000, 1);
    }
    bank->AddInterest();

    transakce* t = new transakce(bank->GetAccount(1), bank->GetAccount(2), 20.2);
    t->maketransaction();
    delete t;
    transakce* r = new transakce(bank->GetAccount(2), bank->GetAccount(1), 10.2);
    r->maketransaction();
    delete r;
    
    for (int i = 0; i < bank->GetAccount(1)->GetHistoryCount(); i++)
    {
        cout << bank->GetAccount(1)->historyPP(i) << endl;
    }
    
    delete bank;
    return 0;
}