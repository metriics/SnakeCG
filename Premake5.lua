-- INFR 1350U Framework
-- Shawn Matthews (August 26, 2019)

-- This premake file sets up an easy to use framework for developing graphics applications, and allows easy addition
-- of other frameworks and libraries where required.
-- This build config is inspired by the Hazel Engine build system (https://github.com/TheCherno/Hazel), as well as
-- David Arpe's CMAKE system

-- Determine the root directory where we are calling premake from (this is our working directory)
local rootDir = path.getabsolute(_WORKING_DIR)

-- Log what the startup project will be
premake.info("Working DIR: " .. _WORKING_DIR)

premake.info("Search DIR: " .. rootDir .. "\\projects\\*")

-- Get all the directories in our projects directory
local projects = os.matchdirs(rootDir .. "\\projects\\*")

if (projects[#projects]) then
	-- Select the last item in the project directory to be our startup project 
	-- (this is easily changed in VS, this is just to be handy)
	startup = path.getbasename(projects[#projects])
else 
	startup = ""
end

-- Log what the startup project will be
premake.info("Startup project: " .. startup)

-- This is our solution name
workspace "INFR-1350U Framework"
	-- Processor architecture
	architecture "x64"

	-- Set which project to run on debug
	startproject(startup)

	-- Our configs are basically with and without compiler optimizations
	configurations {
		"Debug",
		"Release"
	}

-- The directory name for our output
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["glfw"]    = "external/glfw3/include"
IncludeDir["glad"]    = "external/glad/include" 
IncludeDir["ImGui"]   = "external/imgui"
IncludeDir["glm"]     = "external/GLM/include"
IncludeDir["stbs"]    = "external/stbs"
IncludeDir["fmod"]    = "external/fmod"
IncludeDir["spdlog"]  = "external/spdlog"
IncludeDir["toolkit"] = "external/toolkit"
IncludeDir["entt"]    = "external/entt"
IncludeDir["cereal"]    = "external/cereal"

-- These are other projects that we want to include in our solution (each needs their own premake)
include "external/glfw3"
include "external/glad"
include "external/imgui"
include "external/stbs"
include "external/Toolkit"

-- Iterate over all the projects (k is the index)
for k, proj in pairs(projects) do

	-- Extract the project name
	local name = path.getbasename(proj)
	-- Extract a relative path for the project
	local relpath = path.getrelative(rootDir, proj)

	-- Log the project in the console
	premake.info("Adding project: " .. name)
	premake.info("Source: " .. relpath)

	-- Add project to the solution
	project(name)
		-- Root folder of the project
		location(relpath)
		-- Output type of the project
		kind "ConsoleApp"
		-- Language (we are using MSVC)
		language "C++"
		-- C++ version (we are using the c++17 standard)
		cppdialect "C++17"
		-- Sets RuntimLibrary to MultiThreaded (non DLL version for static linking)
		staticruntime "on"

		-- This is where we will output our compiled program
		targetdir ("%{wks.location}\\bin\\" .. outputdir .. "\\%{prj.name}")
		-- This is where we will output our intermediate files
		objdir ("%{wks.location}\\obj\\" .. outputdir .. "\\%{prj.name}")

		-- Set the debug working directory to the output directory
		debugdir ("%{wks.location}bin\\%{outputdir}\\%{prj.name}")

		-- Gets the absolute directory of the current project
		absdir = "%{wks.location}bin\\%{outputdir}\\%{prj.name}"

		-- Gets our project's resource file location
		resdir = "%{prj.location}res"

		-- These are the commands that get executed after build, but before debugging
		postbuildcommands {
			-- This step copies over anything in the dll folder to the output directory
	  		"(xcopy /Q /E /Y /I /C \"%{wks.location}external\\dll\" \"%{absdir}\")",
	  		-- This step ensures that the project has a resource directory
	  		"(IF NOT EXIST \"%{resdir}\" mkdir \"%{resdir}\")",
	  		-- This step copies all the resources to the output directory
	  		"(xcopy /Q /E /Y /I /C \"%{resdir}\" \"%{absdir}\")"
		} 

		-- Our source files are everything in the src folder
		files {
			"%{prj.location}\\src\\**.h",
			"%{prj.location}\\src\\**.cpp"
		}

		-- Disable CRT secure warnings
		defines {
			"_CRT_SECURE_NO_WARNINGS"
		}

		-- Defines what directories we want to include
		includedirs {
			"%{prj.location}\\src",
			"%{IncludeDir.entt}",
			"%{IncludeDir.cereal}",
			"%{IncludeDir.fmod}",	
			"%{IncludeDir.spdlog}",		
			"%{IncludeDir.glfw}",
			"%{IncludeDir.glad}",
			"%{IncludeDir.ImGui}",
			"%{IncludeDir.glm}",
			"%{IncludeDir.stbs}",
			"%{IncludeDir.toolkit}"
		}

		-- These are what we are linking to (mostly other projects)
		links { 
			"GLFW",
			"Glad",
			"stbs",
			"ImGui",
			"Toolkit",
			"opengl32.lib",
			"imagehlp.lib",
			"external/fmod/fmod64.lib"
		}

		-- This filters for our windows builds
		filter "system:windows"
			systemversion "latest"

			-- Set some defines for the windows builds
			defines {
				"GLFW_INCLUDE_NONE", 
				"WINDOWS"
			}

		-- Filters for our debug configurations
		filter "configurations:Debug"
			runtime "Debug"
			symbols "on"

		-- Filters for release configuration
		filter "configurations:Release"
			runtime "Release"
			optimize "on"
end