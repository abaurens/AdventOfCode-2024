-- Ascii-GL (workspace)

require "premake/workspace-files"

workspace "Advent of Code 2024"
  architecture("x86_64")
  startproject("Day 01")

  configurations {
    "Release",
    "Debug",
    "Debug-NoThreads"
  }

IncludeDir = {}
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Enables OpenMP API for shared-memory parallel programming
--openmp "On"

--vectorextensions "SSE"
--vectorextensions "SSE2"
--vectorextensions "SSE3"
--vectorextensions "SSE4.1"
--vectorextensions "SSE4.2"

includedirs {
  "%{IncludeDir.AdventOfCode}",
  "include/",
  "source/"
}

defines {
  "_CRT_SECURE_NO_WARNINGS",
  "_USE_MATH_DEFINES",
  "PROJECT_NAME=\"%{wks.name}\"",
  "DAY_NAME=\"%{prj.name}\""
}

workspace_files {
  "README.md",
  "premake5.lua",
  ".editorconfig",
  ".gitignore"
}

filter "configurations:windows"
  defines "_WIN32"

filter "configurations:Debug-NoThreads"
  defines "SINGLE_THREADED"

filter "configurations:Debug*"
  defines "_DEBUG"
  runtime "Debug"
  symbols "On"
  
filter "configurations:Release"
  runtime "Release"
  optimize "On"

group ""
  include("AdventOfCode")

group "Days"
  include("Day 01")
  include("Day 02")
  include("Day 03")
  include("Day 04")
  include("Day 05")
  include("Day 06")
  include("Day 07")
  include("Day 08")
  include("Day 09")
  include("Day 10")
  include("Day 11")
  include("Day 12")
  --include("Day 13")
  --include("Day 14")
  --include("Day 15")
  --include("Day 16")
  --include("Day 17")
  --include("Day 18")
  --include("Day 19")
  --include("Day 20")
  --include("Day 21")
  --include("Day 22")
  --include("Day 23")
  --include("Day 24")
