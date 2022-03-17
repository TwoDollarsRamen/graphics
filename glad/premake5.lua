project "glad"
	kind "StaticLib"
	language "C"
	staticruntime "on"

	architecture "x64"

	targetdir "../bin"
	objdir "obj"

	includedirs {
		"include"
	}

	files {
		"include/glad/glad.h",
		"include/KHR/khrplatform.h",
		"src/glad.c"
	}

	filter "configurations:debug"
		runtime "debug"
		symbols "on"

	filter "configurations:release"
		runtime "release"
		optimize "on"
