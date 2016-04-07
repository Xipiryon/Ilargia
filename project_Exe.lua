
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
		links	{ "Ilargia-d", "Muon-d" }
		defines { "ILARGIA_DEBUG" }

	filter "Release*"
		kind "WindowedApp"
		links { "Ilargia", "Muon" }
