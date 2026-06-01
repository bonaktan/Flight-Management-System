void saveBookings() {
    ofstream f(FILE_BOOKINGS);
    for (auto& b : bookings)
        f << b.id << "|" << b.passenger_id << "|" << escape(b.flight_id) << "|"
          << b.account_id << "|" << fixed << setprecision(2)
          << b.calculated_price << "|" << escape(b.payment_option) << "|"
          << escape(b.payment_detail) << "|"
          << escape(bookingStatusToStr(b.booking_status)) << "|" << escape(b.created_at)
          << "|" << escape(b.updated_at) << "\n";
}
void loadBookings() {
    bookings.clear();
    ifstream f(FILE_BOOKINGS);
    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        auto t = splitLine(line);
        if (t.size() < 10) continue;
        Booking b;
        b.id = stoll(t[0]);
        b.passenger_id = stoll(t[1]);
        b.flight_id = t[2];
        b.account_id = stoll(t[3]);
        b.calculated_price = stod(t[4]);
        b.payment_option = t[5];
        b.payment_detail = t[6];
        b.booking_status = strToBookingStatus(t[7]);
        b.created_at = t[8];
        b.updated_at = t[9];
        bookings.push_back(b);
    }
}

long long nextBookingId() {
    long long mx = 0;
    for (auto& b : bookings) mx = max(mx, b.id);
    return mx + 1;
}

void viewBookings() {
    printHeader("BOOKINGS");
    if (bookings.empty()) {
        cout << "  No records found.\n";
        return;
    }
    cout << "  " << left << setw(5) << "ID" << setw(11) << "Passenger"
         << setw(12) << "Flight" << setw(9) << "Account" << setw(12) << "Price"
         << setw(12) << "Payment" << setw(12) << "Status" << "\n";
    printDivider();
    for (auto& b : bookings)
        cout << "  " << setw(5) << b.id << setw(11) << b.passenger_id
             << setw(12) << b.flight_id << setw(9)
             << (b.account_id == -1 ? "N/A" : to_string(b.account_id))
             << setw(12) << fixed << setprecision(2) << b.calculated_price
             << setw(12) << b.payment_option << setw(12)
             << bookingStatusToStr(b.booking_status) << "\n";
}
void addBooking() {
    printHeader("ADD BOOKING");
    Booking b;
    b.id = nextBookingId();
    b.passenger_id = getLLInput("Passenger ID: ");
    b.flight_id = getInput("Flight ID: ");
    string ac = getInput("Account ID (blank if none): ");
    b.account_id = ac.empty() ? -1 : stoll(ac);
    b.calculated_price = getDoubleInput("Calculated Price: ");
    b.payment_option = getInput("Payment Option (cash/card/etc): ");
    b.payment_detail = getInput("Payment Detail (JSON string, e.g. {}): ");
    cout << "  Status [1=pending, 2=confirmed, 3=cancelled]: ";
    string sc;
    getline(cin, sc);
    if (sc == "2")
        b.booking_status = BookingStatus::CONFIRMED;
    else if (sc == "3")
        b.booking_status = BookingStatus::CANCELLED;
    else
        b.booking_status = BookingStatus::PENDING;
    b.created_at = b.updated_at = "NOW()";
    bookings.push_back(b);
    saveBookings();
    cout << "\n  [OK] Booking added with ID " << b.id << "\n";
}
void modifyBooking() {
    printHeader("MODIFY BOOKING");
    long long id = getLLInput("Enter Booking ID to modify: ");
    for (auto& b : bookings) {
        if (b.id == id) {
            string v;
            v = getInput("New Payment Option [" + b.payment_option + "]: ");
            if (!v.empty()) b.payment_option = v;
            v = getInput("New Payment Detail [" + b.payment_detail + "]: ");
            if (!v.empty()) b.payment_detail = v;
            cout << "  New Status [1=pending, 2=confirmed, 3=cancelled] "
                    "(current: "
                 << bookingStatusToStr(b.booking_status) << "): ";
            string sc;
            getline(cin, sc);
            if (sc == "1")
                b.booking_status = BookingStatus::PENDING;
            else if (sc == "2")
                b.booking_status = BookingStatus::CONFIRMED;
            else if (sc == "3")
                b.booking_status = BookingStatus::CANCELLED;
            b.updated_at = "NOW()";
            saveBookings();
            cout << "\n  [OK] Booking updated.\n";
            return;
        }
    }
    cout << "\n  [!!] Booking not found.\n";
}
void deleteBooking() {
    printHeader("DELETE BOOKING");
    long long id = getLLInput("Enter Booking ID to delete: ");
    auto it = remove_if(bookings.begin(), bookings.end(),
                        [id](const Booking& b) { return b.id == id; });
    if (it != bookings.end()) {
        bookings.erase(it, bookings.end());
        saveBookings();
        cout << "\n  [OK] Booking deleted.\n";
    } else
        cout << "\n  [!!] Booking not found.\n";
}