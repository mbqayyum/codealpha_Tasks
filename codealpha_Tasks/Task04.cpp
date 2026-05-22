// task 4: banking system
// codealpha internship - c++ programming
// oop-based banking system with customer, account, and transaction management

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <ctime>
#include <sstream>

using namespace std;

// structure to hold a single transaction
struct transaction {
    string type;       // deposit, withdrawal, transfer
    float amount;
    string timestamp;
    string details;
};

// gets current date and time as a string
string get_timestamp() {
    time_t now = time(0);
    struct tm time_info;
    localtime_s(&time_info, &now);
    char buffer[80];
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", &time_info);
    return string(buffer);
}

// class representing a bank account
class account {
private:
    int account_number;
    string account_type; // savings or checking
    float balance;
    vector<transaction> history;

public:
    account() : account_number(0), account_type("savings"), balance(0.0) {}

    account(int acc_num, string type, float initial_balance) {
        account_number = acc_num;
        account_type = type;
        balance = initial_balance;

        // record the opening deposit
        transaction t;
        t.type = "deposit";
        t.amount = initial_balance;
        t.timestamp = get_timestamp();
        t.details = "initial deposit - account opened";
        history.push_back(t);
    }

    int get_account_number() { return account_number; }
    string get_account_type() { return account_type; }
    float get_balance() { return balance; }

    // deposits money into the account
    bool deposit(float amount) {
        if (amount <= 0) {
            cout << "deposit amount must be positive.\n";
            return false;
        }
        balance += amount;

        transaction t;
        t.type = "deposit";
        t.amount = amount;
        t.timestamp = get_timestamp();
        t.details = "cash deposit";
        history.push_back(t);

        cout << "deposited " << fixed << setprecision(2) << amount << " successfully.\n";
        cout << "new balance: " << fixed << setprecision(2) << balance << "\n";
        return true;
    }

    // withdraws money from the account
    bool withdraw(float amount) {
        if (amount <= 0) {
            cout << "withdrawal amount must be positive.\n";
            return false;
        }
        if (amount > balance) {
            cout << "insufficient funds. your balance is " << fixed << setprecision(2) << balance << "\n";
            return false;
        }
        balance -= amount;

        transaction t;
        t.type = "withdrawal";
        t.amount = amount;
        t.timestamp = get_timestamp();
        t.details = "cash withdrawal";
        history.push_back(t);

        cout << "withdrew " << fixed << setprecision(2) << amount << " successfully.\n";
        cout << "new balance: " << fixed << setprecision(2) << balance << "\n";
        return true;
    }

    // records a transfer in the history
    void record_transfer(string type, float amount, int other_acc) {
        transaction t;
        t.type = "transfer";
        t.amount = amount;
        t.timestamp = get_timestamp();

        stringstream ss;
        if (type == "sent") {
            ss << "transferred to account #" << other_acc;
        }
        else {
            ss << "received from account #" << other_acc;
        }
        t.details = ss.str();
        history.push_back(t);
    }

    // displays recent transactions
    void show_transactions(int count = 10) {
        cout << "\n--- transaction history (account #" << account_number << ") ---\n";
        cout << left << setw(22) << "date/time" << setw(15) << "type" << setw(12) << "amount" << "details" << endl;
        cout << string(70, '-') << endl;

        int start = 0;
        if (history.size() > count) {
            start = history.size() - count;
        }

        for (int i = start; i < history.size(); i++) {
            cout << left << setw(22) << history[i].timestamp
                << setw(15) << history[i].type
                << setw(12) << fixed << setprecision(2) << history[i].amount
                << history[i].details << endl;
        }

        if (history.empty()) {
            cout << "no transactions yet.\n";
        }
    }

    // displays account info
    void show_info() {
        cout << "account #" << account_number
            << " | type: " << account_type
            << " | balance: " << fixed << setprecision(2) << balance << "\n";
    }
};

// class representing a customer
class customer {
private:
    int customer_id;
    string name;
    string phone;
    vector<account> accounts;

public:
    customer() : customer_id(0), name(""), phone("") {}

    customer(int id, string n, string p) {
        customer_id = id;
        name = n;
        phone = p;
    }

    int get_id() { return customer_id; }
    string get_name() { return name; }

    // creates a new account for this customer
    void create_account(int acc_num, string type, float initial_balance) {
        account acc(acc_num, type, initial_balance);
        accounts.push_back(acc);
        cout << "account #" << acc_num << " created successfully for " << name << ".\n";
    }

    // finds an account by number, returns pointer or null
    account* find_account(int acc_num) {
        for (int i = 0; i < accounts.size(); i++) {
            if (accounts[i].get_account_number() == acc_num) {
                return &accounts[i];
            }
        }
        return nullptr;
    }

    // shows all accounts of this customer
    void show_accounts() {
        cout << "\naccounts for " << name << " (id: " << customer_id << "):\n";
        if (accounts.empty()) {
            cout << "  no accounts found.\n";
            return;
        }
        for (int i = 0; i < accounts.size(); i++) {
            cout << "  ";
            accounts[i].show_info();
        }
    }

    // displays customer info
    void show_info() {
        cout << "customer id: " << customer_id
            << " | name: " << name
            << " | phone: " << phone
            << " | accounts: " << accounts.size() << "\n";
    }
};

// the main banking system class
class banking_system {
private:
    vector<customer> customers;
    int next_customer_id;
    int next_account_number;

public:
    banking_system() : next_customer_id(1001), next_account_number(5001) {}

    // creates a new customer
    void create_customer() {
        string name, phone;
        cin.ignore();
        cout << "enter customer name: ";
        getline(cin, name);
        cout << "enter phone number: ";
        getline(cin, phone);

        customer c(next_customer_id, name, phone);
        customers.push_back(c);
        cout << "customer created with id: " << next_customer_id << "\n";
        next_customer_id++;
    }

    // finds a customer by id
    customer* find_customer(int id) {
        for (int i = 0; i < customers.size(); i++) {
            if (customers[i].get_id() == id) {
                return &customers[i];
            }
        }
        return nullptr;
    }

    // opens a new account for an existing customer
    void open_account() {
        int cust_id;
        cout << "enter customer id: ";
        cin >> cust_id;

        customer* c = find_customer(cust_id);
        if (c == nullptr) {
            cout << "customer not found.\n";
            return;
        }

        string type;
        float initial;
        cout << "account type (savings/checking): ";
        cin >> type;
        cout << "initial deposit amount: ";
        cin >> initial;

        if (initial < 0) {
            cout << "initial deposit cannot be negative.\n";
            return;
        }

        c->create_account(next_account_number, type, initial);
        next_account_number++;
    }

    // performs a deposit
    void do_deposit() {
        int cust_id, acc_num;
        float amount;

        cout << "enter customer id: ";
        cin >> cust_id;
        customer* c = find_customer(cust_id);
        if (c == nullptr) { cout << "customer not found.\n"; return; }

        c->show_accounts();
        cout << "enter account number: ";
        cin >> acc_num;
        account* a = c->find_account(acc_num);
        if (a == nullptr) { cout << "account not found.\n"; return; }

        cout << "enter deposit amount: ";
        cin >> amount;
        a->deposit(amount);
    }

    // performs a withdrawal
    void do_withdrawal() {
        int cust_id, acc_num;
        float amount;

        cout << "enter customer id: ";
        cin >> cust_id;
        customer* c = find_customer(cust_id);
        if (c == nullptr) { cout << "customer not found.\n"; return; }

        c->show_accounts();
        cout << "enter account number: ";
        cin >> acc_num;
        account* a = c->find_account(acc_num);
        if (a == nullptr) { cout << "account not found.\n"; return; }

        cout << "enter withdrawal amount: ";
        cin >> amount;
        a->withdraw(amount);
    }

    // transfers funds between two accounts
    void do_transfer() {
        int from_cust, from_acc, to_cust, to_acc;
        float amount;

        cout << "--- sender info ---\n";
        cout << "enter sender customer id: ";
        cin >> from_cust;
        customer* sender = find_customer(from_cust);
        if (sender == nullptr) { cout << "sender customer not found.\n"; return; }

        sender->show_accounts();
        cout << "enter sender account number: ";
        cin >> from_acc;
        account* sender_acc = sender->find_account(from_acc);
        if (sender_acc == nullptr) { cout << "sender account not found.\n"; return; }

        cout << "\n--- receiver info ---\n";
        cout << "enter receiver customer id: ";
        cin >> to_cust;
        customer* receiver = find_customer(to_cust);
        if (receiver == nullptr) { cout << "receiver customer not found.\n"; return; }

        receiver->show_accounts();
        cout << "enter receiver account number: ";
        cin >> to_acc;
        account* receiver_acc = receiver->find_account(to_acc);
        if (receiver_acc == nullptr) { cout << "receiver account not found.\n"; return; }

        cout << "enter transfer amount: ";
        cin >> amount;

        if (sender_acc->withdraw(amount)) {
            receiver_acc->deposit(amount);
            sender_acc->record_transfer("sent", amount, to_acc);
            receiver_acc->record_transfer("received", amount, from_acc);
            cout << "transfer completed successfully!\n";
        }
        else {
            cout << "transfer failed.\n";
        }
    }

    // shows transaction history for an account
    void show_history() {
        int cust_id, acc_num;

        cout << "enter customer id: ";
        cin >> cust_id;
        customer* c = find_customer(cust_id);
        if (c == nullptr) { cout << "customer not found.\n"; return; }

        c->show_accounts();
        cout << "enter account number: ";
        cin >> acc_num;
        account* a = c->find_account(acc_num);
        if (a == nullptr) { cout << "account not found.\n"; return; }

        a->show_transactions();
    }

    // shows all customers
    void show_all_customers() {
        cout << "\n--- all customers ---\n";
        if (customers.empty()) {
            cout << "no customers registered.\n";
            return;
        }
        for (int i = 0; i < customers.size(); i++) {
            customers[i].show_info();
        }
    }

    // shows account details
    void show_account_info() {
        int cust_id;
        cout << "enter customer id: ";
        cin >> cust_id;
        customer* c = find_customer(cust_id);
        if (c == nullptr) { cout << "customer not found.\n"; return; }
        c->show_accounts();
    }
};

int main() {
    banking_system bank;
    int choice;

    cout << "========================================\n";
    cout << "        banking system\n";
    cout << "        codealpha internship\n";
    cout << "========================================\n";

    do {
        cout << "\n--- main menu ---\n";
        cout << "1. create customer\n";
        cout << "2. open account\n";
        cout << "3. deposit\n";
        cout << "4. withdraw\n";
        cout << "5. transfer funds\n";
        cout << "6. view transaction history\n";
        cout << "7. view account info\n";
        cout << "8. view all customers\n";
        cout << "9. exit\n";
        cout << "enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: bank.create_customer(); break;
        case 2: bank.open_account(); break;
        case 3: bank.do_deposit(); break;
        case 4: bank.do_withdrawal(); break;
        case 5: bank.do_transfer(); break;
        case 6: bank.show_history(); break;
        case 7: bank.show_account_info(); break;
        case 8: bank.show_all_customers(); break;
        case 9: cout << "thank you for using our banking system. goodbye!\n"; break;
        default: cout << "invalid choice. please try again.\n";
        }
    } while (choice != 9);

    return 0;
}
