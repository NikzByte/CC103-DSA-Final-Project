#include "auth.h"
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

// ============================================
// HELPER - simple hash function
// ============================================
string simple_hash(const string& input) {
    size_t hash = 0;
    for (char c : input)
        hash = hash * 31 + c;
    return to_string(hash);
}

// ============================================
// HELPER - load users from users.txt into a stack
// Uses a Stack — last registered
// 'user' is on top
// ============================================
User* load_users() {
    ifstream file("users.txt");
    User* top = nullptr;   // top of stack

    if (!file.is_open()) return top;
    if (file.peek() == ifstream::traits_type::eof()) return top;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        User* newUser = new User();

        getline(ss, newUser->username, '|');
        getline(ss, newUser->email,    '|');
        getline(ss, newUser->password, '|');

        // push onto stack — new user becomes top
        newUser->next = top;
        top = newUser;
    }

    file.close();
    return top;
}

// ============================================
// HELPER - save all users back to users.txt
// ============================================
void save_users(User* top) {
    ofstream file("users.txt");
    User* temp = top;
    while (temp != nullptr) {
        file << temp->username << "|"
             << temp->email    << "|"
             << temp->password << "\n";
        temp = temp->next;
    }
    file.close();
}

// ============================================
// HELPER - free stack memory
// ============================================
void free_users(User* top) {
    while (top != nullptr) {
        User* temp = top;
        top = top->next;
        delete temp;
    }
}

// ============================================
// USER REGISTRATION
// ============================================
string register_user() {
    cout << "\nCreate New Account" << endl;

    string username, email, password;

    cout << "Enter a Username: ";
    getline(cin, username);

    // validate BatStateU email
    while (true) {
        cout << "Enter Your Gsuite Email: ";
        getline(cin, email);

        if (email == "exit") {
            cout << "\nReturning to Main Menu...\n";
            return "";
        }

        // equivalent to: email.endswith("@g.batstate-u.edu.ph")
        string domain = "@g.batstate-u.edu.ph";
        if (email.size() > domain.size() &&
            email.substr(email.size() - domain.size()) == domain)
            break;

        cout << "Invalid email. Please use your BatStateU Gsuite email only.\n";
    }

    cout << "Enter a Password: ";
    getline(cin, password);

    if (password == "exit") {
        cout << "\nReturning to Main Menu...\n";
        return "";
    }

    string hashed = simple_hash(password);

    // load existing users, add new one, save back
    User* top = load_users();

    User* newUser = new User();
    newUser->username = username;
    newUser->email    = email;
    newUser->password = hashed;
    newUser->next     = top;   // push onto stack
    top = newUser;

    save_users(top);
    free_users(top);

    cout << "Registration Successful! You can now log in.\n";
    return username;
}

// ============================================
// USER LOGIN
// searches the user stack for a match
// ============================================
string login() {
    while (true) {
        cout << "\nLog Into Your Account" << endl;

        string username, email, password;

        cout << "Enter username: ";
        getline(cin, username);
        if (username == "exit") return "";
            
        cout << "Enter your Gsuite Email: ";
        getline(cin, email);
        if (email == "exit") return "";

        cout << "Enter password: ";
        getline(cin, password);
        if (password == "exit") return "";

        string hashed = simple_hash(password);

        // load stack and search for matching user
        User* top = load_users();
        User* temp = top;
        bool found = false;

        while (temp != nullptr) {
            if (temp->username == username &&
                temp->email    == email    &&
                temp->password == hashed) {
                cout << "\nSuccessfully Logged In!\n";
                found = true;
                break;
            }
            temp = temp->next;  // pop through stack
        }

        free_users(top);

        if (found) return username;
        cout << "Username/Gsuite/Password Is Incorrect. Try again.\n";
    }
}