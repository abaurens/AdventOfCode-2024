-- Day 03 (project)
project "Day 03"
  kind "ConsoleApp"
  language "C++"
  cppdialect "C++20"
  staticruntime "On"

  targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
  objdir ("%{wks.location}/build/" .. outputdir .. "%{prj.name}")

  pchheader "AdventOfCode.hpp"
  pchsource ("source/pch.cpp")

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
    "AdventOfCode"
  }
