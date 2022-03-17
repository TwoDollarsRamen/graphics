project "graphics"
	kind "ConsoleApp"
	language "C"
	staticruntime "on"

	architecture "x64"

	targetdir "../bin"
	objdir "obj"

	defines {
		"GLFW_INCLUDE_NONE"
	}

	includedirs {
		"src",
		"../glfw/include",
		"../glad/include"
	}

	files {
		"src/**.c",
		"src/**.c"
	}

	links {
		"glfw",
		"glad"
	}

	filter "system:linux"
		links {
			"X11",
			"dl",
			"m",
			"GL",
			"pthread"
		}
	
	filter "system:windows"
		links {
			"opengl32",
			"user32",
			"gdi32",
			"kernel32"
		}

		defines {
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:debug"
		runtime "debug"
		symbols "on"

	filter "configurations:release"
		runtime "release"
		optimize "on"
