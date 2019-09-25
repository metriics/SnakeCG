project "Toolkit"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    -- Sets RuntimLibrary to MultiThreaded (non DLL version for static linking)
    staticruntime "on"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("obj/" .. outputdir .. "/%{prj.name}")

    files
    {
        "Logging.h",
        "Logging.cpp",
        "CerealGLM.h",
        "EnumToString.h",
        "Sys.h",
        "Sys.cpp",
        "TTK\\**.cpp",
        "TTK\\**.h"
    }

    links {
        "Glad",
        "GLFW",
        "stbs",
        "opengl32.lib"
    }

    includedirs {
        "%{wks.location}\\external\\spdlog",
        "%{wks.location}\\external\\GLM\\include",
        "%{wks.location}\\external\\glad\\include",
        "%{wks.location}\\external\\glfw3\\include",
        "%{wks.location}\\external\\imgui",
        "%{wks.location}\\external\\stbs"
    }

    filter "system:windows"
        systemversion "latest"

        defines {
            "WINDOWS",
            "TTK_GLFW"
        }

        
    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
        
