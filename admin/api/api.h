#pragma once

#ifndef SKYBRIDGE_API
#define SKYBRIDGE_API

#include <cpr/cpr.h>

#include <nlohmann/json.hpp>

#include "../main.h"

namespace Skybridge::API {
class ApiClient {
   private:
    static ApiClient* instance;
    cpr::Cookies cookies;
    const std::string base_url =
        "http://localhost:8080/api";  // TODO: flags for dev/prod
    // "https://skybridge.bonnybonnybonaktan.xyz/api";
    ApiClient() = default;
    void checkSession(const cpr::Cookies& updated_cookies);

   public:
    ApiClient(const ApiClient&) = delete;
    ApiClient& operator=(const ApiClient&) = delete;
    static ApiClient& getInstance();

    cpr::Response get(const std::string& endpoint);
    cpr::Response post(const std::string& endpoint, nlohmann::json payload);
    cpr::Response del(const std::string& endpoint);
    cpr::Response patch(const std::string& endpoint,
                                   nlohmann::json payload);
};

class Auth {
   private:
    static Auth* instance;
    Auth() = default;

   public:
    Auth(const Auth&) = delete;
    Auth& operator=(const Auth&) = delete;

    static Auth& getInstance() {
        if (!instance) instance = new Auth();
        return *instance;
    }

    bool login(std::string& email, std::string& password);
};

class Account {
   private:
    static Account* instance;
    Account() = default;

   public:
    Account(const Account&) = delete;
    Account& operator=(const Account&) = delete;

    static Account& getInstance() {
        if (!instance) instance = new Account();
        return *instance;
    }
    std::string name = "Account";
    std::vector<std::string> UNSUPPORTED_OPS = {"add"};
    static long long nextId();
    static std::vector<std::vector<std::string>> view();
    static std::vector<std::vector<std::string>> view_one(std::string id);
    static void add();
    static std::vector<std::vector<std::string>> modify(std::string id,
                                                        std::string field,
                                                        std::string value);
    static void remove();
};

class Airplane {
   private:
    static Airplane* instance;
    Airplane() = default;

   public:
    Airplane(const Airplane&) = delete;
    Airplane& operator=(const Airplane&) = delete;

    static Airplane& getInstance() {
        if (!instance) instance = new Airplane();
        return *instance;
    }
    std::string name = "Airplane";
    std::vector<std::string> UNSUPPORTED_OPS = {};
    static std::vector<std::vector<std::string>> view();
    static std::vector<std::vector<std::string>> view_one(std::string id);
    static void add();
    static std::vector<std::vector<std::string>> modify(std::string id,
                                                        std::string field,
                                                        std::string value);
    static void remove();
};

class Airport {
   private:
    static Airport* instance;
    Airport() = default;

   public:
    Airport(const Airport&) = delete;
    Airport& operator=(const Airport&) = delete;

    static Airport& getInstance() {
        if (!instance) instance = new Airport();
        return *instance;
    }
    std::string name = "Airport";
    std::vector<std::string> UNSUPPORTED_OPS = {};
    static std::vector<std::vector<std::string>> view();
    static std::vector<std::vector<std::string>> view_one(std::string id);
    static void add();
    static std::vector<std::vector<std::string>> modify(std::string id,
                                                        std::string field,
                                                        std::string value);
    static void remove();
};

class Booking {
   private:
    static Booking* instance;
    Booking() = default;

   public:
    Booking(const Booking&) = delete;
    Booking& operator=(const Booking&) = delete;

    static Booking& getInstance() {
        if (!instance) instance = new Booking();
        return *instance;
    }
    std::string name = "Booking";
    std::vector<std::string> UNSUPPORTED_OPS = {"add"};

    static long long nextId();
    static void view();
    static std::vector<std::vector<std::string>> view_one(std::string id);
    static void add();
    static std::vector<std::vector<std::string>> modify(std::string id,
                                                        std::string field,
                                                        std::string value);
    static void remove();
};

class Flight {
   private:
    static Flight* instance;
    Flight() = default;

   public:
    Flight(const Flight&) = delete;
    Flight& operator=(const Flight&) = delete;

    static Flight& getInstance() {
        if (!instance) instance = new Flight();
        return *instance;
    }
    std::string name = "Flight";
    std::vector<std::string> UNSUPPORTED_OPS = {"view"};
    static std::vector<std::vector<std::string>> view();
    static std::vector<std::vector<std::string>> view_one(std::string id);
    static void add();
    static std::vector<std::vector<std::string>> modify(std::string id,
                                                        std::string field,
                                                        std::string value);
    static void remove();
};

class Passenger {
   private:
    static Passenger* instance;
    Passenger() = default;

   public:
    Passenger(const Passenger&) = delete;
    Passenger& operator=(const Passenger&) = delete;

    static Passenger& getInstance() {
        if (!instance) instance = new Passenger();
        return *instance;
    }
    std::string name = "Passenger";
    std::vector<std::string> UNSUPPORTED_OPS = {"view", "add", "modify"};
    static long long nextId();
    static void view();
    static void add();
    static std::vector<std::vector<std::string>> modify(std::string id,
                                                        std::string field,
                                                        std::string value);
    static void remove();
};
}  // namespace Skybridge::API
#endif