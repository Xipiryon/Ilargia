--[[
    ***********************
        This file will be included in top-project premake
    ***********************
--]]
------------------------------
-- Project
------------------------------

local ProjectName = "Module_OpenGLRenderer"

project(ProjectName)
    local ProjectRoot = os.getcwd()
    local ProjectDefineModuleName = "ILARGIA_MODULE_NAME="..ProjectName

    dependson("Ilargia_Core")
    language "C++"
    targetdir(SolutionRoot.."/bin/plugins")
    targetsuffix ""

    libdirs { ProjectRoot.."/lib" }
    includedirs { ProjectRoot.."/include" }

    if os.is("windows") then
        linkoptions { "/NODEFAULTLIB:libcmt.lib" }
    end

    files {
        ProjectRoot.."/main.cpp",
        ProjectRoot.."/include/**",
        ProjectRoot.."/src/**",
    }

    links {
        --"opengl32",
        --"glfw3",
        --"gl3w",
    }

    defines { "ILARGIA_EXPORTS", ProjectDefineModuleName }

    filter {}
