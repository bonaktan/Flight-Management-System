import { Outlet } from "react-router";
export default function Layout() {
    return (
        <div className="relative min-h-dvh h-fit w-dvw bg-blaze-deep">
            <div className="absolute top-0 left-0"></div>
            <div className="absolute flex items-center justify-center lg:h-dvh h-fit lg:w-1/3 w-full top-1/2 lg:left-auto lg:right-0 lg:-translate-x-1/16 left-1/2 transform -translate-x-1/2 -translate-y-1/2">
                <div className="bg-blaze-tint h-9/12 p-4 flex flex-col lg:justify-center justify-evenly lg:py-20 rounded-md">
                    <div>
                        <p className="text-3xl text-center tracking-wider">SkyBridge Airlines</p>
                    </div>
                    <div className="px-8">
                        <Outlet />
                    </div>
                </div>
            </div>
        </div>
    );
}
