function file_check(file_name)
  local file_found=io.open(file_name, "r")      

  if file_found==nil then
    return false
  end
  return true
end


solution "VoxelGL"
 
    ------------------------------------------------------------------
    -- setup common settings
    ------------------------------------------------------------------
	defines 
	{ 
		"_CRT_SECURE_NO_WARNINGS",
        "ELPP_THREAD_SAFE"
	}
	configurations 
	{ 
		"PriDeb", 
		"PriRel", 
		"PubRel" 
	}

	vectorextensions "SSE2"
	floatingpoint "Fast"
    
    includedirs 
    { 
        "../Include/", 
        "../Include/SDL",
        "../Code/",         
        "../Code/log",
	}
    
    if os.is("windows") then
    links
    {
        "ws2_32",
        "winmm",
        "opengl32",
        "glu32",
    }
    end
	
    location "projects"
    platforms { "x32", "x64" }
	
	configuration { "vs*"}
		buildoptions 
		{ 
			"/wd4512",
			"/wd4996",
			"/wd4018"
		}
        defines
        {
            "WIN32",
            "GLEW_STATIC"
        }
        debugdir "Runtime"
       
	configuration "x32"
        libdirs { "../Libs/x86" }
        targetdir "bin/x32"
      
    configuration "x64"
        libdirs { "../Libs/x64" }
        targetdir "bin/x64"
    
    configuration "PriDeb"
        defines { "DEBUG", "INTERNAL" }
        flags { "Symbols" }
        targetsuffix "_d"
 
    configuration "PriRel"
        defines { "NDEBUG", "INTERNAL" }
        optimize "On"
		targetsuffix "_r"
		
	configuration "PubRel"
        defines { "NDEBUG", "PUBLIC_BUILD" }
        optimize "On"
        
    filter "files:**.c"
        flags { "NoPCH" }
 
	------------------------------------------------------------------
    -- logic projects
    ------------------------------------------------------------------

        group "Client"		
            project "Game"
                targetname "Game"
                kind "ConsoleApp"
                language "C++"
                
                defines
                {
                }
                
                includedirs 
                { 
                    "../Code/Game/include", 
                    "../Code/Game/",              
                }
                
                files 
                { 
                    "../Code/Game/include/**.h", 
                    "../Code/Game/src/**.cpp",
                    "../Code/Game/src/**.def",
                }
                
                pchheader "Base.h"
                pchsource "../Code/Game/src/Base.cpp"
                
                
                filter "files:**.cpp"
                    forceincludes { "Base.h" }
    
                filter {}
    
    if os.is("windows") then  
                files 
                { 
                    "../Code/Game/glew/**.c",
                }
    end    
                
                libdirs 
                {
                    "lib" 
                }
  
    if os.is("windows") then  
                links 
                { 
                    "ws2_32",
                    "winmm",
                    "dbghelp",
                    "SDL2main",
                    "SDL2",
                    "SDL2_image",
                    "SDL2_ttf",
                    "SDL2_mixer"
                }
    end          

