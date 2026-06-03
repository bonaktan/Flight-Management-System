import { redirect } from "react-router";
import { AuthContext } from "./context";
import fetchUser from "./auth.server";

export const authMiddleware = async ({ request, context }, next) => {
    const userRet = await fetchUser(request);
    console.log(userRet);
    context.set(AuthContext, userRet.user);
    if (!userRet.success) throw redirect("/auth/login");
    next();
};
