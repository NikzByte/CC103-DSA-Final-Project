// to compile code in terminal: g++ *.cpp -o main ; .\main

#include "auth.h"
#include "listings.h"
#include <iostream>
using namespace std;

// ============================================
// MAIN MENU
// ============================================
int main() {
    // Both pointers start at nullptr because the list is empty
    Listing* head = nullptr;
    Listing* tail = nullptr;

    // Load saved listings on startup - pass tail so it can be updated
    load_listings(head, tail);

    string choice;
    while (true) {
        cout << "\n\033[1;31mWelcome to the BatStateU Lost and Found Listing System!\033[0m";
        cout << "\n=== LOGIN OR REGISTER ===" << endl;
        cout << "1. Login"                        << endl;
        cout << "2. Register"                     << endl;
        cout << "3. Exit"                         << endl;
        cout << "Select an option: ";
        getline(cin, choice);

        if (choice == "1") {
            string user = login();
            if (!user.empty())
                // Pass tail here so create_listing and delete_listing can use it
                listing_menu(head, tail, user); 

        } else if (choice == "2") {
            register_user();

        } else if (choice == "3") {
            cout << "\nExiting The Program...\n";
            break;

        } else {
            cout << "Invalid choice.\n";
        }
    }

    // free linked list memory before exit
    Listing* temp = head;
    while (temp != nullptr) {
        Listing* nextNode = temp->next;
        delete temp;
        temp = nextNode;
    }

    return 0;
}