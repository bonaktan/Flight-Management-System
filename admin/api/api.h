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
    const std::string base_url = "https://skybridge.bonnybonnybonaktan.xyz/api";

    ApiClient() = default;
    void checkSession(const cpr::Cookies& updated_cookies) {
        if (!updated_cookies.empty()) cookies = updated_cookies;
    }

   public:
    ApiClient(const ApiClient&) = delete;
    ApiClient& operator=(const ApiClient&) = delete;

    static ApiClient& getInstance() {
        if (!instance) instance = new ApiClient();
        return *instance;
    }

    cpr::Response get(const std::string& endpoint) {
        auto response = cpr::Get(cpr::Url{base_url + endpoint}, cookies);
        checkSession(response.cookies);
        return response;
    }

    cpr::Response post(const std::string& endpoint, nlohmann::json payload) {
        auto response =
            cpr::Post(cpr::Url{base_url + endpoint},
                      cpr::Header{{"Content-Type", "application/json"}},
                      cpr::Body{payload.dump()}, cookies);
        checkSession(response.cookies);
        return response;
    }
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
   public:
    static void save();
    static void load();
    static long long nextId();
    static void view();
    static void add();
    static void modify();
    static void remove();
};

class Airplane {
   public:
    static void save();
    static void load();
    static void view();
    static void add();
    static void modify();
    static void remove();
};

class Airport {
   public:
    static void save();
    static void load();
    static void view();
    static void add();
    static void modify();
    static void remove();
};

class Booking {
   public:
    static void save();
    static void load();
    static long long nextId();
    static void view();
    static void add();
    static void modify();
    static void remove();
};

class Flight {
   public:
    static void save();
    static void load();
    static void view();
    static void add();
    static void modify();
    static void remove();
};

class Passenger {
   public:
    static void save();
    static void load();
    static long long nextId();
    static void view();
    static void add();
    static void modify();
    static void remove();
};

class SeatClass {
   public:
    static void save();
    static void load();
    static long long nextId();
    static void view();
    static void add();
    static void modify();
    static void remove();
};

class Staff {
   public:
    static void save();
    static void load();
    static long long nextId();
    static void view();
    static void add();
    static void modify();
    static void remove();
};


}  // namespace Skybridge::API
#endif