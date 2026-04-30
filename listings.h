#ifndef LISTINGS_H
#define LISTINGS_H

#include <string>
using namespace std;

// ======================================================
// LISTING NODE this is equivalent to the Python dict {}
// Each listing is a node in a linked list
// ======================================================
struct Listing {
    string user;        // owner of the listing
    string status;      // "Lost" or "Found"
    string id;          // unique ID e.g. "ITEM-A1B2C3D4"
    string name;
    string description;
    string location;
    string date;
    string contact;
    Listing* next;      // pointer to next listing (linked list)
};

// Function declarations
void load_listings(Listing*& head, Listing*& tail);
void listing_menu(Listing*& head, Listing*& tail, string user);
void create_listing(Listing*& head, Listing*& tail, string current_user);
void view_all_listings(Listing* head);
void lost_item_listings(Listing* head);
void found_item_listings(Listing* head);
void my_listings(Listing* head, string current_user);
void search_listings(Listing* head);
void edit_listing(Listing* head, string current_user);
void delete_listing(Listing*& head, Listing*& tail, string current_user);
void listing_menu(Listing*& head, string current_user);

#endif