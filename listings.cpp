#include "listings.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>   // for rand()
#include <ctime>     // for time()
using namespace std;

// ============================================
// HELPER - generate a random 8-char hex ID
// ============================================
string generate_id() {
    // seed random once
    static bool seeded = false;
    if (!seeded) { srand((unsigned)time(0)); seeded = true; }

    string chars = "0123456789ABCDEF";
    string result = "ITEM-";
    for (int i = 0; i < 8; i++)
        result += chars[rand() % 16];
    return result;
}

// ============================================
// HELPER - append using tail pointer
// ============================================
void append_node(Listing*& head, Listing*& tail, Listing* newNode) {
    if (head == nullptr) {
        head = newNode;
        tail = newNode;
    } else {
        tail->next = newNode; // Direct link to the end
        tail = newNode;       // Update the tail to be the new node
    }
    newNode->next = nullptr; // Ensure the new end points to nothing
}

// ============================================
// HELPER - print a single listing's details
// ============================================
void print_listing(Listing* item) {
    cout << "\n----------------------" << endl;
    cout << "STATUS: "      << item->status      << endl;
    cout << "Item ID: "     << item->id          << endl;
    cout << "Name: "        << item->name        << endl;
    cout << "Description: " << item->description << endl;
    cout << "Location: "    << item->location    << endl;
    cout << "Date: "        << item->date        << endl;
    cout << "Contact: "     << item->contact     << endl;
}

// ============================================
// LOAD LISTINGS FROM FILE
// reads listings.txt and builds linked list
// ============================================
void load_listings(Listing*& head, Listing*& tail) {
    ifstream file("listings.txt");

    // equivalent to: FileNotFoundError check
    if (!file.is_open()) {
        cout << "No existing listings found. Starting fresh.\n";
        return;
    }

    // equivalent to: if not content check
    if (file.peek() == ifstream::traits_type::eof()) {
        cout << "Listings file is empty. Starting fresh...\n";
        file.close();
        return;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        // each line: user|status|id|name|desc|location|date|contact
        stringstream ss(line);
        Listing* newNode = new Listing();

        getline(ss, newNode->user,        '|');
        getline(ss, newNode->status,      '|');
        getline(ss, newNode->id,          '|');
        getline(ss, newNode->name,        '|');
        getline(ss, newNode->description, '|');
        getline(ss, newNode->location,    '|');
        getline(ss, newNode->date,        '|');
        getline(ss, newNode->contact,     '|');
        newNode->next = nullptr;

        append_node(head, tail, newNode);
    }

    file.close();
}

// ============================================
// SAVE LISTINGS TO FILE
// traverses linked list and writes to file
// ============================================
void save_listings(Listing* head) {
    ofstream file("listings.txt");

    if (!file.is_open()) {
        cout << "Error: Could not save listings.\n";
        return;
    }

    // traverse linked list and write each node as one line
    Listing* temp = head;
    while (temp != nullptr) {
        file << temp->user        << "|"
             << temp->status      << "|"
             << temp->id          << "|"
             << temp->name        << "|"
             << temp->description << "|"
             << temp->location    << "|"
             << temp->date        << "|"
             << temp->contact     << "\n";
        temp = temp->next;
    }

    file.close();
}

// ============================================
// CREATE LISTING
// equivalent to: create_listing() in Python
// ============================================
void create_listing(Listing*& head, Listing*& tail, string current_user) {
    char again;
    do {
        cout << "\nPOST A LOST OR FOUND ITEM" << endl;

        string status;
        while (true) {
            cout << "Is Your Item Lost or Found?: ";
            getline(cin, status);

            // capitalize first letter
            if (!status.empty()) status[0] = toupper(status[0]);

            if (status == "Lost" || status == "Found") break;
                cout << "Invalid Input. Please Enter 'Lost' or 'Found'" << endl;
        }

        // generate unique item id
        string item_id = generate_id();
        string name, desc, loc, date, contact;

        cout << "Enter Item Name: ";        
            getline(cin, name);
        cout << "Enter Item Description: "; 
            getline(cin, desc);
        cout << "Enter Location " << status << " at: "; 
            getline(cin, loc);
        cout << "Enter Date " << status << ": ";        
            getline(cin, date);
        cout << "Enter Your Contact Details: ";         
            getline(cin, contact);

        // create new node
        Listing* newNode = new Listing();
        newNode->user        = current_user;
        newNode->status      = status;
        newNode->id          = item_id;
        newNode->name        = name;
        newNode->description = desc;
        newNode->location    = loc;
        newNode->date        = date;
        newNode->contact     = contact;
        newNode->next        = nullptr;

        // add to linked list — equivalent to: listings.append(listing)
        append_node(head, tail, newNode);
        save_listings(head);

        cout << "\nListing successfully added! ID: " << item_id << endl;
        cout << "Add another? (y/n): ";
        cin >> again;
        cin.ignore();

    } while (again == 'y' || again == 'Y');
}

// ============================================
// VIEW ALL LISTINGS
// traverses entire linked list
// ============================================
void view_all_listings(Listing* head) {
    if (head == nullptr) {
        cout << "\nNo listings available." << endl;
        return;
    }

    cout << "\n\033[1;31ALL ITEM LISTINGS:\033[0m";
    Listing* temp = head;
    while (temp != nullptr) {
        print_listing(temp);
        temp = temp->next;     // move to next node
    }
}

// ============================================
// VIEW LOST ITEMS ONLY
// filters linked list by status == "Lost"
// ============================================
void lost_item_listings(Listing* head) {
    cout << "\n\033[1;31ALL LOST ITEM LISTINGS:\033[0m" << endl;
    bool found_any = false;

    Listing* temp = head;
    while (temp != nullptr) {
        if (temp->status == "Lost") {
            print_listing(temp);
            found_any = true;
        }
        temp = temp->next;
    }

    if (!found_any)
        cout << "No lost items have been reported yet." << endl;
}

// ============================================
// VIEW FOUND ITEMS ONLY
// ============================================
void found_item_listings(Listing* head) {
    cout << "\n\033[1;31ALL FOUND ITEM LISTINGS:\033[0m" << endl;
    bool found_any = false;

    Listing* temp = head;
    while (temp != nullptr) {
        if (temp->status == "Found") {
            print_listing(temp);
            found_any = true;
        }
        temp = temp->next;
    }

    if (!found_any)
        cout << "No found items have been reported yet." << endl;
}

// ============================================
// VIEW MY LISTINGS
// filters linked list by user
// ============================================
void my_listings(Listing* head, string current_user) {
    cout << "\n\n\033[1;31MY LISTINGS:\033[0m" << endl;
    bool found_any = false;

    Listing* temp = head;
    while (temp != nullptr) {
        if (temp->user == current_user) {
            print_listing(temp);
            found_any = true;
        }
        temp = temp->next;
    }

    if (!found_any)
        cout << "You have no listings yet." << endl;
}

// ============================================
// SEARCH LISTINGS BY KEYWORD
// ============================================
void search_listings(Listing* head) {
    string keyword;
    cout << "\nEnter keyword to search: ";
    getline(cin, keyword);

    // convert keyword to lowercase for case-insensitive search
    for (char& c : keyword) c = tolower(c);

    bool found = false;
    Listing* temp = head;

    while (temp != nullptr) {
        // make lowercase copies for comparison
        string name = temp->name, desc = temp->description, loc = temp->location;
        for (char& c : name) c = tolower(c);
        for (char& c : desc) c = tolower(c);
        for (char& c : loc)  c = tolower(c);

        // find keyword in active listings
        if (name.find(keyword) != string::npos ||
            desc.find(keyword) != string::npos ||
            loc.find(keyword)  != string::npos) {
            print_listing(temp);
            found = true;
        }
        temp = temp->next;
    }

    if (!found)
        cout << "No matching items found." << endl;
}

// ============================================
// DELETE LISTING
// removes a node from the linked list
// ============================================
void delete_listing(Listing*& head, Listing*& tail, string current_user) {
    cout << "\n\033[1;31DELETE ITEM LISTING:\033[0m" << endl;
    bool has_listings = false;

    Listing* temp = head;
    while (temp != nullptr) {
        if (temp->user == current_user) {
            cout << "\n----------------------" << endl;
            cout << "Item ID: " << temp->id     << endl;
            cout << "Name: "    << temp->name    << endl;
            cout << "Status: "  << temp->status  << endl;
            has_listings = true;
        }
        temp = temp->next;
    }

    if (!has_listings) {
        cout << "You have no listings to delete." << endl;
        return;
    }

    string delete_id;
    cout << "\nEnter Item ID to delete: ";
    getline(cin, delete_id);

    // need prev pointer to re-link the list after deletion
    Listing* prev = nullptr;
    temp = head;

    while (temp != nullptr) {
        if (temp->id == delete_id && temp->user == current_user) {
            char confirm;
            cout << "Are you sure you want to delete this? (y/n): ";
            cin >> confirm;
            cin.ignore();

            if (confirm == 'y' || confirm == 'Y') {
                if (prev == nullptr) {
                    head = temp->next;
                    if (head == nullptr) tail = nullptr; // List is now empty
                } else {
                    prev->next = temp->next;
                    if (temp == tail) tail = prev; // If we deleted the last item, update tail
                }
                delete temp;
                save_listings(head);
            }
        }
    }
}
// ============================================
// LISTING MENU
// ============================================
void listing_menu(Listing*& head, Listing*& tail, string current_user) {
    string choice;
    while (true) {
        cout << "\nLISTING OPTIONS"      << endl;
        cout << "1. Create a Post"       << endl;
        cout << "2. View all Listings"   << endl;
        cout << "3. Lost Item Listings"  << endl;
        cout << "4. Found Item Listings" << endl;
        cout << "5. My Listings"         << endl;
        cout << "6. Search By Keyword"   << endl;
        cout << "7. Delete My Listing"   << endl;
        cout << "8. Logout"              << endl;
        cout << "Select an option: ";
        getline(cin, choice);

        if (choice == "1") create_listing(head, tail, current_user);
        else if (choice == "2") view_all_listings(head);
        else if (choice == "3") lost_item_listings(head);
        else if (choice == "4") found_item_listings(head);
        else if (choice == "5") my_listings(head, current_user);
        else if (choice == "6") search_listings(head);
        else if (choice == "7") delete_listing(head, tail, current_user);
        else if (choice == "8") { cout << "Logging out...\n"; break; }
        else cout << "Invalid option. Please enter a number from 1 to 9." << endl;
    }
}