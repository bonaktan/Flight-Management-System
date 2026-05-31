import { Outlet } from "react-router";
export default function Login() {
    return (
        <div className="relative min-h-dvh h-fit w-dvw bg-red-200">
            <div className="absolute top-0 left-0">
                <p className="text-center">insert slowly sliding images here</p>
            </div>
            <div className="absolute lg:h-dvh h-fit lg:w-1/3 w-full top-1/2 lg:left-auto lg:right-0 lg:-translate-x-1/16 left-1/2 transform -translate-x-1/2 -translate-y-1/2">
                <div className="bg-gray-500 h-full p-4 flex flex-col lg:justify-start justify-evenly lg:py-20">
                    <div>
                        <p className="text-3xl text-center m-10">SkyBridge Airlines</p>
                    </div>
                    <div className="bg-red-200 p-8 mb-4 border">
                        <Outlet />
                    </div>
                </div>
            </div>
        </div>
    );
}
