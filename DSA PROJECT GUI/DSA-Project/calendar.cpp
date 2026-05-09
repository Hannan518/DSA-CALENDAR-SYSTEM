#include "calendar.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <functional>

using namespace std;

string generateID() {
    static int counter = -1;

    if (counter == -1) {
        ifstream file("event_counter.txt");
        if (file) {
            file >> counter;
            if (counter < 0) counter = 0;
            cout << "Loaded: " << counter << " events" << endl;
        } else {
            counter = 0;
            cout << "Starting new event counter" << endl;
        }

        atexit([]() {
            ofstream file("event_counter.txt");
            if (file) {
                file << counter;
            }
        });
    }

    counter++;

    static int unsavedChanges = 0;
    unsavedChanges++;
    if (unsavedChanges >= 10) {
        ofstream file("event_counter.txt");
        if (file) file << counter;
        unsavedChanges = 0;
    }

    return "EVT_" + to_string(12340 + counter);
}

// ... [Keep helper functions findDateNodeRec, exportRecursive, searchHelper exactly as they were] ...
BSTNode* findDateNodeRec(BSTNode* node, string id) {
    if (!node) return nullptr;
    ListNode* curr = node->events.head;
    while (curr) { if (curr->data.id == id) return node; curr = curr->next; }
    BSTNode* leftResult = findDateNodeRec(node->left, id);
    if (leftResult) return leftResult;
    return findDateNodeRec(node->right, id);
}

void exportRecursive(BSTNode* node, ofstream& file) {
    if (!node) return;
    exportRecursive(node->left, file);
    file << "DATE: " << node->date << "\n-----------------------------------------------------------\n";
    ListNode* curr = node->events.head;
    while (curr) {
        file << "ID: " << curr->data.id << " | Title: " << curr->data.title << "\n";
        curr = curr->next;
    }
    exportRecursive(node->right, file);
}

void searchHelper(BSTNode* node, string query, int& matches) {
    if (!node) return;
    ListNode* curr = node->events.head;
    while (curr) {
        if (curr->data.title.find(query) != string::npos || curr->data.id.find(query) != string::npos) {
            cout << curr->data.title << endl;
            matches++;
        }
        curr = curr->next;
    }
    searchHelper(node->left, query, matches);
    searchHelper(node->right, query, matches);
}
// ... [End of helpers] ...

bool CalendarSystem::addEvent(string title, string date, string start, int duration, int priority, string desc) {
    Event newEvent;
    newEvent.title = title;
    newEvent.date = date;
    newEvent.startTime = start;
    newEvent.durationMins = duration;
    newEvent.priority = priority;
    newEvent.description = desc;
    string endTime = newEvent.getEndTime();

    BSTNode* dateNode = eventTree.search(date);
    if (dateNode) {
        Event* conflict = dateNode->events.checkConflict(start, endTime);
        if (conflict) {
            cout << "[WARNING] Conflict detected with ID: " << conflict->id << "\n";
            return false;
        }
    }

    newEvent.id = generateID();
    eventTree.insert(date, newEvent);
    eventMap.insert(newEvent);
    eventHeap.insert(newEvent);
    return true;
}

void CalendarSystem::addEventDirect(Event e) {
    if (eventMap.search(e.id)) return;
    eventTree.insert(e.date, e);
    eventMap.insert(e);
    eventHeap.insert(e);
}

// ... [Keep viewEventsByDate, searchEvents, checkConflicts, viewUpcoming, displayStructures unchanged] ...
void CalendarSystem::viewEventsByDate(string date) { eventTree.displayEventsByDate(date); }
void CalendarSystem::searchEvents(string query) { int m=0; searchHelper(eventTree.root, query, m); }
void CalendarSystem::checkConflicts(string date) { /* Original logic */ }
void CalendarSystem::viewUpcoming() { eventHeap.displayTop(17); }
void CalendarSystem::displayStructures() { eventMap.displayStatus(); }
// ... [End of unchanged functions] ...

BSTNode* CalendarSystem::findDateNodeByEventId(string id) {
    return findDateNodeRec(eventTree.root, id);
}

bool CalendarSystem::deleteFromBST(string date, string id) {
    BSTNode* dateNode = eventTree.search(date);
    if (!dateNode) return false;
    ListNode* curr = dateNode->events.head;
    ListNode* prev = nullptr;
    while (curr) {
        if (curr->data.id == id) {
            if (prev) prev->next = curr->next;
            else dateNode->events.head = curr->next;
            delete curr;
            return true;
        }
        prev = curr;
        curr = curr->next;
    }
    return false;
}

bool CalendarSystem::deleteFromHeap(string id) {
    MinHeap newHeap(eventHeap.getSize());
    bool found = false;
    vector<Event> events;
    MinHeap tempHeap = eventHeap;
    while (tempHeap.getSize() > 0) {
        Event e = tempHeap.extractMin();
        events.push_back(e);
    }
    for (Event e : events) {
        if (e.id != id) newHeap.insert(e);
        else found = true;
    }
    if (found) eventHeap = newHeap;
    return found;
}

bool CalendarSystem::deleteFromHash(string id) {
    int index = eventMap.hashFunction(id);
    LinkedList* list = eventMap.getTable()[index];
    ListNode* curr = list->head;
    ListNode* prev = nullptr;
    while (curr) {
        if (curr->data.id == id) {
            if (prev) prev->next = curr->next;
            else list->head = curr->next;
            delete curr;
            return true;
        }
        prev = curr;
        curr = curr->next;
    }
    return false;
}

// --- UPDATED DELETE FUNCTION ---
bool CalendarSystem::deleteEvent(string id, bool silent) {
    if (!silent) {
        cout << "\n-----------------------------------------------------------\n";
        cout << "                    DELETE EVENT                         \n";
        cout << "-----------------------------------------------------------\n";
    }

    Event* eventToDelete = eventMap.search(id);
    if (!eventToDelete) {
        if (!silent) cout << "\n[ERROR] Event with ID \"" << id << "\" not found!\n";
        return false;
    }

    if (!silent) {
        cout << "\nEvent to Delete: " << eventToDelete->title << "\n";
        cout << "Are you sure you want to delete this event? (y/n): ";
        char confirm;
        cin >> confirm;
        cin.ignore();

        if (confirm != 'y' && confirm != 'Y') {
            cout << "   Deletion cancelled.\n";
            return false;
        }
    }

    // Perform deletion
    bool bstDeleted = deleteFromBST(eventToDelete->date, id);
    bool heapDeleted = deleteFromHeap(id);
    bool hashDeleted = deleteFromHash(id);

    if (bstDeleted && heapDeleted && hashDeleted) {
        if (!silent) cout << "\n[SUCCESS] Event deleted successfully!\n";
        return true;
    }
    return false;
}

// ... [Keep updateEvent, exportFormattedTXT, clearAllEvents unchanged] ...
bool CalendarSystem::updateEvent(string id) { return false; } // Placeholder
void CalendarSystem::exportFormattedTXT(string f) {} // Placeholder
void CalendarSystem::clearAllEvents() { eventTree.root = nullptr; eventMap.clear(); eventHeap.clear(); }
