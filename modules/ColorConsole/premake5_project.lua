--[[
	***********************
		This file will be included in top-project premake
	***********************
--]]
------------------------------
-- Project
------------------------------

G_ProjectName = "ColorConsole"

project(G_ProjectName)
	local ProjectRoot = os.getcwd()
	local ProjectNameDefine = "ILARGIA_MODULE_NAME="..G_ProjectName

	dependson("IlargiaLibrary")
	language "C++"
	targetdir(SolutionRoot.."/bin/lib")

	if os.is("windows") then
		postbuildcommands { string.gsub("copy "..SolutionRoot.."/bin/lib/"..G_ProjectName.."*.dll "..SolutionRoot.."/bin/", "/", "\\") }
	else
		postbuildcommands { "cp "..SolutionRoot.."/bin/lib/"..G_ProjectName.."*.so "..SolutionRoot.."/bin/" }
	end

	files {
		ProjectRoot.."/**.cpp",
		ProjectRoot.."/**.hpp",
	}

	defines { ProjectNameDefine }

	filter "Debug*"
		links	{
			"Muon-d",
			"Ilargia-d"
		}
	filter "Release*"
		links {
			"Muon",
			"Ilargia"
		}

	filter "*DLL"
		defines { "ILARGIA_EXPORTS" }
