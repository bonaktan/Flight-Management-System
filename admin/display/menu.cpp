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
    Account& account = Account::getInstance();
    Airplane& airplane = Airplane::getInstance();
    Airport& airport = Airport::getInstance();
    Booking& booking = Booking::getInstance();
    Flight& flight = Flight::getInstance();
    Passenger& passenger = Passenger::getInstance();
    ftxui::ScreenInteractive screen = ftxui::ScreenInteractive::Fullscreen();

    const std::vector<std::string> nav_labels = {"Airports", "Airplanes",
                                                 "Flights", "Booking"};
    int selected = -1;
    int selected_tab = 4;  // 4 = placeholder (no selection)

    // Sample data per section — replace with your real data
    std::vector<std::vector<std::vector<std::string>>> section_data = {
        {{"IATA", "Name", "City"},
         {"MNL", "Ninoy Aquino Intl", "Manila"},
         {"CEB", "Mactan-Cebu Intl", "Cebu"}},
        {{"Tail", "Model", "Seats"},
         {"RP-C001", "A320", "180"},
         {"RP-C002", "B737", "160"}},
        {{"No.", "From", "To", "Dep"},
         {"SK101", "MNL", "CEB", "08:00"},
         {"SK202", "CEB", "MNL", "14:00"}},
        {{"ID", "Passenger", "Flight"},
         {"B001", "Juan Dela Cruz", "SK101"},
         {"B002", "Maria Santos", "SK202"}},
    };

    // Build content views (one per section + placeholder)
    std::vector<ftxui::Component> content_views;
    for (int i = 0; i < 4; ++i) {
        content_views.push_back(Display::TableInteractiveComponent(
            section_data[i], [&selected, &selected_tab] {
                selected = -1;
                selected_tab = 4;
            }));
    }
    content_views.push_back(ftxui::Renderer([] {  // placeholder at index 4
        return ftxui::text("Select a section from the sidebar.") | ftxui::dim |
               ftxui::center | ftxui::flex;
    }));

    ftxui::Component content_tab =
        ftxui::Container::Tab(content_views, &selected_tab);

    // Nav buttons — also update selected_tab
    std::vector<ftxui::Component> nav_buttons;
    for (int i = 0; i < 4; ++i) {
        int idx = i;
        nav_buttons.push_back(Button(
            nav_labels[i],
            [&selected, &selected_tab, idx] {
                selected = idx;
                selected_tab = idx;
            },
            ftxui::ButtonOption::Simple()));
    }

    ftxui::Component logout_btn = Button("Log Out", screen.ExitLoopClosure(),
                                         ftxui::ButtonOption::Simple());

    ftxui::Component container = ftxui::Container::Horizontal({
        ftxui::Container::Vertical({
            nav_buttons[0],
            nav_buttons[1],
            nav_buttons[2],
            nav_buttons[3],
            logout_btn,
        }),
        content_tab,
    });

    ftxui::Component renderer = Renderer(container, [&]() -> ftxui::Element {
        // Nav buttons
        ftxui::Elements nav_elems;
        for (int i = 0; i < 4; ++i) {
            ftxui::Element e =
                nav_buttons[i]->Render() | size(ftxui::WIDTH, ftxui::EQUAL, 20);
            if (i == selected) e = e | ftxui::inverted;
            nav_elems.push_back(e);
        }

        // Sidebar
        ftxui::Element sidebar =
            ftxui::vbox({
                vbox(nav_elems),
                ftxui::filler(),
                ftxui::vbox({ftxui::text("user") | ftxui::dim,
                             ftxui::text("Administrator") | ftxui::bold}),
                ftxui::separator(),
                logout_btn->Render() | size(ftxui::WIDTH, ftxui::EQUAL, 20),
            }) |
            ftxui::border | size(ftxui::WIDTH, ftxui::EQUAL, 22);

        // Title
        ftxui::Element title_bar =
            ftxui::text("SkyBridge Airlines - Admin TUI") | ftxui::bold |
            ftxui::hcenter | ftxui::bgcolor(ftxui::Color::GrayLight) |
            color(ftxui::Color::Black);

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