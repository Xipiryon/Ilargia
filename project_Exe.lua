
-- Executable
-------------------------------------------

project "Ilargia_Executable"
	local ProjectRoot = os.getcwd()

	dependson("Ilargia_Core")

	if _OPTIONS["buildhaize"] then
		dependson { "Muon_Haize" }
	end

	language "C++"
	targetname "Ilargia"
	targetdir (SolutionRoot.."/bin")

	files	{
		ProjectRoot.."/main/main.cpp"
	}

	for _,project in pairs(G_ModuleToBuild) do
		dependson(project.Name)
		links{ project.Name }
	end

	links { "Muon_Core", "Ilargia_Core" }

	filter "Debug*"
		kind "ConsoleApp"

	filter "Release*"
		kind "ConsoleApp"

	filter "Final*"
		kind "WindowedApp"

	filter {}
