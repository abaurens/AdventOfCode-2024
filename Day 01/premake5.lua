-- Day 01 (project)
project "Day 01"
  kind "ConsoleApp"
  language "C++"
  cppdialect "C++20"
  staticruntime "On"

  targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
  objdir ("%{wks.location}/build/" .. outputdir .. "%{prj.name}")

  IncludeDir["Day 01"] = "%{wks.location}/Day 01/include"

  -- Not using pre compiled header yet --
  -- pchheader "pch.h"
  -- pchsource ("src/pch.cpp")

  files {
    "premake5.lua",

    "include/**.h",
    "include/**.hpp",

    "source/**.h",
    "source/**.c",
    "source/**.hpp",
    "source/**.cpp",
    "source/**.tpp",

    "assets/*"
  }

  includedirs {
    "include/",
    "source/"
  }

  defines {
  }

  links {
  }

  filter "system:linux"
    pic "On"
  
  filter "system:macosx"
    pic "On"

  filter "configurations:Debug"
    runtime "Debug"
    symbols "On"
    
  filter "configurations:Release"
    defines "NDEBUG"
    runtime "Release"
    optimize "On"