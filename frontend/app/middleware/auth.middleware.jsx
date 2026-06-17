import { UserContext } from "./context";
import { redirect } from "react-router";

export const userMiddleware = async ({ request, context }, next) => {
    const userRet = await fetchUser();
    context.set(UserContext, userRet);
    next();
};

export const authMiddleware = async ({ context, request }, next) => {
    const user = context.get(UserContext);
    if (!user.logged_in) {
        const returnTo = encodeURIComponent(new URL(request.url).pathname);
        return redirect(`/auth/login?returnTo=${returnTo}`);
    }
    next();
};

import axios from "axios";

async function fetchUser() {
    let status;
    let user;
    try {
        const apiReturn = await axios.get("/api/auth/authenticate");
        status = apiReturn.status;
        user = apiReturn.data;
    } catch (e) {
        status = e.response?.status;
    }
    return { logged_in: status == 200 ? true : false, status: status, user: user };
}
