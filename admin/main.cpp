#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>
#include <iomanip>

using namespace std;


//ENUMS
enum class BookingStatus { PENDING, CONFIRMED, CANCELLED };

string bookingStatusToStr(BookingStatus s) {
    switch (s) {
        case BookingStatus::PENDING:   return "pending";
        case BookingStatus::CONFIRMED: return "confirmed";
        case BookingStatus::CANCELLED: return "cancelled";
    }
    return "pending";
}

BookingStatus strToBookingStatus(const string& s) {
    if (s == "confirmed") return BookingStatus::CONFIRMED;
    if (s == "cancelled") return BookingStatus::CANCELLED;
    return BookingStatus::PENDING;
}


//STRUCTS
struct Account {
    long long id;
    string account_name;
    string email;
    string password_hash;
    string permissions;
    string created_at;
    string updated_at;
};

struct Airport {
    string id;
    string name;
    int capacity;
    string created_at;
};

struct Staff {
    long long id;
    string name;
    string current_location;
    string role;
    string schedule;
    string created_at;
};

struct Passenger {
    long long id;
    string frequent_flyer_code;
    string title;
    string first_name;
    string last_name;
    string birthdate;
    string contact_email;
    string emergency_contact_name;
    string emergency_contact_email;
    long long associated_to;
    string created_at;
    string updated_at;
};

struct Flight {
    string id;
    string departure_airport_id;
    string arrival_airport_id;
    double base_ticket_price;
    string flight_time;
    string departure;   
    string frequency;   
    string created_at;
};

struct Airplane {
    string id;
    string model;
    string location;
};

struct SeatClass {
    long long id;
    string class_name;
    int amt_of_seats;
    double markup_price;
    string airplane_id;
    string created_at;
};

struct Booking {
    long long id;
    long long passenger_id;
    string flight_id;
    long long account_id;
    double calculated_price;
    string payment_option;
    string payment_detail;
    BookingStatus booking_status;
    string created_at;
    string updated_at;
};

struct FlightStaff {
    string flight_id;
    long long staff_id;
};

struct AirportFlight {
    string airport_id;
    string flight_id;
    string relationship_type;
};

//GLOBAL DATA STORES
    vector<Account>       accounts;
    vector<Airport>       airports;
    vector<Staff>         staffs;
    vector<Passenger>     passengers;
    vector<Flight>        flights;
    vector<Airplane>      airplanes;
    vector<SeatClass>     seatClasses;
    vector<Booking>       bookings;
    vector<FlightStaff>   flightStaffs;
    vector<AirportFlight> airportFlights;


//UTILITIES
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause() {
    cout << "\n  Press Enter to continue...";
    cin.get();
}

void printHeader(const string& title) {
    cout << "\n";
    cout << "  *------------------------------------------*\n";
    cout << "  |  " << left << setw(42) << title << "|\n";
    cout << "  *------------------------------------------*\n\n";
}

void printDivider() {
    cout << "  ------------------------------------------\n";
}

string getInput(const string& prompt) {
    string val;
    cout << "  " << prompt;
    getline(cin, val);
    return val;
}

long long getLLInput(const string& prompt) {
    string line = getInput(prompt);
    try { return stoll(line); } catch (...) { return -1; }
}

int getIntInput(const string& prompt) {
    string line = getInput(prompt);
    try { return stoi(line); } catch (...) { return 0; }
}

double getDoubleInput(const string& prompt) {
    string line = getInput(prompt);
    try { return stod(line); } catch (...) { return 0.0; }
}

// Escape/unescape pipe characters in stored strings
string escape(const string& s) {
    string r = s;
    size_t pos = 0;
    while ((pos = r.find('|', pos)) != string::npos) { r.replace(pos, 1, "[PIPE]"); pos += 6; }
    return r;
}

string unescape(const string& s) {
    string r = s;
    size_t pos = 0;
    while ((pos = r.find("[PIPE]", pos)) != string::npos) { r.replace(pos, 6, "|"); pos += 1; }
    return r;
}

string ts(const string& s) { return escape(s); }

vector<string> splitLine(const string& line) {
    vector<string> tokens;
    stringstream ss(line);
    string token;
    while (getline(ss, token, '|')) tokens.push_back(unescape(token));
    return tokens;
}


//FILE PATHS
const string FILE_ACCOUNTS      = "accounts.txt";
const string FILE_AIRPORTS      = "airports.txt";
const string FILE_STAFFS        = "staffs.txt";
const string FILE_PASSENGERS    = "passengers.txt";
const string FILE_FLIGHTS       = "flights.txt";
const string FILE_AIRPLANES     = "airplanes.txt";
const string FILE_SEATCLASSES   = "seat_classes.txt";
const string FILE_BOOKINGS      = "bookings.txt";
const string FILE_FLIGHTSTAFF   = "flight_staff.txt";
const string FILE_AIRPORTFLIGHT = "airport_flight.txt";

//SAVE/LOAD FUNCTIONS
void saveAccounts() {
    ofstream f(FILE_ACCOUNTS);
    for (auto& a : accounts)
        f << a.id << "|" << ts(a.account_name) << "|" << ts(a.email) << "|"
          << ts(a.password_hash) << "|" << ts(a.permissions) << "|"
          << ts(a.created_at) << "|" << ts(a.updated_at) << "\n";
}
void loadAccounts() {
    accounts.clear();
    ifstream f(FILE_ACCOUNTS); string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        auto t = splitLine(line); if (t.size() < 7) continue;
        Account a; a.id=stoll(t[0]); a.account_name=t[1]; a.email=t[2];
        a.password_hash=t[3]; a.permissions=t[4]; a.created_at=t[5]; a.updated_at=t[6];
        accounts.push_back(a);
    }
}

void saveAirports() {
    ofstream f(FILE_AIRPORTS);
    for (auto& a : airports)
        f << ts(a.id) << "|" << ts(a.name) << "|" << a.capacity << "|" << ts(a.created_at) << "\n";
}
void loadAirports() {
    airports.clear();
    ifstream f(FILE_AIRPORTS); string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        auto t = splitLine(line); if (t.size() < 4) continue;
        Airport a; a.id=t[0]; a.name=t[1]; a.capacity=stoi(t[2]); a.created_at=t[3];
        airports.push_back(a);
    }
}

void saveStaffs() {
    ofstream f(FILE_STAFFS);
    for (auto& s : staffs)
        f << s.id << "|" << ts(s.name) << "|" << ts(s.current_location) << "|"
          << ts(s.role) << "|" << ts(s.schedule) << "|" << ts(s.created_at) << "\n";
}
void loadStaffs() {
    staffs.clear();
    ifstream f(FILE_STAFFS); string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        auto t = splitLine(line); if (t.size() < 6) continue;
        Staff s; s.id=stoll(t[0]); s.name=t[1]; s.current_location=t[2];
        s.role=t[3]; s.schedule=t[4]; s.created_at=t[5];
        staffs.push_back(s);
    }
}

void savePassengers() {
    ofstream f(FILE_PASSENGERS);
    for (auto& p : passengers)
        f << p.id << "|" << ts(p.frequent_flyer_code) << "|" << ts(p.title) << "|"
          << ts(p.first_name) << "|" << ts(p.last_name) << "|" << ts(p.birthdate) << "|"
          << ts(p.contact_email) << "|" << ts(p.emergency_contact_name) << "|"
          << ts(p.emergency_contact_email) << "|" << p.associated_to << "|"
          << ts(p.created_at) << "|" << ts(p.updated_at) << "\n";
}
void loadPassengers() {
    passengers.clear();
    ifstream f(FILE_PASSENGERS); string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        auto t = splitLine(line); if (t.size() < 12) continue;
        Passenger p; p.id=stoll(t[0]); p.frequent_flyer_code=t[1]; p.title=t[2];
        p.first_name=t[3]; p.last_name=t[4]; p.birthdate=t[5]; p.contact_email=t[6];
        p.emergency_contact_name=t[7]; p.emergency_contact_email=t[8];
        p.associated_to=stoll(t[9]); p.created_at=t[10]; p.updated_at=t[11];
        passengers.push_back(p);
    }
}

void saveFlights() {
    ofstream f(FILE_FLIGHTS);
    for (auto& fl : flights)
        f << ts(fl.id) << "|" << ts(fl.departure_airport_id) << "|" << ts(fl.arrival_airport_id) << "|"
          << fixed << setprecision(2) << fl.base_ticket_price << "|" << ts(fl.flight_time) << "|"
          << ts(fl.departure) << "|" << ts(fl.frequency) << "|" << ts(fl.created_at) << "\n";
}
void loadFlights() {
    flights.clear();
    ifstream f(FILE_FLIGHTS); string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        auto t = splitLine(line); if (t.size() < 8) continue;
        Flight fl; fl.id=t[0]; fl.departure_airport_id=t[1]; fl.arrival_airport_id=t[2];
        fl.base_ticket_price=stod(t[3]); fl.flight_time=t[4]; fl.departure=t[5];
        fl.frequency=t[6]; fl.created_at=t[7];
        flights.push_back(fl);
    }
}

void saveAirplanes() {
    ofstream f(FILE_AIRPLANES);
    for (auto& a : airplanes)
        f << ts(a.id) << "|" << ts(a.model) << "|" << ts(a.location) << "\n";
}
void loadAirplanes() {
    airplanes.clear();
    ifstream f(FILE_AIRPLANES); string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        auto t = splitLine(line); if (t.size() < 3) continue;
        Airplane a; a.id=t[0]; a.model=t[1]; a.location=t[2];
        airplanes.push_back(a);
    }
}

void saveSeatClasses() {
    ofstream f(FILE_SEATCLASSES);
    for (auto& sc : seatClasses)
        f << sc.id << "|" << ts(sc.class_name) << "|" << sc.amt_of_seats << "|"
          << fixed << setprecision(2) << sc.markup_price << "|"
          << ts(sc.airplane_id) << "|" << ts(sc.created_at) << "\n";
}
void loadSeatClasses() {
    seatClasses.clear();
    ifstream f(FILE_SEATCLASSES); string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        auto t = splitLine(line); if (t.size() < 6) continue;
        SeatClass sc; sc.id=stoll(t[0]); sc.class_name=t[1]; sc.amt_of_seats=stoi(t[2]);
        sc.markup_price=stod(t[3]); sc.airplane_id=t[4]; sc.created_at=t[5];
        seatClasses.push_back(sc);
    }
}

void saveBookings() {
    ofstream f(FILE_BOOKINGS);
    for (auto& b : bookings)
        f << b.id << "|" << b.passenger_id << "|" << ts(b.flight_id) << "|"
          << b.account_id << "|" << fixed << setprecision(2) << b.calculated_price << "|"
          << ts(b.payment_option) << "|" << ts(b.payment_detail) << "|"
          << ts(bookingStatusToStr(b.booking_status)) << "|"
          << ts(b.created_at) << "|" << ts(b.updated_at) << "\n";
}
void loadBookings() {
    bookings.clear();
    ifstream f(FILE_BOOKINGS); string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        auto t = splitLine(line); if (t.size() < 10) continue;
        Booking b; b.id=stoll(t[0]); b.passenger_id=stoll(t[1]); b.flight_id=t[2];
        b.account_id=stoll(t[3]); b.calculated_price=stod(t[4]); b.payment_option=t[5];
        b.payment_detail=t[6]; b.booking_status=strToBookingStatus(t[7]);
        b.created_at=t[8]; b.updated_at=t[9];
        bookings.push_back(b);
    }
}

void saveFlightStaffs() {
    ofstream f(FILE_FLIGHTSTAFF);
    for (auto& fs : flightStaffs)
        f << ts(fs.flight_id) << "|" << fs.staff_id << "\n";
}
void loadFlightStaffs() {
    flightStaffs.clear();
    ifstream f(FILE_FLIGHTSTAFF); string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        auto t = splitLine(line); if (t.size() < 2) continue;
        FlightStaff fs; fs.flight_id=t[0]; fs.staff_id=stoll(t[1]);
        flightStaffs.push_back(fs);
    }
}

void saveAirportFlights() {
    ofstream f(FILE_AIRPORTFLIGHT);
    for (auto& af : airportFlights)
        f << ts(af.airport_id) << "|" << ts(af.flight_id) << "|" << ts(af.relationship_type) << "\n";
}
void loadAirportFlights() {
    airportFlights.clear();
    ifstream f(FILE_AIRPORTFLIGHT); string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        auto t = splitLine(line); if (t.size() < 3) continue;
        AirportFlight af; af.airport_id=t[0]; af.flight_id=t[1]; af.relationship_type=t[2];
        airportFlights.push_back(af);
    }
}

void loadAll() {
    loadAccounts(); loadAirports(); loadStaffs(); loadPassengers();
    loadFlights(); loadAirplanes(); loadSeatClasses(); loadBookings();
    loadFlightStaffs(); loadAirportFlights();
}

//AUTO-ID HELPERS
long long nextAccountId()   { long long mx=0; for(auto&a:accounts)    mx=max(mx,a.id);  return mx+1; }
long long nextStaffId()     { long long mx=0; for(auto&s:staffs)      mx=max(mx,s.id);  return mx+1; }
long long nextPassengerId() { long long mx=0; for(auto&p:passengers)  mx=max(mx,p.id);  return mx+1; }
long long nextSeatClassId() { long long mx=0; for(auto&s:seatClasses) mx=max(mx,s.id); return mx+1; }
long long nextBookingId()   { long long mx=0; for(auto&b:bookings)    mx=max(mx,b.id);  return mx+1; }


//ACCOUNT CRUD
void viewAccounts() {
    printHeader("ACCOUNTS");
    if (accounts.empty()) { cout << "  No records found.\n"; return; }
    cout << "  " << left << setw(5)<<"ID" << setw(22)<<"Name" << setw(30)<<"Email" << setw(15)<<"Permissions" << "\n";
    printDivider();
    for (auto& a : accounts)
        cout << "  " << setw(5)<<a.id << setw(22)<<a.account_name << setw(30)<<a.email << setw(15)<<a.permissions << "\n";
}
void addAccount() {
    printHeader("ADD ACCOUNT");
    Account a;
    a.id            = nextAccountId();
    a.account_name  = getInput("Account Name: ");
    a.email         = getInput("Email: ");
    a.password_hash = getInput("Password Hash: ");
    a.permissions   = getInput("Permissions (JSON string, e.g. {}): ");
    a.created_at = a.updated_at = "NOW()";
    accounts.push_back(a); saveAccounts();
    cout << "\n  [OK] Account added with ID " << a.id << "\n";
}
void modifyAccount() {
    printHeader("MODIFY ACCOUNT");
    long long id = getLLInput("Enter Account ID to modify: ");
    for (auto& a : accounts) {
        if (a.id == id) {
            cout << "  Leave blank to keep current value.\n\n";
            string v;
            v=getInput("New Name ["+a.account_name+"]: ");    if(!v.empty()) a.account_name  =v;
            v=getInput("New Email ["+a.email+"]: ");          if(!v.empty()) a.email          =v;
            v=getInput("New Password Hash [***]: ");          if(!v.empty()) a.password_hash  =v;
            v=getInput("New Permissions ["+a.permissions+"]: "); if(!v.empty()) a.permissions =v;
            a.updated_at="NOW()"; saveAccounts();
            cout << "\n  [OK] Account updated.\n"; return;
        }
    }
    cout << "\n  [!!] Account not found.\n";
}
void deleteAccount() {
    printHeader("DELETE ACCOUNT");
    long long id = getLLInput("Enter Account ID to delete: ");
    auto it = remove_if(accounts.begin(), accounts.end(), [id](const Account& a){ return a.id==id; });
    if (it != accounts.end()) { accounts.erase(it, accounts.end()); saveAccounts(); cout << "\n  [OK] Account deleted.\n"; }
    else cout << "\n  [!!] Account not found.\n";
}


//AIRPORT CRUD
void viewAirports() {
    printHeader("AIRPORTS");
    if (airports.empty()) { cout << "  No records found.\n"; return; }
    cout << "  " << left << setw(12)<<"ID" << setw(32)<<"Name" << setw(10)<<"Capacity" << "\n";
    printDivider();
    for (auto& a : airports)
        cout << "  " << setw(12)<<a.id << setw(32)<<a.name << setw(10)<<a.capacity << "\n";
}
void addAirport() {
    printHeader("ADD AIRPORT");
    Airport a;
    a.id       = getInput("Airport ID (e.g. MNL): ");
    a.name     = getInput("Airport Name: ");
    a.capacity = getIntInput("Capacity: ");
    a.created_at = "NOW()";
    airports.push_back(a); saveAirports();
    cout << "\n  [OK] Airport added.\n";
}
void modifyAirport() {
    printHeader("MODIFY AIRPORT");
    string id = getInput("Enter Airport ID to modify: ");
    for (auto& a : airports) {
        if (a.id == id) {
            string v;
            v=getInput("New Name ["+a.name+"]: "); if(!v.empty()) a.name=v;
            string c=getInput("New Capacity ["+to_string(a.capacity)+"]: ");
            if(!c.empty()) a.capacity=stoi(c);
            saveAirports(); cout << "\n  [OK] Airport updated.\n"; return;
        }
    }
    cout << "\n  [!!] Airport not found.\n";
}
void deleteAirport() {
    printHeader("DELETE AIRPORT");
    string id = getInput("Enter Airport ID to delete: ");
    auto it = remove_if(airports.begin(), airports.end(), [&id](const Airport& a){ return a.id==id; });
    if (it != airports.end()) { airports.erase(it, airports.end()); saveAirports(); cout << "\n  [OK] Airport deleted.\n"; }
    else cout << "\n  [!!] Airport not found.\n";
}


//STAFF CRUD
void viewStaffs() {
    printHeader("STAFF");
    if (staffs.empty()) { cout << "  No records found.\n"; return; }
    cout << "  " << left << setw(5)<<"ID" << setw(22)<<"Name" << setw(12)<<"Location" << setw(20)<<"Role" << setw(20)<<"Schedule" << "\n";
    printDivider();
    for (auto& s : staffs)
        cout << "  " << setw(5)<<s.id << setw(22)<<s.name << setw(12)<<s.current_location << setw(20)<<s.role << setw(20)<<s.schedule << "\n";
}
void addStaff() {
    printHeader("ADD STAFF");
    Staff s;
    s.id               = nextStaffId();
    s.name             = getInput("Name: ");
    s.current_location = getInput("Current Location (Airport ID): ");
    s.role             = getInput("Role: ");
    s.schedule         = getInput("Schedule (JSON string, e.g. {}): ");
    s.created_at       = "NOW()";
    staffs.push_back(s); saveStaffs();
    cout << "\n  [OK] Staff added with ID " << s.id << "\n";
}
void modifyStaff() {
    printHeader("MODIFY STAFF");
    long long id = getLLInput("Enter Staff ID to modify: ");
    for (auto& s : staffs) {
        if (s.id == id) {
            string v;
            v=getInput("New Name ["+s.name+"]: ");                   if(!v.empty()) s.name=v;
            v=getInput("New Location ["+s.current_location+"]: ");   if(!v.empty()) s.current_location=v;
            v=getInput("New Role ["+s.role+"]: ");                   if(!v.empty()) s.role=v;
            v=getInput("New Schedule ["+s.schedule+"]: ");           if(!v.empty()) s.schedule=v;
            saveStaffs(); cout << "\n  [OK] Staff updated.\n"; return;
        }
    }
    cout << "\n  [!!] Staff not found.\n";
}
void deleteStaff() {
    printHeader("DELETE STAFF");
    long long id = getLLInput("Enter Staff ID to delete: ");
    auto it = remove_if(staffs.begin(), staffs.end(), [id](const Staff& s){ return s.id==id; });
    if (it != staffs.end()) { staffs.erase(it, staffs.end()); saveStaffs(); cout << "\n  [OK] Staff deleted.\n"; }
    else cout << "\n  [!!] Staff not found.\n";
}


//PASSENGER CRUD
void viewPassengers() {
    printHeader("PASSENGERS");
    if (passengers.empty()) { cout << "  No records found.\n"; return; }
    cout << "  " << left << setw(5)<<"ID" << setw(8)<<"Title" << setw(15)<<"First" << setw(15)<<"Last"
         << setw(12)<<"Birthdate" << setw(26)<<"Email" << setw(10)<<"AcctID" << "\n";
    printDivider();
    for (auto& p : passengers)
        cout << "  " << setw(5)<<p.id << setw(8)<<p.title << setw(15)<<p.first_name << setw(15)<<p.last_name
             << setw(12)<<p.birthdate << setw(26)<<p.contact_email << setw(10)<<p.associated_to << "\n";
}
void addPassenger() {
    printHeader("ADD PASSENGER");
    Passenger p;
    p.id                      = nextPassengerId();
    p.frequent_flyer_code     = getInput("Frequent Flyer Code (blank if none): ");
    p.title                   = getInput("Title (Mr/Ms/Dr...): ");
    p.first_name              = getInput("First Name: ");
    p.last_name               = getInput("Last Name: ");
    p.birthdate               = getInput("Birthdate (YYYY-MM-DD): ");
    p.contact_email           = getInput("Contact Email: ");
    p.emergency_contact_name  = getInput("Emergency Contact Name: ");
    p.emergency_contact_email = getInput("Emergency Contact Email: ");
    p.associated_to           = getLLInput("Associated Account ID: ");
    p.created_at = p.updated_at = "NOW()";
    passengers.push_back(p); savePassengers();
    cout << "\n  [OK] Passenger added with ID " << p.id << "\n";
}
void modifyPassenger() {
    printHeader("MODIFY PASSENGER");
    long long id = getLLInput("Enter Passenger ID to modify: ");
    for (auto& p : passengers) {
        if (p.id == id) {
            string v;
            v=getInput("New FFC ["+p.frequent_flyer_code+"]: ");              if(!v.empty()) p.frequent_flyer_code=v;
            v=getInput("New Title ["+p.title+"]: ");                          if(!v.empty()) p.title=v;
            v=getInput("New First Name ["+p.first_name+"]: ");                if(!v.empty()) p.first_name=v;
            v=getInput("New Last Name ["+p.last_name+"]: ");                  if(!v.empty()) p.last_name=v;
            v=getInput("New Birthdate ["+p.birthdate+"]: ");                  if(!v.empty()) p.birthdate=v;
            v=getInput("New Email ["+p.contact_email+"]: ");                  if(!v.empty()) p.contact_email=v;
            v=getInput("New Emrg Name ["+p.emergency_contact_name+"]: ");     if(!v.empty()) p.emergency_contact_name=v;
            v=getInput("New Emrg Email ["+p.emergency_contact_email+"]: ");   if(!v.empty()) p.emergency_contact_email=v;
            p.updated_at="NOW()"; savePassengers();
            cout << "\n  [OK] Passenger updated.\n"; return;
        }
    }
    cout << "\n  [!!] Passenger not found.\n";
}
void deletePassenger() {
    printHeader("DELETE PASSENGER");
    long long id = getLLInput("Enter Passenger ID to delete: ");
    auto it = remove_if(passengers.begin(), passengers.end(), [id](const Passenger& p){ return p.id==id; });
    if (it != passengers.end()) { passengers.erase(it, passengers.end()); savePassengers(); cout << "\n  [OK] Passenger deleted.\n"; }
    else cout << "\n  [!!] Passenger not found.\n";
}

//FLIGHT CRUD
void viewFlights() {
    printHeader("FLIGHTS");
    if (flights.empty()) { cout << "  No records found.\n"; return; }
    cout << "  " << left << setw(12)<<"ID" << setw(10)<<"Dep.AP" << setw(10)<<"Arr.AP"
         << setw(12)<<"Price" << setw(12)<<"Duration" << setw(22)<<"Departure" << setw(12)<<"Frequency" << "\n";
    printDivider();
    for (auto& fl : flights)
        cout << "  " << setw(12)<<fl.id << setw(10)<<fl.departure_airport_id << setw(10)<<fl.arrival_airport_id
             << setw(12)<<fixed<<setprecision(2)<<fl.base_ticket_price << setw(12)<<fl.flight_time
             << setw(22)<<fl.departure << setw(12)<<fl.frequency << "\n";
}
void addFlight() {
    printHeader("ADD FLIGHT");
    Flight fl;
    fl.id                   = getInput("Flight ID (e.g. PR101): ");
    fl.departure_airport_id = getInput("Departure Airport ID: ");
    fl.arrival_airport_id   = getInput("Arrival Airport ID: ");
    fl.base_ticket_price    = getDoubleInput("Base Ticket Price: ");
    fl.flight_time          = getInput("Flight Time (e.g. 02:30:00): ");
    fl.departure            = getInput("Departure (YYYY-MM-DD HH:MM:SS): ");
    fl.frequency            = getInput("Frequency (e.g. 7 days): ");
    fl.created_at           = "NOW()";
    flights.push_back(fl); saveFlights();
    cout << "\n  [OK] Flight added.\n";
}
void modifyFlight() {
    printHeader("MODIFY FLIGHT");
    string id = getInput("Enter Flight ID to modify: ");
    for (auto& fl : flights) {
        if (fl.id == id) {
            string v;
            v=getInput("New Dep Airport ["+fl.departure_airport_id+"]: "); if(!v.empty()) fl.departure_airport_id=v;
            v=getInput("New Arr Airport ["+fl.arrival_airport_id+"]: ");   if(!v.empty()) fl.arrival_airport_id=v;
            string pr=getInput("New Price ["+to_string(fl.base_ticket_price)+"]: ");
            if(!pr.empty()) fl.base_ticket_price=stod(pr);
            v=getInput("New Flight Time ["+fl.flight_time+"]: ");           if(!v.empty()) fl.flight_time=v;
            v=getInput("New Departure ["+fl.departure+"]: ");               if(!v.empty()) fl.departure=v;
            v=getInput("New Frequency ["+fl.frequency+"]: ");               if(!v.empty()) fl.frequency=v;
            saveFlights(); cout << "\n  [OK] Flight updated.\n"; return;
        }
    }
    cout << "\n  [!!] Flight not found.\n";
}
void deleteFlight() {
    printHeader("DELETE FLIGHT");
    string id = getInput("Enter Flight ID to delete: ");
    auto it = remove_if(flights.begin(), flights.end(), [&id](const Flight& fl){ return fl.id==id; });
    if (it != flights.end()) { flights.erase(it, flights.end()); saveFlights(); cout << "\n  [OK] Flight deleted.\n"; }
    else cout << "\n  [!!] Flight not found.\n";
}


//AIRPLANE CRUD
void viewAirplanes() {
    printHeader("AIRPLANES");
    if (airplanes.empty()) { cout << "  No records found.\n"; return; }
    cout << "  " << left << setw(15)<<"ID" << setw(28)<<"Model" << setw(12)<<"Location" << "\n";
    printDivider();
    for (auto& a : airplanes)
        cout << "  " << setw(15)<<a.id << setw(28)<<a.model << setw(12)<<a.location << "\n";
}
void addAirplane() {
    printHeader("ADD AIRPLANE");
    Airplane a;
    a.id       = getInput("Airplane ID (e.g. RP-C8888): ");
    a.model    = getInput("Model (e.g. Boeing 737): ");
    a.location = getInput("Location (Airport ID, or blank): ");
    airplanes.push_back(a); saveAirplanes();
    cout << "\n  [OK] Airplane added.\n";
}
void modifyAirplane() {
    printHeader("MODIFY AIRPLANE");
    string id = getInput("Enter Airplane ID to modify: ");
    for (auto& a : airplanes) {
        if (a.id == id) {
            string v;
            v=getInput("New Model ["+a.model+"]: ");     if(!v.empty()) a.model=v;
            v=getInput("New Location ["+a.location+"]: ");if(!v.empty()) a.location=v;
            saveAirplanes(); cout << "\n  [OK] Airplane updated.\n"; return;
        }
    }
    cout << "\n  [!!] Airplane not found.\n";
}
void deleteAirplane() {
    printHeader("DELETE AIRPLANE");
    string id = getInput("Enter Airplane ID to delete: ");
    auto it = remove_if(airplanes.begin(), airplanes.end(), [&id](const Airplane& a){ return a.id==id; });
    if (it != airplanes.end()) { airplanes.erase(it, airplanes.end()); saveAirplanes(); cout << "\n  [OK] Airplane deleted.\n"; }
    else cout << "\n  [!!] Airplane not found.\n";
}


//SEAT CLASS CRUD
void viewSeatClasses() {
    printHeader("SEAT CLASSES");
    if (seatClasses.empty()) { cout << "  No records found.\n"; return; }
    cout << "  " << left << setw(5)<<"ID" << setw(20)<<"Class Name" << setw(10)<<"# Seats" << setw(14)<<"Markup" << setw(15)<<"Airplane ID" << "\n";
    printDivider();
    for (auto& sc : seatClasses)
        cout << "  " << setw(5)<<sc.id << setw(20)<<sc.class_name << setw(10)<<sc.amt_of_seats
             << setw(14)<<fixed<<setprecision(2)<<sc.markup_price << setw(15)<<sc.airplane_id << "\n";
}
void addSeatClass() {
    printHeader("ADD SEAT CLASS");
    SeatClass sc;
    sc.id           = nextSeatClassId();
    sc.class_name   = getInput("Class Name (e.g. Economy): ");
    sc.amt_of_seats = getIntInput("Number of Seats: ");
    sc.markup_price = getDoubleInput("Markup Price: ");
    sc.airplane_id  = getInput("Airplane ID: ");
    sc.created_at   = "NOW()";
    seatClasses.push_back(sc); saveSeatClasses();
    cout << "\n  [OK] Seat Class added with ID " << sc.id << "\n";
}
void modifySeatClass() {
    printHeader("MODIFY SEAT CLASS");
    long long id = getLLInput("Enter Seat Class ID to modify: ");
    for (auto& sc : seatClasses) {
        if (sc.id == id) {
            string v;
            v=getInput("New Class Name ["+sc.class_name+"]: "); if(!v.empty()) sc.class_name=v;
            string s=getInput("New # Seats ["+to_string(sc.amt_of_seats)+"]: ");
            if(!s.empty()) sc.amt_of_seats=stoi(s);
            string m=getInput("New Markup Price ["+to_string(sc.markup_price)+"]: ");
            if(!m.empty()) sc.markup_price=stod(m);
            v=getInput("New Airplane ID ["+sc.airplane_id+"]: "); if(!v.empty()) sc.airplane_id=v;
            saveSeatClasses(); cout << "\n  [OK] Seat Class updated.\n"; return;
        }
    }
    cout << "\n  [!!] Seat Class not found.\n";
}
void deleteSeatClass() {
    printHeader("DELETE SEAT CLASS");
    long long id = getLLInput("Enter Seat Class ID to delete: ");
    auto it = remove_if(seatClasses.begin(), seatClasses.end(), [id](const SeatClass& sc){ return sc.id==id; });
    if (it != seatClasses.end()) { seatClasses.erase(it, seatClasses.end()); saveSeatClasses(); cout << "\n  [OK] Seat Class deleted.\n"; }
    else cout << "\n  [!!] Seat Class not found.\n";
}


//  BOOKING CRUD
void viewBookings() {
    printHeader("BOOKINGS");
    if (bookings.empty()) { cout << "  No records found.\n"; return; }
    cout << "  " << left << setw(5)<<"ID" << setw(11)<<"Passenger" << setw(12)<<"Flight"
         << setw(9)<<"Account" << setw(12)<<"Price" << setw(12)<<"Payment" << setw(12)<<"Status" << "\n";
    printDivider();
    for (auto& b : bookings)
        cout << "  " << setw(5)<<b.id << setw(11)<<b.passenger_id << setw(12)<<b.flight_id
             << setw(9)<<(b.account_id==-1?"N/A":to_string(b.account_id))
             << setw(12)<<fixed<<setprecision(2)<<b.calculated_price
             << setw(12)<<b.payment_option << setw(12)<<bookingStatusToStr(b.booking_status) << "\n";
}
void addBooking() {
    printHeader("ADD BOOKING");
    Booking b;
    b.id               = nextBookingId();
    b.passenger_id     = getLLInput("Passenger ID: ");
    b.flight_id        = getInput("Flight ID: ");
    string ac          = getInput("Account ID (blank if none): ");
    b.account_id       = ac.empty() ? -1 : stoll(ac);
    b.calculated_price = getDoubleInput("Calculated Price: ");
    b.payment_option   = getInput("Payment Option (cash/card/etc): ");
    b.payment_detail   = getInput("Payment Detail (JSON string, e.g. {}): ");
    cout << "  Status [1=pending, 2=confirmed, 3=cancelled]: ";
    string sc; getline(cin, sc);
    if (sc=="2") b.booking_status=BookingStatus::CONFIRMED;
    else if(sc=="3") b.booking_status=BookingStatus::CANCELLED;
    else b.booking_status=BookingStatus::PENDING;
    b.created_at=b.updated_at="NOW()";
    bookings.push_back(b); saveBookings();
    cout << "\n  [OK] Booking added with ID " << b.id << "\n";
}
void modifyBooking() {
    printHeader("MODIFY BOOKING");
    long long id = getLLInput("Enter Booking ID to modify: ");
    for (auto& b : bookings) {
        if (b.id == id) {
            string v;
            v=getInput("New Payment Option ["+b.payment_option+"]: "); if(!v.empty()) b.payment_option=v;
            v=getInput("New Payment Detail ["+b.payment_detail+"]: "); if(!v.empty()) b.payment_detail=v;
            cout << "  New Status [1=pending, 2=confirmed, 3=cancelled] (current: "
                 << bookingStatusToStr(b.booking_status) << "): ";
            string sc; getline(cin, sc);
            if(sc=="1") b.booking_status=BookingStatus::PENDING;
            else if(sc=="2") b.booking_status=BookingStatus::CONFIRMED;
            else if(sc=="3") b.booking_status=BookingStatus::CANCELLED;
            b.updated_at="NOW()"; saveBookings();
            cout << "\n  [OK] Booking updated.\n"; return;
        }
    }
    cout << "\n  [!!] Booking not found.\n";
}
void deleteBooking() {
    printHeader("DELETE BOOKING");
    long long id = getLLInput("Enter Booking ID to delete: ");
    auto it = remove_if(bookings.begin(), bookings.end(), [id](const Booking& b){ return b.id==id; });
    if (it != bookings.end()) { bookings.erase(it, bookings.end()); saveBookings(); cout << "\n  [OK] Booking deleted.\n"; }
    else cout << "\n  [!!] Booking not found.\n";
}


//  FLIGHT STAFF CRUD
void viewFlightStaffs() {
    printHeader("FLIGHT STAFF (Junction)");
    if (flightStaffs.empty()) { cout << "  No records found.\n"; return; }
    cout << "  " << left << setw(16)<<"Flight ID" << setw(12)<<"Staff ID" << "\n";
    printDivider();
    for (auto& fs : flightStaffs)
        cout << "  " << setw(16)<<fs.flight_id << setw(12)<<fs.staff_id << "\n";
}
void addFlightStaff() {
    printHeader("ADD FLIGHT STAFF");
    FlightStaff fs;
    fs.flight_id = getInput("Flight ID: ");
    fs.staff_id  = getLLInput("Staff ID: ");
    flightStaffs.push_back(fs); saveFlightStaffs();
    cout << "\n  [OK] Flight-Staff link added.\n";
}
void modifyFlightStaff() {
    printHeader("MODIFY FLIGHT STAFF");
    string fid    = getInput("Flight ID of record to find: ");
    long long sid = getLLInput("Staff ID of record to find: ");
    for (auto& fs : flightStaffs) {
        if (fs.flight_id==fid && fs.staff_id==sid) {
            string v=getInput("New Flight ID ["+fs.flight_id+"]: "); if(!v.empty()) fs.flight_id=v;
            string ns=getInput("New Staff ID ["+to_string(fs.staff_id)+"]: ");
            if(!ns.empty()) fs.staff_id=stoll(ns);
            saveFlightStaffs(); cout << "\n  [OK] Record updated.\n"; return;
        }
    }
    cout << "\n  [!!] Record not found.\n";
}
void deleteFlightStaff() {
    printHeader("DELETE FLIGHT STAFF");
    string fid    = getInput("Flight ID: ");
    long long sid = getLLInput("Staff ID: ");
    auto it = remove_if(flightStaffs.begin(), flightStaffs.end(),
        [&fid,sid](const FlightStaff& fs){ return fs.flight_id==fid && fs.staff_id==sid; });
    if (it != flightStaffs.end()) { flightStaffs.erase(it,flightStaffs.end()); saveFlightStaffs(); cout << "\n  [OK] Record deleted.\n"; }
    else cout << "\n  [!!] Record not found.\n";
}


//  AIRPORT FLIGHT CRUD
void viewAirportFlights() {
    printHeader("AIRPORT FLIGHT (Junction)");
    if (airportFlights.empty()) { cout << "  No records found.\n"; return; }
    cout << "  " << left << setw(14)<<"Airport ID" << setw(14)<<"Flight ID" << setw(20)<<"Relationship" << "\n";
    printDivider();
    for (auto& af : airportFlights)
        cout << "  " << setw(14)<<af.airport_id << setw(14)<<af.flight_id << setw(20)<<af.relationship_type << "\n";
}
void addAirportFlight() {
    printHeader("ADD AIRPORT FLIGHT");
    AirportFlight af;
    af.airport_id        = getInput("Airport ID: ");
    af.flight_id         = getInput("Flight ID: ");
    af.relationship_type = getInput("Relationship Type (e.g. departure/arrival): ");
    airportFlights.push_back(af); saveAirportFlights();
    cout << "\n  [OK] Airport-Flight link added.\n";
}
void modifyAirportFlight() {
    printHeader("MODIFY AIRPORT FLIGHT");
    string aid = getInput("Airport ID of record to find: ");
    string fid = getInput("Flight ID of record to find: ");
    string rel = getInput("Relationship Type of record to find: ");
    for (auto& af : airportFlights) {
        if (af.airport_id==aid && af.flight_id==fid && af.relationship_type==rel) {
            string v;
            v=getInput("New Airport ID ["+af.airport_id+"]: ");           if(!v.empty()) af.airport_id=v;
            v=getInput("New Flight ID ["+af.flight_id+"]: ");             if(!v.empty()) af.flight_id=v;
            v=getInput("New Relationship Type ["+af.relationship_type+"]: "); if(!v.empty()) af.relationship_type=v;
            saveAirportFlights(); cout << "\n  [OK] Record updated.\n"; return;
        }
    }
    cout << "\n  [!!] Record not found.\n";
}
void deleteAirportFlight() {
    printHeader("DELETE AIRPORT FLIGHT");
    string aid = getInput("Airport ID: ");
    string fid = getInput("Flight ID: ");
    string rel = getInput("Relationship Type: ");
    auto it = remove_if(airportFlights.begin(), airportFlights.end(),
        [&](const AirportFlight& af){ return af.airport_id==aid && af.flight_id==fid && af.relationship_type==rel; });
    if (it != airportFlights.end()) { airportFlights.erase(it,airportFlights.end()); saveAirportFlights(); cout << "\n  [OK] Record deleted.\n"; }
    else cout << "\n  [!!] Record not found.\n";
}


//SUB-MENU
void subMenu(const string& title,
    void(*viewFn)(), void(*addFn)(), void(*modFn)(), void(*delFn)())
{
    while (true) {
        clearScreen();
        printHeader(title + " - TABLE MENU");
        cout << "  [1] View All\n";
        cout << "  [2] Add New\n";
        cout << "  [3] Modify\n";
        cout << "  [4] Delete\n";
        cout << "  [0] Back to Main Menu\n\n";
        string ch = getInput("Choice: ");
        clearScreen();
        if      (ch=="1") { viewFn(); pause(); }
        else if (ch=="2") { addFn();  pause(); }
        else if (ch=="3") { modFn();  pause(); }
        else if (ch=="4") { delFn();  pause(); }
        else if (ch=="0") break;
        else { cout << "\n  Invalid choice.\n"; pause(); }
    }
}


//MAIN MENU
void mainMenu() {
    while (true) {
        clearScreen();
        cout << "\n";
        cout << "  *------------------------------------------*\n";
        cout << "  |      AIRLINE MANAGEMENT SYSTEM           |\n";
        cout << "  |           Main Menu                      |\n";
        cout << "  *------------------------------------------*\n\n";
        cout << "  Select a table to manage:\n\n";
        cout << "   [1]  Account\n";
        cout << "   [2]  Airport\n";
        cout << "   [3]  Staff\n";
        cout << "   [4]  Passenger\n";
        cout << "   [5]  Flight\n";
        cout << "   [6]  Airplane\n";
        cout << "   [7]  Seat Class\n";
        cout << "   [8]  Booking\n";
        cout << "   [9]  Flight Staff  (junction)\n";
        cout << "  [10]  Airport Flight (junction)\n";
        cout << "\n   [0]  Exit\n\n";
        string ch = getInput("Choice: ");
        if      (ch=="1")  subMenu("ACCOUNT",         viewAccounts,       addAccount,       modifyAccount,       deleteAccount);
        else if (ch=="2")  subMenu("AIRPORT",         viewAirports,       addAirport,       modifyAirport,       deleteAirport);
        else if (ch=="3")  subMenu("STAFF",           viewStaffs,         addStaff,         modifyStaff,         deleteStaff);
        else if (ch=="4")  subMenu("PASSENGER",       viewPassengers,     addPassenger,     modifyPassenger,     deletePassenger);
        else if (ch=="5")  subMenu("FLIGHT",          viewFlights,        addFlight,        modifyFlight,        deleteFlight);
        else if (ch=="6")  subMenu("AIRPLANE",        viewAirplanes,      addAirplane,      modifyAirplane,      deleteAirplane);
        else if (ch=="7")  subMenu("SEAT CLASS",      viewSeatClasses,    addSeatClass,     modifySeatClass,     deleteSeatClass);
        else if (ch=="8")  subMenu("BOOKING",         viewBookings,       addBooking,       modifyBooking,       deleteBooking);
        else if (ch=="9")  subMenu("FLIGHT STAFF",    viewFlightStaffs,   addFlightStaff,   modifyFlightStaff,   deleteFlightStaff);
        else if (ch=="10") subMenu("AIRPORT FLIGHT",  viewAirportFlights, addAirportFlight, modifyAirportFlight, deleteAirportFlight);
        else if (ch=="0")  { cout << "\n  Goodbye!\n\n"; break; }
        else { clearScreen(); cout << "\n  Invalid choice.\n"; pause(); }
    }
}


//PASSWORD
bool authenticate() {
    clearScreen();
    cout << "\n";
    cout << "  *------------------------------------------*\n";
    cout << "  |      AIRLINE MANAGEMENT SYSTEM           |\n";
    cout << "  |           Access Required                |\n";
    cout << "  *------------------------------------------*\n\n";
    //Password here
    const string CORRECT_PASSWORD = "12345678";
    //For Attempt
    int attempts = 3;
    while (attempts-- > 0) {
        string pass = getInput("Password: ");
        if (pass == CORRECT_PASSWORD) {
            cout << "\n  [OK] Access granted.\n";
            return true;
        }
        cout << "  [!!] Incorrect. " << attempts << " attempt(s) remaining.\n\n";
    }
    cout << "\n  Access denied. Exiting.\n\n";
    return false;
}


//MAIN
int main() {
    if (!authenticate()) return 1;
    pause();
    loadAll();
    mainMenu();
    return 0;
}