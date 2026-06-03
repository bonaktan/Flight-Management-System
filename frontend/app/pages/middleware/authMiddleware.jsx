import { redirect } from "react-router";
import { AuthContext } from "./context";

export const authMiddleware = async ({ request, context }, next) => {
    console.log(request);
    console.log(context);
    context.set(AuthContext, { logged_in: true });
    throw redirect("/auth/login");
    // next();
};
