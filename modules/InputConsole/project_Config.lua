--[[
    ***********************
        This file will be included in top-project premake
    ***********************
--]]
------------------------------
-- Project
------------------------------

local ProjectName = "Module_InputConsole"

project(ProjectName)
    local ProjectRoot = os.getcwd()
    local ProjectDefineModuleName = "ILARGIA_MODULE_NAME="..ProjectName

    dependson("Ilargia_Core")
    language "C++"
    targetdir(SolutionRoot.."/bin/plugins")
    targetsuffix ""

    libdirs { ProjectRoot.."/lib" }
    includedirs { ProjectRoot.."/include" }

    files {
        ProjectRoot.."/**.cpp",
        ProjectRoot.."/**.hpp",
    }

    defines { "ILARGIA_EXPORTS", ProjectDefineModuleName }

    links { "Muon_Core", "Ilargia_Core" }

    filter {}
