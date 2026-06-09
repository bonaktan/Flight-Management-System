import axios from "axios";

const apiUrl = import.meta.env.VITE_BACKEND_URL;
export default async function fetchUser(request) {
    const cookie = request.headers.get("Cookie") ?? "";
    // BUG: in server, this endpoint is set to post for some reason, not get
    let status;
    let user;
    try {
        const apiReturn = await axios.get(`${apiUrl}/api/auth/authenticate`, {
            headers: {
                Cookie: cookie,
            },
        });
        status = apiReturn.status;
        user = apiReturn.data;
    } catch (e) {
        console.log(e);
        status = e.response?.status;
    }
    return { logged_in: status == 200 ? true : false, status: status, user: user };
}
