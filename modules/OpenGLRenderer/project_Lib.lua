--[[
	***********************
		This file will be included in top-project premake
	***********************
--]]
------------------------------
-- Project
------------------------------

G_ProjectName = "Module_OpenGLRenderer"

project(G_ProjectName)
	local ProjectRoot = os.getcwd()
	local ProjectNameDefine = "ILARGIA_MODULE_NAME="..G_ProjectName

	dependson("Ilargia_Core")
	language "C++"
	targetdir(SolutionRoot.."/bin/lib")

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

	defines { "ILARGIA_EXPORTS", ProjectNameDefine }

	filter "*DLL"

		links { "Muon_Core", "Ilargia_Core" }

		if os.is("windows") then
			postbuildcommands { string.gsub("copy "..SolutionRoot.."/bin/lib/"..G_ProjectName.."*.dll "..SolutionRoot.."/bin/plugins/", "/", "\\") }
		else
			postbuildcommands { "find "..SolutionRoot.."/bin/lib/ -name lib"..G_ProjectName.."*.so -exec cp {} "..SolutionRoot.."/bin/plugins/ \\;" }
		end

	filter {}
