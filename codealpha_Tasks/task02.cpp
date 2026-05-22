// task 2: login and registration system
// codealpha internship - c++ programming
// file-based user authentication system

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

const string users_file = "users.txt";

// simple hash function to avoid storing passwords in plain text
string hash_password(string password) {
    unsigned long hash = 5381;
    for (int i = 0; i < password.length(); i++) {
        hash = ((hash << 5) + hash) + password[i];
    }
    // convert hash to string
    stringstream ss;
    ss << hash;
    return ss.str();
}

// checks if a username already exists in the file
bool username_exists(string username) {
    ifstream file(users_file);
    string line;

    while (getline(file, line)) {
        // each line format: username:hashed_password
        size_t pos = line.find(':');
        if (pos != string::npos) {
            string stored_user = line.substr(0, pos);
            if (stored_user == username) {
                file.close();
                return true;
            }
        }
    }
    file.close();
    return false;
}

// validates username - must be at least 3 chars, no spaces
bool validate_username(string username) {
    if (username.length() < 3) {
        cout << "username must be at least 3 characters long.\n";
        return false;
    }
    for (int i = 0; i < username.length(); i++) {
        if (username[i] == ' ') {
            cout << "username cannot contain spaces.\n";
            return false;
        }
    }
    return true;
}

// validates password - must be at least 6 chars, have a digit and a letter
bool validate_password(string password) {
    if (password.length() < 6) {
        cout << "password must be at least 6 characters long.\n";
        return false;
    }
    bool has_digit = false;
    bool has_letter = false;
    for (int i = 0; i < password.length(); i++) {
        if (isdigit(password[i])) has_digit = true;
        if (isalpha(password[i])) has_letter = true;
    }
    if (!has_digit) {
        cout << "password must contain at least one digit.\n";
        return false;
    }
    if (!has_letter) {
        cout << "password must contain at least one letter.\n";
        return false;
    }
    return true;
}

// registers a new user
void register_user() {
    string username, password, confirm_password;

    cout << "\n--- register new account ---\n";
    cout << "enter username: ";
    cin >> username;

    if (!validate_username(username)) return;

    if (username_exists(username)) {
        cout << "sorry, username '" << username << "' is already taken.\n";
        return;
    }

    cout << "enter password: ";
    cin >> password;

    if (!validate_password(password)) return;

    cout << "confirm password: ";
    cin >> confirm_password;

    if (password != confirm_password) {
        cout << "passwords do not match. registration failed.\n";
        return;
    }

    // save to file
    ofstream file(users_file, ios::app);
    if (file.is_open()) {
        file << username << ":" << hash_password(password) << "\n";
        file.close();
        cout << "registration successful! welcome, " << username << "!\n";
    }
    else {
        cout << "error: could not open file to save user data.\n";
    }
}

// logs in an existing user
void login_user() {
    string username, password;

    cout << "\n--- login ---\n";
    cout << "enter username: ";
    cin >> username;
    cout << "enter password: ";
    cin >> password;

    string hashed = hash_password(password);

    ifstream file(users_file);
    if (!file.is_open()) {
        cout << "no users registered yet. please register first.\n";
        return;
    }

    string line;
    bool found = false;

    while (getline(file, line)) {
        size_t pos = line.find(':');
        if (pos != string::npos) {
            string stored_user = line.substr(0, pos);
            string stored_pass = line.substr(pos + 1);
            if (stored_user == username && stored_pass == hashed) {
                found = true;
                break;
            }
        }
    }
    file.close();

    if (found) {
        cout << "login successful! welcome back, " << username << "!\n";
    }
    else {
        cout << "invalid username or password. please try again.\n";
    }
}

// displays all registered users (just usernames, not passwords)
void show_registered_users() {
    ifstream file(users_file);
    if (!file.is_open()) {
        cout << "no users registered yet.\n";
        return;
    }

    cout << "\n--- registered users ---\n";
    string line;
    int count = 0;
    while (getline(file, line)) {
        size_t pos = line.find(':');
        if (pos != string::npos) {
            cout << "  " << ++count << ". " << line.substr(0, pos) << "\n";
        }
    }

    if (count == 0) {
        cout << "no users found.\n";
    }
    file.close();
}

int main() {
    int choice;

    cout << "========================================\n";
    cout << "   login and registration system\n";
    cout << "   codealpha internship\n";
    cout << "========================================\n";

    do {
        cout << "\n--- main menu ---\n";
        cout << "1. register\n";
        cout << "2. login\n";
        cout << "3. show registered users\n";
        cout << "4. exit\n";
        cout << "enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            register_user();
            break;
        case 2:
            login_user();
            break;
        case 3:
            show_registered_users();
            break;
        case 4:
            cout << "goodbye! thanks for using the system.\n";
            break;
        default:
            cout << "invalid choice. please try again.\n";
        }
    } while (choice != 4);

    return 0;
}
