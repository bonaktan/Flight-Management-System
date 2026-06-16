#include <algorithm>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/screen.hpp>
#include <iomanip>
#include <iostream>
#include <numeric>

#include "./display.h"

using namespace Skybridge;
// UTILITIES
void Display::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Display::pause() {
    std::cout << "\n  Press Enter to continue...";
    std::cin.get();
}

void Display::printHeader(const std::string& title) {
    std::cout << "\n";
    std::cout << "  *------------------------------------------*\n";
    std::cout << "  |  " << std::left << std::setw(42) << title << "|\n";
    std::cout << "  *------------------------------------------*\n\n";
}

void Display::printDivider() {
    std::cout << "  ------------------------------------------\n";
}

std::string Display::bookingStatusToStr(Structs::BookingStatus s) {
    switch (s) {
        case Structs::BookingStatus::PENDING:
            return "pending";
        case Structs::BookingStatus::CONFIRMED:
            return "confirmed";
        case Structs::BookingStatus::CANCELLED:
            return "cancelled";
    }
    return "pending";
}

Structs::BookingStatus Display::strToBookingStatus(const std::string& s) {
    if (s == "confirmed") return Structs::BookingStatus::CONFIRMED;
    if (s == "cancelled") return Structs::BookingStatus::CANCELLED;
    return Structs::BookingStatus::PENDING;
}

void Display::Table(std::vector<std::vector<std::string>>& data) {
    int termWidth = ftxui::Terminal::Size().dimx;
    int cols = data.empty() ? 0 : data[0].size();

    // Find natural max width per column
    std::vector<int> colWidths(cols, 0);
    for (auto& row : data)
        for (int c = 0; c < cols; c++)
            colWidths[c] = std::max(colWidths[c], (int)row[c].size());

    int totalWidth = std::accumulate(colWidths.begin(), colWidths.end(), 0) +
                     cols + 1;  // +borders

    if (totalWidth > termWidth) {
        int overflow = totalWidth - termWidth;

        std::vector<int> order(cols);
        std::iota(order.begin(), order.end(), 0);
        std::sort(order.begin(), order.end(),
                  [&](int a, int b) { return colWidths[a] > colWidths[b]; });

        // First pass: truncate data cells only
        for (int c : order) {
            if (overflow <= 0) break;
            int headerLen = (int)data[0][c].size();
            int minWidth = headerLen;  // preserve header
            int cut = std::min(overflow, colWidths[c] - minWidth);
            if (cut <= 0) continue;
            colWidths[c] -= cut;
            overflow -= cut;
            int cellMax = colWidths[c];
            for (int r = 1; r < (int)data.size(); r++)  // skip header
                if ((int)data[r][c].size() > cellMax)
                    data[r][c] =
                        data[r][c].substr(0, std::max(3, cellMax - 3)) + "...";
        }

        // Second pass: truncate headers only if still overflowing
        for (int c : order) {
            if (overflow <= 0) break;
            int cut = std::min(overflow, colWidths[c] - 3);
            if (cut <= 0) continue;
            colWidths[c] -= cut;
            overflow -= cut;
            int cellMax = colWidths[c];
            if ((int)data[0][c].size() > cellMax)
                data[0][c] =
                    data[0][c].substr(0, std::max(3, cellMax - 3)) + "...";
        }
    }

    ftxui::Table table = ftxui::Table(data);
    table.SelectAll().Border(ftxui::LIGHT);
    table.SelectAll().Separator(ftxui::LIGHT);
    table.SelectRow(0).Decorate(ftxui::bold);
    table.SelectRow(0).SeparatorVertical(ftxui::LIGHT);
    table.SelectRow(0).Border(ftxui::LIGHT);
    auto document = table.Render();
    auto screen = ftxui::Screen::Create(ftxui::Dimension::Full(),
                                        ftxui::Dimension::Fit(document));
    Render(screen, document);
    std::cout << screen.ToString() << std::endl;
}

ftxui::Component Display::TableInteractiveComponent(
    std::vector<std::vector<std::string>>& data,
    std::function<void()> on_quit) {
    auto scroll_x = std::make_shared<int>(0);
    auto scroll_y = std::make_shared<int>(0);
    int max_y = static_cast<int>(data.size()) - 1;
    int max_x = static_cast<int>(data[0].size()) - 1;

    return CatchEvent(
        ftxui::Renderer([&data, scroll_x, scroll_y] {
            ftxui::Table table(data);
            table.SelectAll().Border(ftxui::LIGHT);
            table.SelectAll().Separator(ftxui::LIGHT);
            table.SelectRow(0).Decorate(ftxui::bold);
            table.SelectRow(0).SeparatorVertical(ftxui::LIGHT);
            table.SelectRow(0).Border(ftxui::LIGHT);
            return ftxui::vbox({
                table.Render() | ftxui::focusPosition(*scroll_x, *scroll_y) |
                    ftxui::frame | ftxui::flex,
                ftxui::separator(),
                ftxui::text("Arrow keys: scroll | q: back") | ftxui::dim,
            });
        }),
        [scroll_x, scroll_y, max_x, max_y, on_quit](ftxui::Event event) {
            if (event == ftxui::Event::ArrowUp) {
                *scroll_y = std::max(0, *scroll_y - 1);
                return true;
            }
            if (event == ftxui::Event::ArrowDown) {
                *scroll_y = std::min(max_y, *scroll_y + 1);
                return true;
            }
            if (event == ftxui::Event::ArrowLeft) {
                *scroll_x = std::max(0, *scroll_x - 1);
                return true;
            }
            if (event == ftxui::Event::ArrowRight) {
                *scroll_x = std::min(max_x, *scroll_x + 1);
                return true;
            }
            if (event == ftxui::Event::Character('q')) {
                on_quit();
                return true;
            }
            return false;
        });
}