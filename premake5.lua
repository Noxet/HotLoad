workspace "HotLoad"
    architecture "x64"
    configurations {"Debug", "Release"}

    language "C++"

    outputdir = "%{cfg.buildcfg}-%{cfg.architecture}"

    cppdialect "C++17"

    -- multi-byte character set (not unicode) used to prevent error message like "cannot convert char* to LPCWSTR"
    characterset "MBCS"

    filter "configurations:Debug"
        -- Add preprocessor definition DEBUG to compiler
        defines {"DEBUG"}
        -- Enable debugging symbols
        symbols "On"
    
    filter "configurations:Release"
        defines {"RELEASE"}
        -- Turn on dat optimization
        optimize "On"

    filter "system:windows"
        defines
        {
            "HL_BUILD_DLL"
        }
    
-- HotLoad library with hot reload functions
project "HotLoad"
    location "HotLoad"
    kind "None"
    
    files
    {
        "%{prj.name}/src/**.h",
    }

    includedirs
    {
        "%{prj.name}/src"
    }

    filter "system:windows"
        defines
        {
            "HL_BUILD_DLL"
        }

-- HotLoadTest test library to be exported to DLL
project "HotLoadTest"
    location "HotLoadTest"
    kind "SharedLib"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp",
        "%{prj.name}/pub/**.h"
    }

    includedirs
    {
        "HotLoad/src"
    }

    filter "system:windows"
        defines
        {
           "TESTLIB_BUILD_DLL"
        }

    prebuildcommands
    {
        -- This is needed otherwise we can't copy the new DLL when the old one is still in use
        -- https://serverfault.com/a/503769
        --("{DELETE} ../bin/" ..outputdir .. "/Sandbox/%{prj.name}_old.dll"), --First remove any old file
        --("{MOVE} ../bin/" ..outputdir .. "/Sandbox/%{prj.name}.dll ../bin/" ..outputdir .. "/Sandbox/%{prj.name}_old.dll") -- Do the trick
    }

    -- Copy the .dll file to the Sandbox folder
    postbuildcommands
    {
        ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
    }

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    links
    {
        "HotLoadTest"
    }

    includedirs
    {
        "HotLoadTest",
        "HotLoad/src"
    }