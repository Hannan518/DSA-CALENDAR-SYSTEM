# 📅 DSA Calendar System

A feature-rich desktop Calendar application built in **C++ with Qt**, powered by custom-implemented Data Structures from scratch — no STL containers used for core logic.

---

## 🖥️ Application Preview

> Clean, modern GUI with month navigation, event search, and a real-time event panel.
> ![DSA Calendar System](screenshot.png)

---

## 🚀 Features

- **Monthly Calendar View** — Navigate across all months with a clean visual layout
- **Add / Edit / Remove Events** — Full CRUD operations on calendar events
- **Search Events** — Instant lookup by event Title or unique ID
- **Upcoming Events (30 Days)** — Priority-sorted view of next 30 days using a Min Heap
- **View All Events** — Browse all stored events across dates
- **Conflict Detection** — Automatically detects overlapping event times on the same day
- **Clear All Data** — Reset all events at once

---

## 🧠 Data Structures Implemented

All core data structures are **built from scratch** in `ds_lib.h` — no `std::map`, `std::priority_queue`, or `std::unordered_map` used.

### 🔷 Hash Table (Event ID Lookup)
- Custom implementation with **DJB2 hashing algorithm**
- **Separate chaining** via Linked Lists for collision resolution
- O(1) average-case event lookup by ID
- Table size: 21 buckets

### 🌲 Binary Search Tree (Date-Based Storage)
- Events organized by date using a **BST** for O(log n) date search
- Each BST node holds a **Sorted Linked List** of events for that day
- Supports in-order traversal to display all dates chronologically

### 📋 Sorted Linked List (Per-Day Event Ordering)
- Each date node in the BST contains a **custom Linked List**
- Events inserted in **sorted order by start time** automatically
- Also used for **conflict detection** — checks for overlapping time ranges

### ⬆️ Min Heap (Upcoming Events)
- Custom **Min Heap** with dynamic resizing (doubles capacity when full)
- Extracts events in chronological order for the "Upcoming 30 Days" feature
- Sorted by date first, then by start time for same-day events

---

## 🛠️ Tech Stack

| Component | Technology |
|-----------|-----------|
| Language | C++ (C++17) |
| GUI Framework | Qt (Widgets) |
| Build System | CMake + Makefile |
| Data Structures | Custom (Hash Table, BST, Min Heap, Linked List) |

---

## 📁 Project Structure

```
DSA-CALENDAR-SYSTEM/
│
├── DSA PROJECT GUI/
│   └── DSA-Project/
│       ├── main.cpp                # App entry point, Qt stylesheet
│       ├── mainwindow.cpp/.h       # Core UI logic and event handling
│       ├── calendar.cpp/.h         # Calendar rendering and date logic
│       ├── addeventdialog.cpp/.h   # Add/Edit event dialog UI
│       ├── eventcalendar.h         # Event-calendar integration layer
│       ├── file_manager.cpp/.h     # File persistence (read/write calendar_data.txt)
│       ├── event.cpp/.h            # Event struct and methods
│       ├── ds_lib.h                # All DSA implementations (HashTable, BST, MinHeap, LinkedList)
│       ├── calendar_data.txt       # Persistent event storage
│       ├── CMakeLists.txt          # CMake build configuration
│       └── build/                  # Compiled output
│
└── README.md
```

---

## ⚙️ How to Build & Run

### Requirements
- [Qt Creator](https://www.qt.io/download) (includes Qt framework and compiler)
- Qt 6.x (or Qt 5.x)

### Steps (Qt Creator — Recommended)

1. **Clone the repository**
```bash
git clone https://github.com/Hannan518/DSA-CALENDAR-SYSTEM.git
```

2. **Open Qt Creator**

3. **Open the project** — Go to `File → Open File or Project` and select:
```
DSA PROJECT GUI/DSA-Project/CMakeLists.txt
```

4. **Configure the kit** — Qt Creator will prompt you to select a Qt kit. Choose your installed Qt version and click **Configure Project**.

5. **Build & Run** — Click the green ▶ Run button (or press `Ctrl+R`)

The application window will launch automatically.

---

## 🧩 DSA Concepts Demonstrated

| Concept | Where Used |
|--------|-----------|
| Hash Table + DJB2 Hashing | Fast event lookup by ID |
| Separate Chaining | Hash collision resolution |
| Binary Search Tree | Date-indexed event storage |
| BST Traversal (In-order) | Chronological event display |
| Min Heap + Heapify | Upcoming events sorted by date/time |
| Dynamic Array Resizing | Heap capacity management |
| Sorted Linked List | Per-day events ordered by start time |
| Conflict Detection Algorithm | Overlapping event time checking |
| Deep Copy (Rule of Three) | All DS classes implement copy constructor and assignment operator |

---

## 👨‍💻 Developed By

| Name | Roll No |
|------|---------|
| M Hannan Najeeb | FA24-BSE-080 |
| Muhammad Ahmad | FA24-BSE-068 |
| Shifa Zeeshan | FA24-BSE-113 |
| Mahnoor Khan | FA24-BSE-060 |

---

> Built as a Data Structures & Algorithms course project — all data structures implemented manually without STL containers.
