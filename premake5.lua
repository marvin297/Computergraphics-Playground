-- premake5.lua
workspace "Playground"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "Playground"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
include "Walnut/WalnutExternal.lua"

include "Playground"