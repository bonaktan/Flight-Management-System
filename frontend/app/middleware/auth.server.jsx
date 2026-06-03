import axios from "axios";

const apiUrl = import.meta.env.VITE_BACKEND_URL;
export default async function fetchUser(request) {
    const cookie = request.headers.get("Cookie") ?? "";
    const url = new URL(request.url);
    console.log(cookie);
    console.log(url);
    // BUG: in server, this endpoint is set to post for some reason, not get
    let status;
    let user;
    try {
        const apiReturn = await axios.post(`${apiUrl}/api/auth/authenticate`, null, {
            headers: {
                Cookie: cookie,
            },
        });
        status = apiReturn.status;
        user = apiReturn.data;
    } catch (e) {
        status = e.status;
    }
    return { success: status == 200 ? true : false, status: status, user: user };
}
