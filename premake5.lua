-- A solution contains projects, and defines the available configurations
solution "ArkEngine"
	configurations { "Debug", "Release"}
	flags { "Unicode" , "NoPCH"}
	projectName = "ArkEngine"
	
	-- A project defines one build target
	project (projectName)
		kind "ConsoleApp"
		location (projectName)
		language "C++"
		targetdir ( projectName )
	  
	configuration { "windows" }
		buildoptions ""
		linkoptions { "/NODEFAULTLIB:msvcrt" }
	configuration
	{
	}

	files -- build all .h and .cpp files recursively
	{ 	
		path.join("**.h"),
		path.join("**.cpp"),
		path.join("**.hpp"),
		path.join("**.vert"),
		path.join("**.frag"),
		path.join("**.bmp"),
		path.join("**.png"),
		path.join("./Content/Models/**.obj"),
		path.join("./Content/**.lua"),
	}
	excludes  -- don't build files in graphics_dependencies/
	{
		"./EngineLibraries/**"
	}
	
	-- where are header files?
	configuration "windows"
	includedirs
	{
		"./Source",
		"./EngineLibraries/SDL2/include",
		"./EngineLibraries/SDL2/include/SDL2",
		"./EngineLibraries/glew/include",
		"./EngineLibraries/glm/glm",
		"./EngineLibraries/assimp/include",
		"./EngineLibraries/SDL2_image/include",
		"./EngineLibraries/SDL2_ttf/include",
		"./EngineLibraries/lua/src",
	}


	  -- what libraries need linking to
	configuration "windows"
		links 
		{ 
			"SDL2", 
			"SDL2main", 
			"opengl32", 
			"glew32", 
			"assimp-vc130-mtd",
			"SDL2_image",
			"SDL2_ttf",
			"lua",
		}
	configuration
	{
	}
	
	-- where are libraries?
	configuration "windows"
	libdirs
	{
		"./EngineLibraries/glew/lib/Release/Win32",
		"./EngineLibraries/SDL2/lib/win32",
		"./EngineLibraries/assimp/lib",
		"./EngineLibraries/SDL2_image/lib/x86",
		"./EngineLibraries/SDL2_ttf/lib/x86",
		"./EngineLibraries/lua/lib",
	}
	
	configuration "*Debug"
		defines { "DEBUG" }
		flags { "Symbols" }
		optimize "Off"
		targetsuffix "-debug"

	configuration "*Release"
		defines { "NDEBUG" }
		optimize "On"
		targetsuffix "-release"

	-- copy dlls on windows 
	configuration "windows"
		os.mkdir(projectName)
		os.copyfile("./EngineLibraries/glew/bin/Release/Win32/glew32.dll", path.join(projectName, "glew32.dll"))
		os.copyfile("./EngineLibraries/SDL2/lib/win32/SDL2.dll", path.join(projectName, "SDL2.dll"))
		os.copyfile("./EngineLibraries/assimp/bin/assimp-vc130-mtd.dll", path.join(projectName, "assimp-vc130-mtd.dll"))
		os.copyfile("./EngineLibraries/SDL2_image/lib/x86/SDL2_image.dll", path.join(projectName, "SDL2_image.dll"))
		os.copyfile("./EngineLibraries/SDL2_image/lib/x86/libjpeg-9.dll", path.join(projectName, "libjpeg-9.dll"))
		os.copyfile("./EngineLibraries/SDL2_image/lib/x86/libpng16-16.dll", path.join(projectName, "libpng16-16.dll"))
		os.copyfile("./EngineLibraries/SDL2_ttf/lib/x86/SDL2_ttf.dll", path.join(projectName, "SDL2_ttf.dll"))
		os.copyfile("./EngineLibraries/SDL2_ttf/lib/x86/libfreetype-6.dll", path.join(projectName, "libfreetype-6.dll"))
		os.copyfile("./EngineLibraries/SDL2_ttf/lib/x86/zlib1.dll", path.join(projectName, "zlib1.dll"))
		os.copyfile("./EngineLibraries/lua/lib/lua.dll", path.join(projectName, "lua.dll"))
