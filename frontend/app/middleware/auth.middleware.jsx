import { UserContext } from "./context";
import fetchUser from "./auth.server";
import { redirect } from "react-router";

export const userMiddleware = async ({ request, context }, next) => {
    const userRet = await fetchUser(request);
    context.set(UserContext, userRet);
    next();
};

export const authMiddleware = async ({ context }, next) => {
    const user = context.get(UserContext);
    if (!user.logged_in) {
        return redirect("/auth/login");
    }
    next();
};
