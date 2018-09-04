Is64bit  = true
UseClang = true

workspace "Labyrinth"
    language "C++"
    cppdialect "C++17"

    location "build"
    if Is64bit then
        location "build64"
    else 
        location "build32"
    end

    if Is64bit  then
        architecture "x86_64"
    else
        architecture "x86"
    end

    configurations { "Debug", "Release" }

    filter { "configurations:Debug" }
        symbols "On"
        optimize "Off"

    filter { "configurations:Release" }
        optimize "On"
        symbols "Off"
    filter { }

    if Is64bit then
        targetdir ("build64/bin/%{prj.name}/%{cfg.longname}")

        objdir ("build64/obj/%{prj.name}/%{cfg.longname}")
    else 
        targetdir ("build32/bin/%{prj.name}/%{cfg.longname}")

        objdir ("build32/obj/%{prj.name}/%{cfg.longname}")
    end

    if UseClang then
        toolset "clang"
    end

    libdirs { "lib/glfw3/" }

project "Framework"

    kind "ConsoleApp"

    files { "src/**.hpp", "src/**.cpp", "src/**.inl", "src/**.h", "src/**.c"  }

    includedirs { "lib/rang/include" }

    links { "glfw3", "GL", "pthread", "X11", "vulkan", "Xrandr", "Xcursor", "Xinerama", "Xxf86vm", "dl", "Xi" } --OOF, so many packages


    filter "configurations:Debug"
            defines { "DEBUG" }

    filter "configurations:Release"
            defines { "NDEBUG" }