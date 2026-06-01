#pragma once
#include <string>
#include <vector>

#ifndef SKYBRIDGE_MAIN_H
#define SKYBRIDGE_MAIN_H

namespace Skybridge {
namespace Structs {
enum BookingStatus { PENDING = 0, CONFIRMED = 1, CANCELLED = -1 };
struct Account {
    long long id;
    std::string account_name;
    std::string email;
    std::string password_hash;
    std::string permissions;
    std::string created_at;
    std::string updated_at;
};

struct Airport {
    std::string id;
    std::string name;
    int capacity;
    std::string created_at;
};

struct Staff {
    long long id;
    std::string name;
    std::string current_location;
    std::string role;
    std::string schedule;
    std::string created_at;
};

struct Passenger {
    long long id;
    std::string frequent_flyer_code;
    std::string title;
    std::string first_name;
    std::string last_name;
    std::string birthdate;
    std::string contact_email;
    std::string emergency_contact_name;
    std::string emergency_contact_email;
    long long associated_to;
    std::string created_at;
    std::string updated_at;
};

struct Flight {
    std::string id;
    std::string departure_airport_id;
    std::string arrival_airport_id;
    double base_ticket_price;
    std::string flight_time;
    std::string departure;
    std::string frequency;
    std::string created_at;
};

struct Airplane {
    std::string id;
    std::string model;
    std::string location;
};

struct SeatClass {
    long long id;
    std::string class_name;
    int amt_of_seats;
    double markup_price;
    std::string airplane_id;
    std::string created_at;
};

struct Booking {
    long long id;
    long long passenger_id;
    std::string flight_id;
    long long account_id;
    double calculated_price;
    std::string payment_option;
    std::string payment_detail;
    BookingStatus booking_status;
    std::string created_at;
    std::string updated_at;
};

struct FlightStaff {
    std::string flight_id;
    long long staff_id;
};

struct AirportFlight {
    std::string airport_id;
    std::string flight_id;
    std::string relationship_type;
};
}  // namespace Structs

namespace Data {
const std::string FILE_ACCOUNTS = "accounts.txt";
const std::string FILE_AIRPORTS = "airports.txt";
const std::string FILE_STAFFS = "staffs.txt";
const std::string FILE_PASSENGERS = "passengers.txt";
const std::string FILE_FLIGHTS = "flights.txt";
const std::string FILE_AIRPLANES = "airplanes.txt";
const std::string FILE_SEATCLASSES = "seat_classes.txt";
const std::string FILE_BOOKINGS = "bookings.txt";
const std::string FILE_FLIGHTSTAFF = "flight_staff.txt";
const std::string FILE_AIRPORTFLIGHT = "airport_flight.txt";

std::vector<Structs::Account> accounts;
std::vector<Structs::Airport> airports;
std::vector<Structs::Staff> staffs;
std::vector<Structs::Passenger> passengers;
std::vector<Structs::Flight> flights;
std::vector<Structs::Airplane> airplanes;
std::vector<Structs::SeatClass> seatClasses;
std::vector<Structs::Booking> bookings;
std::vector<Structs::FlightStaff> flightStaffs;
std::vector<Structs::AirportFlight> airportFlights;
}  // namespace Data
}  // namespace Skybridge

#endif