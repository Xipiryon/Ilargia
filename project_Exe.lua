
-- Executable
-------------------------------------------

project "IlargiaExecutable"
	local ProjectRoot = os.getcwd()

	dependson("Ilargia")
	language "C++"
	targetname "Ilargia"
	targetdir (SolutionRoot.."/bin")

	files	{
		ProjectRoot.."/main/main.cpp"
	}

	filter "Debug*"
		kind "ConsoleApp"
		links { "Muon-d", "Ilargia-d" }

	filter "Release*"
		links { "Muon-r", "Ilargia-r" }

	filter "Final*"
		kind "WindowedApp"
		links { "Muon-f", "Ilargia-f" }

	filter {}

	for _,project in pairs(G_ModuleToBuild) do

		dependson(project)

		filter "DebugLib"
			links{ project.."-d" }

		filter "ReleaseLib"
			links{ project.."-r" }

		filter "FinalLib"
			links{ project.."-f" }
	end

	filter {}
