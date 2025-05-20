
require "vendor/export-compile-commands"

-- Define the workspace

workspace "FluidSimulation"
    architecture "x64" -- x86_64 architecture
    configurations { "Debug", "Release" }
    startproject "FluidSim"

    -- Output directory format
    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    -- Define the main project
    project "FluidSim"
        kind "ConsoleApp" -- Console application
        language "C++"
        cppdialect "C++17" -- C++17 standard

        -- Output directories
        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

        -- Source files
        files {
            "src/**.cpp", -- All .cpp files in src/
            "include/**.hpp", -- All .h files in include/ for IDE visibility
			"vendor/**.h"
        }

        -- Include directories
        includedirs {
            "include", -- Project headers (core/, renderer/)
			"vendor"
        }

        -- Link system-installed libraries (e.g., GLFW, OpenGL, GLEW)
        filter "system:linux"
            links {
				"vulkan",
                "glfw", -- GLFW
            }
            defines { "PLATFORM_LINUX" }

        -- Configuration-specific settings
        filter "configurations:Debug"
            defines { "DEBUG" }
            symbols "On" -- Debugging symbols
            optimize "Off"

        filter "configurations:Release"
            defines { "NDEBUG" }
            optimize "On"

		
