#ifndef AUTH_H
#define AUTH_H

#include <string>
using namespace std;

// User node for a stack (login session history)
struct User {
    string username;
    string email;
    string password;  // stored as simple_hash (see auth.cpp)
    User* next;       // pointer for stack linking
};

string login();
string register_user();

#endif