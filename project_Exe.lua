
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

		dependson("Module_"..project)

		filter "DebugLib"
			links{ project.."-d" }

		filter "ReleaseLib"
			links{ project.."-r" }

		filter "FinalLib"
			links{ project.."-f" }
	end

	filter {}
