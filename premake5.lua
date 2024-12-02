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

defines {
  "_CRT_SECURE_NO_WARNINGS",
  "_USE_MATH_DEFINES"
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

-- dependencies compiled from source
group "Dependencies"

group ""
  include("Day 01")
  include("Day 02")
