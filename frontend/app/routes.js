import { index, layout, route } from "@react-router/dev/routes";

export default [
    layout("components/layout.jsx", [
        index("./pages/home.jsx"),
        route("about", "./pages/about.jsx"),
        route("support", "./pages/support.jsx"),

        route("account", "pages/account/layout.jsx", [
            route("dashboard", "./pages/account/main.jsx"),
            route("booking", "./pages/account/booking/layout.jsx", [
                index("./pages/account/booking/main.jsx"),
                route("details", "./pages/account/booking/details.jsx"),
            ]),
        ]),

        route("booking", "pages/booking/layout.jsx", [
            route("form", "pages/booking/form.jsx"),
            route("seatmap", "pages/booking/seatmap.jsx"),
            route("details", "pages/booking/flightdetails.jsx"),
            route("payment", "pages/booking/payment.jsx"),
            route("confirmation", "pages/booking/confirmation.jsx"),
        ]),

        route("search", "pages/search/layout.jsx", [index("pages/search/main.jsx")]),
    ]),
    route("/auth", "./pages/account/auth/layout.jsx", [
        route("login", "./pages/account/auth/login.jsx"),
        route("signup", "./pages/account/auth/signup.jsx"),
    ]),
];
