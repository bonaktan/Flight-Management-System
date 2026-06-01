// #include <algorithm>
// #include <fstream>
// #include <iomanip>
// #include <iostream>
// #include <limits>
// #include <sstream>
// #include <string>
// #include <vector>
#include "main.h"

// MAIN
int main() {
    if (!authenticate()) return 1;
    pause();
    loadAll();
    mainMenu();
    return 0;
}

void loadAll() {
    loadAccounts();
    loadAirports();
    loadStaffs();
    loadPassengers();
    loadFlights();
    loadAirplanes();
    loadSeatClasses();
    loadBookings();
    loadFlightStaffs();
    loadAirportFlights();
}