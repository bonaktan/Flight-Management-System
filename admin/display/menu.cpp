#include <algorithm>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/screen.hpp>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#include "../api/api.h"
#include "../controls/controls.h"
#include "./display.h"
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"

using namespace Skybridge;
using namespace API;

bool Menu::contains(std::vector<std::string>& vector, std::string value) {
    auto it = std::find(vector.begin(), vector.end(), value);
    if (it != vector.end()) {
        return true;
    } else {
        return false;
    }
}

void Menu::mainMenu() {
    Account& _accountInstance = Account::getInstance();
    APIEntity Account = {
        _accountInstance.name,   _accountInstance.UNSUPPORTED_OPS,
        _accountInstance.view,   _accountInstance.add,
        _accountInstance.modify, _accountInstance.remove,
    };
    Airplane& _airplaneInstance = Airplane::getInstance();
    APIEntity Airplane = {
        _airplaneInstance.name,   _airplaneInstance.UNSUPPORTED_OPS,
        _airplaneInstance.view,   _airplaneInstance.add,
        _airplaneInstance.modify, _airplaneInstance.remove,
    };

    Airport& _airportInstance = Airport::getInstance();
    APIEntity Airport = {
        _airportInstance.name,   _airportInstance.UNSUPPORTED_OPS,
        _airportInstance.view,   _airportInstance.add,
        _airportInstance.modify, _airportInstance.remove,
    };

    Booking& _bookingInstance = Booking::getInstance();
    APIEntity Booking = {
        _bookingInstance.name,   _bookingInstance.UNSUPPORTED_OPS,
        _bookingInstance.view,   _bookingInstance.add,
        _bookingInstance.modify, _bookingInstance.remove,
    };

    Flight& _flightInstance = Flight::getInstance();
    APIEntity Flight = {
        _flightInstance.name,   _flightInstance.UNSUPPORTED_OPS,
        _flightInstance.view,   _flightInstance.add,
        _flightInstance.modify, _flightInstance.remove,
    };

    Passenger& _passengerInstance = Passenger::getInstance();
    APIEntity Passenger = {
        _passengerInstance.name,   _passengerInstance.UNSUPPORTED_OPS,
        _passengerInstance.view,   _passengerInstance.add,
        _passengerInstance.modify, _passengerInstance.remove,
    };
    Auth& authInstance = Auth::getInstance();
    std::vector<APIEntity> entities = {Account, Airplane, Airport,
                                       Booking, Flight,   Passenger};

    const std::vector<std::string> nav_labels = {"Accounts", "Airplanes",
                                                 "Airports", "Booking",
                                                 "Flights",  "Passengers"};
    const int N = (int)entities.size();

    auto screen = ftxui::ScreenInteractive::Fullscreen();
    int selected = -1;
    int selected_tab = N;  // N = placeholder index

    std::vector<ftxui::Component> content_views;
    for (int i = 0; i < N; ++i) {
        content_views.push_back(Display::TableInteractiveComponent(
            entities[i], [&selected, &selected_tab, N] {
                selected = -1;
                selected_tab = N;
            }));
    }
    content_views.push_back(ftxui::Renderer([] {  // placeholder
        return ftxui::text("Select a section from the sidebar.") | ftxui::dim |
               ftxui::center | ftxui::flex;
    }));

    auto content_tab = ftxui::Container::Tab(content_views, &selected_tab);

    std::vector<ftxui::Component> nav_buttons;
    for (int i = 0; i < N; ++i) {
        int idx = i;
        nav_buttons.push_back(Button(
            nav_labels[i],
            [&selected, &selected_tab, &content_tab, idx] {
                selected = idx;
                selected_tab = idx;
                content_tab->TakeFocus();  // <-- add this
            },
            ftxui::ButtonOption::Simple()));
    }

    auto logout_btn = Button("Log Out", screen.ExitLoopClosure(),
                             ftxui::ButtonOption::Simple());

    auto nav_container = ftxui::Container::Vertical({
        nav_buttons[0],
        nav_buttons[1],
        nav_buttons[2],
        nav_buttons[3],
        nav_buttons[4],
        nav_buttons[5],
        logout_btn,
    });

    auto container = ftxui::Container::Horizontal({nav_container, content_tab});

    auto renderer = Renderer(container, [&]() -> ftxui::Element {
        ftxui::Elements nav_elems;
        for (int i = 0; i < N; ++i) {
            ftxui::Element e =
                nav_buttons[i]->Render() | size(ftxui::WIDTH, ftxui::EQUAL, 20);
            if (i == selected) e = e | ftxui::inverted;
            nav_elems.push_back(e);
        }

        ftxui::Element sidebar =
            ftxui::vbox({
                ftxui::vbox(nav_elems),
                ftxui::filler(),
                ftxui::vbox({ftxui::text(authInstance.user) | ftxui::bold,
                             ftxui::text("Administrator") | ftxui::dim}),
                ftxui::separator(),
                logout_btn->Render() | size(ftxui::WIDTH, ftxui::EQUAL, 20),
            }) |
            ftxui::border | size(ftxui::WIDTH, ftxui::EQUAL, 22);

        ftxui::Element title_bar =
            ftxui::text("SkyBridge Airlines - Admin TUI") | ftxui::bold |
            ftxui::hcenter | ftxui::bgcolor(ftxui::Color::GrayLight) |
            ftxui::color(ftxui::Color::Black);

        return ftxui::vbox({
            title_bar,
            ftxui::hbox({sidebar,
                         content_tab->Render() | ftxui::flex | ftxui::border}) |
                ftxui::flex,
        });
    });

    screen.Loop(renderer);
}

bool Menu::authenticate() {
    auto& auth = API::Auth::getInstance();
    Display::clearScreen();
    std::cout << "\n";
    std::cout << "  *------------------------------------------*\n";
    std::cout << "  |      AIRLINE MANAGEMENT SYSTEM           |\n";
    std::cout << "  |           Access Required                |\n";
    std::cout << "  *------------------------------------------*\n\n";
    int attempts = 3;
    while (attempts-- > 0) {
        std::string email = Input::getInput("Email: ");
        std::string pass = Input::getInput("Password: ");
        try {
            bool response = auth.login(email, pass);
            if (response) {
                std::cout << "\n  [OK] Access granted.\n";
                return true;
            }
            std::cout << "  [!!] Incorrect. " << attempts
                      << " attempt(s) remaining.\n\n";
        } catch (const std::runtime_error& e) {
            std::cout << "\n  [!!] Server error: " << e.what() << "\n";
            std::cout << "  Unable to reach server. Exiting.\n\n";
            return false;
        }
    }
    std::cout << "\n  Access denied. Exiting.\n\n";
    return false;
}