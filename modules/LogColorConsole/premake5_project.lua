--[[ 	
	***********************
		This file will be included in top-project premake
	***********************
--]]
------------------------------
-- Project
------------------------------

G_ProjectName = "IlargiaLogColorConsole"

project(G_ProjectName)
	local ProjectRoot = os.getcwd()
	
	language "C++"
	targetdir(SolutionRoot.."/bin/lib")
	--defines { "ILG_MODULE_NAME="..G_ProjectName }
	
	if os.is("windows") then
		postbuildcommands { string.gsub("copy "..SolutionRoot.."/bin/lib/*.dll "..SolutionRoot.."/bin/", "/", "\\") }
	else
		postbuildcommands { "cp "..SolutionRoot.."/bin/lib/*.so "..SolutionRoot.."/bin/" }
	end

	files {
       SolutionRoot.."/modules/"..G_ProjectName.."/**.cpp",
	   SolutionRoot.."/modules/"..G_ProjectName.."/**.hpp",
	}
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