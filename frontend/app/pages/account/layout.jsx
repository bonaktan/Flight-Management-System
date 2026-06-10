import { Outlet } from "react-router";
import { authMiddleware } from "../../middleware/auth.middleware";
import axios from "axios";
import { useNavigate } from "react-router";
import { UserContext } from "../../middleware/context";
import { use, useState } from "react";
import { OverlayBase, OverlayModal } from "../../components/overlay";
import { InputField } from "../../components/input";
// const apiUrl = import.meta.env.VITE_BACKEND_URL;
export function meta() {
    return [{ title: "Account - SkyBridge Airlines" }];
}
export function HydrateFallback() {
    return (
        <div className="flex justify-center items-center">
            <p>Loading...</p>
        </div>
    );
}
export const middleware = [authMiddleware];

function EditModal({ parameter, children, className }) {
    return (
        <OverlayModal className="p-2 w-1/2">
            <div className="p-4">
                <p className="font-semibold text-xl">Edit {parameter}</p>
                <div className={className}>{children}</div>
            </div>
        </OverlayModal>
    );
}

function DetailEntry({ field, value, editable = false }) {
    const [showModal, setShowModal] = useState(false);
    const userContext = use(UserContext);
    return (
        <div className="flex">
            <div className="flex w-4/5">
                <div className="w-2/10 text-right">{field}</div>
                <div className="w-4">:</div>
                <div className="w-auto">{value}</div>
            </div>
            {editable && (
                <>
                    <div className="w-1/5">
                        <button onClick={() => setShowModal(true)}>
                            <span className="material-symbols-outlined">edit</span>
                        </button>
                    </div>

                    <OverlayBase open={showModal} setOpen={setShowModal}>
                        <EditModal parameter={field}>
                            {field == "Name" ? (
                                <div class="flex ">
                                    <InputField label="Edit Name" name="username" icon="person" />
                                </div>
                            ) : (
                                <></>
                            )}
                        </EditModal>
                    </OverlayBase>
                </>
            )}
        </div>
    );
}

function Dashboard({ account }) {
    const navigate = useNavigate();
    const userContext = use(UserContext);
    async function logout() {
        const user = await axios.post("/api/auth/logout");
        navigate("/auth/login");
    }
    return (
        <div className="flex flex-col gap-4">
            <div className="bg-blaze-deep flex py-4 px-8 items-center gap-2 rounded-sm">
                <div className="aspect-square h-28 bg-white rounded-full">
                    <svg viewBox="0 0 338 338" width="100%" height="100%" xmlns="http://www.w3.org/2000/svg">
                        <path fill="#DDD" d="m169,.5a169,169 0 1,0 2,0zm0,86a76,76 0 1 1-2,0zM57,287q27-35 67-35h92q40,0 67,35a164,164 0 0,1-226,0" />
                    </svg>
                </div>
                <div className="text-cloud-warm leading-tight">
                    <span className="text-3xl text-blaze-tint font-bold">{userContext.user.username}</span>
                    <br />
                    <p>Traveler</p>
                </div>
            </div>

            <div className="flex flex-col">
                <DetailEntry field="Name" value={account.account_name} />
                <DetailEntry field="User ID" value={account.userId} />
                <DetailEntry field="Email" value={account.email} />
            </div>
            <div className="flex justify-end gap-2">
                <button onClick={logout} className="bg-red-200 p-2 rounded-sm">
                    Log out
                </button>
            </div>
        </div>
    );
}

export async function clientLoader() {
    let ret = { apiReturn: null, apiError: null };
    try {
        ret.apiReturn = (await axios.get("/api/account/details")).data;
    } catch (e) {
        console.error("error in loader: ", e);
    }
    return ret;
}
export default function AccountLayout({ loaderData }) {
    return (
        <div className="flex">
            <div id="profile" className="w-1/3 p-2">
                {/* <div className="bg-blaze-tint flex p-2 items-center gap-2 rounded-sm">
                    <div className="aspect-square h-32 bg-white rounded-full" />
                        <p className="text-altitude-ink leading-tight">
                            <span className="text-3xl text-altitude-mid font-bold">Bonny</span>
                        <br/>Traveler</p>
                </div> */}
                <Dashboard account={loaderData.apiReturn} />
            </div>

            {/* <div className="bg-orange-300 px-16 py-8 flex gap-8 items-center ">
                <div className="aspect-square h-32 bg-white rounded-full" />
                <div className="flex flex-col gap-2">
                    <div className="text-3xl font-bold">Bonny</div>
                    <div>Traveller</div>
                </div>
            </div> */}
            {/* <div className="flex flex-row w-full gap-2 p-2">
                <NavButtons to="Dashboard" />
                <NavButtons to="Booking" />
            </div> */}
            <div className="bg-orange-100 m-2 p-4 w-2/3">
                <Outlet />
            </div>
        </div>
    );
}
