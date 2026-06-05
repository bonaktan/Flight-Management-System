#pragma once

#ifndef SKYBRIDGE_API
#define SKYBRIDGE_API

#include "../main.h"

namespace Skybridge::API {

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

}  // namespace API
#endif