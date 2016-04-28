--[[
	***********************
		This file will be included in top-project premake
	***********************
--]]
------------------------------
-- Project
------------------------------

G_ProjectName = "Module_ColorConsole"

project(G_ProjectName)
	local ProjectRoot = os.getcwd()
	local ProjectNameDefine = "ILARGIA_MODULE_NAME="..G_ProjectName

	dependson("Ilargia_Core")
	language "C++"
	targetdir(SolutionRoot.."/bin/lib")

	libdirs { ProjectRoot.."/lib" }
	includedirs { ProjectRoot.."/include" }

	files {
		ProjectRoot.."/**.cpp",
		ProjectRoot.."/**.hpp",
	}

	defines { "ILARGIA_EXPORTS", ProjectNameDefine }

	links { "Muon_Core", "Ilargia_Core" }

	filter "*DLL"
		if os.is("windows") then
			postbuildcommands { string.gsub("copy "..SolutionRoot.."/bin/lib/"..G_ProjectName.."*.dll "..SolutionRoot.."/bin/plugins/", "/", "\\") }
		else
			postbuildcommands { "find "..SolutionRoot.."/bin/lib/ -name lib"..G_ProjectName.."*.so -exec cp {} "..SolutionRoot.."/bin/plugins/ \\;" }
		end

	filter {}