#include <algorithm>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/screen.hpp>
#include <iomanip>
#include <iostream>
#include <numeric>

#include "../api/api.h"
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

// hey, if you're reaading this, just know that only god knows how this thing
// works
ftxui::Component Display::TableInteractiveComponent(
    Skybridge::API::APIEntity entity, std::function<void()> on_quit) {
    // Shared state — heap allocated so lambdas stay valid
    auto data = std::make_shared<std::vector<std::vector<std::string>>>();
    auto loaded = std::make_shared<bool>(false);
    auto scroll_x = std::make_shared<int>(0);
    auto scroll_y = std::make_shared<int>(1);
    auto selected_row = std::make_shared<int>(1);
    auto edit_mode = std::make_shared<bool>(false);
    auto form_mode = std::make_shared<int>(0);  // 0=none 1=add 2=edit
    auto form_fields = std::make_shared<std::vector<std::string>>();
    auto inputs = std::make_shared<std::vector<ftxui::Component>>();

    // Rebuilt when data is loaded
    auto form_container = std::make_shared<ftxui::Component>(
        ftxui::Renderer([] { return ftxui::text(""); }));
    auto sidebar_container = std::make_shared<ftxui::Component>(
        ftxui::Renderer([] { return ftxui::text(""); }));
    auto full_container = std::make_shared<ftxui::Component>(
        ftxui::Renderer([] { return ftxui::text(""); }));

    auto toggle_edit_btn = std::make_shared<ftxui::Component>();
    auto add_btn = std::make_shared<ftxui::Component>();
    auto edit_btn = std::make_shared<ftxui::Component>();
    auto delete_btn = std::make_shared<ftxui::Component>();
    auto confirm_btn = std::make_shared<ftxui::Component>();
    auto cancel_btn = std::make_shared<ftxui::Component>();

    auto build_components = [=, unsupported = entity.unsupported_ops,
                             modify_fn = entity.modify]() {
        int col_count = (int)(*data)[0].size();
        form_fields->assign(col_count, "");
        inputs->clear();

        for (int i = 0; i < col_count; ++i) {
            ftxui::InputOption opt;
            opt.multiline = false;
            inputs->push_back(
                ftxui::Input(&(*form_fields)[i], (*data)[0][i], opt));
        }

        *form_container = ftxui::Container::Vertical(*inputs);

        bool add_disabled =
            Menu::contains(const_cast<std::vector<std::string>&>(unsupported),
                           std::string("add"));

        *add_btn = Button(
            "Add",
            [=] {
                if (add_disabled) return;
                *form_mode = 1;
                std::fill(form_fields->begin(), form_fields->end(), "");
            },
            ftxui::ButtonOption::Simple());

        *edit_btn = Button(
            "Edit",
            [=] {
                if (*selected_row >= 1 && *selected_row < (int)data->size()) {
                    *form_mode = 2;
                    for (int i = 0; i < (int)(*data)[*selected_row].size(); ++i)
                        (*form_fields)[i] = (*data)[*selected_row][i];
                }
            },
            ftxui::ButtonOption::Simple());

        *delete_btn = Button(
            "Delete",
            [=] {
                if (*selected_row >= 1 && *selected_row < (int)data->size()) {
                    // first col is assumed to be ID
                    std::string id = (*data)[*selected_row][0];
                    data->erase(data->begin() + *selected_row);
                    if (*selected_row >= (int)data->size())
                        *selected_row = (int)data->size() - 1;
                    *scroll_y = *selected_row;
                }
                *form_mode = 0;
            },
            ftxui::ButtonOption::Simple());

        *confirm_btn = Button(
            "Confirm",
            [=] {
                if (*form_mode == 1) {
                    // add: call entity.add() — wire your own add() args here
                    data->push_back(*form_fields);
                    *selected_row = (int)data->size() - 1;
                    *scroll_y = *selected_row;
                } else if (*form_mode == 2) {
                    if (*selected_row >= 1 &&
                        *selected_row < (int)data->size()) {
                        std::string id = (*data)[*selected_row][0];
                        // modify each changed field
                        for (int i = 1; i < (int)form_fields->size(); ++i) {
                            modify_fn(id, (*data)[0][i], (*form_fields)[i]);
                        }
                        (*data)[*selected_row] = *form_fields;
                    }
                }
                *form_mode = 0;
            },
            ftxui::ButtonOption::Simple());

        *cancel_btn = Button(
            "Cancel", [=] { *form_mode = 0; }, ftxui::ButtonOption::Simple());

        *toggle_edit_btn = Button(
            "[ Edit Mode ]",
            [=] {
                *edit_mode = !*edit_mode;
                if (!*edit_mode) *form_mode = 0;
            },
            ftxui::ButtonOption::Simple());

        *sidebar_container =
            ftxui::Container::Vertical({*add_btn, *edit_btn, *delete_btn});

        *full_container = ftxui::Container::Vertical({
            *toggle_edit_btn,
            ftxui::Container::Horizontal({
                *sidebar_container,
                ftxui::Container::Vertical(
                    {*form_container, *confirm_btn, *cancel_btn}),
            }),
        });
    };

    // Initial placeholder container (replaced after load)
    auto placeholder = ftxui::Renderer([] {
        return ftxui::text("Loading...") | ftxui::dim | ftxui::center |
               ftxui::flex;
    });

    auto outer = std::make_shared<ftxui::Component>(placeholder);

    // The real renderer — only used after data is loaded
    auto make_renderer = [=]() -> ftxui::Component {
        return CatchEvent(
            Renderer(
                *full_container,
                [=]() -> ftxui::Element {
                    auto display_data = *data;
                    if (*selected_row >= 1 &&
                        *selected_row < (int)display_data.size()) {
                        for (auto& cell : display_data[*selected_row])
                            cell = "> " + cell;
                    }

                    ftxui::Table table(display_data);
                    table.SelectAll().Border(ftxui::LIGHT);
                    table.SelectAll().Separator(ftxui::LIGHT);
                    table.SelectRow(0).Decorate(ftxui::bold);
                    table.SelectRow(0).Border(ftxui::LIGHT);

                    ftxui::Element table_elem =
                        table.Render() |
                        ftxui::focusPosition(*scroll_x, *scroll_y) |
                        ftxui::frame | ftxui::flex;

                    ftxui::Element edit_panel = ftxui::vbox({});
                    if (*edit_mode) {
                        ftxui::Element panel =
                            ftxui::vbox({
                                ftxui::text("Actions") | ftxui::bold |
                                    ftxui::hcenter,
                                ftxui::separator(),
                                (*sidebar_container)->Render(),
                            }) |
                            ftxui::border |
                            size(ftxui::WIDTH, ftxui::EQUAL, 14);

                        if (*form_mode != 0) {
                            std::string form_title =
                                (*form_mode == 1) ? "Add Row" : "Edit Row";
                            ftxui::Elements field_elems;
                            for (int i = 0; i < (int)(*data)[0].size(); ++i) {
                                field_elems.push_back(ftxui::hbox({
                                    ftxui::text((*data)[0][i] + ": ") |
                                        size(ftxui::WIDTH, ftxui::EQUAL, 12),
                                    (*inputs)[i]->Render() | ftxui::flex,
                                }));
                            }
                            field_elems.push_back(ftxui::separator());
                            field_elems.push_back(ftxui::hbox(
                                {(*confirm_btn)->Render(), ftxui::text(" "),
                                 (*cancel_btn)->Render()}));
                            ftxui::Element form =
                                ftxui::vbox({
                                    ftxui::text(form_title) | ftxui::bold |
                                        ftxui::hcenter,
                                    ftxui::separator(),
                                    ftxui::vbox(field_elems),
                                }) |
                                ftxui::border |
                                size(ftxui::WIDTH, ftxui::EQUAL, 36);

                            edit_panel = ftxui::hbox({panel, form});
                        } else {
                            edit_panel = panel;
                        }
                    }

                    return ftxui::vbox({
                        ftxui::hbox(
                            {ftxui::filler(), (*toggle_edit_btn)->Render()}),
                        ftxui::hbox(
                            {table_elem,
                             (*edit_mode ? edit_panel : ftxui::vbox({}))}),
                        ftxui::separator(),
                        ftxui::text("↑↓: select row | ←→: scroll | Tab: switch "
                                    "focus | q: back") |
                            ftxui::dim,
                    });
                }),
            [=](ftxui::Event event) -> bool {
                if (*form_mode != 0) return false;
                int max_y = (int)data->size() - 1;
                int max_x = (int)(*data)[0].size() - 1;
                if (event == ftxui::Event::ArrowUp) {
                    *selected_row = std::max(1, *selected_row - 1);
                    *scroll_y = *selected_row;
                    return true;
                }
                if (event == ftxui::Event::ArrowDown) {
                    *selected_row = std::min(max_y, *selected_row + 1);
                    *scroll_y = *selected_row;
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
    };

    // Wrapper that lazy-loads on first render

    auto view_fn = entity.view;
    auto wrapper = ftxui::Container::Vertical({placeholder});

    return ftxui::Renderer(wrapper, [=]() mutable -> ftxui::Element {
        if (!*loaded) {
            *data = view_fn();
            *loaded = true;
            if (data->empty())
                return ftxui::text("No data returned from API.") | ftxui::dim |
                       ftxui::center | ftxui::flex;
            build_components();
            *outer = make_renderer();
            wrapper->DetachAllChildren();
            wrapper->Add(*outer);
        }
        return (*outer)->Render();
    });
}