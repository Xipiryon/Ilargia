
-- Executable
-------------------------------------------

project "IlargiaExecutable"
	local ProjectRoot = os.getcwd()
	
	dependson("IlargiaLibrary")
	language "C++"
	targetname "Ilargia"
	targetdir (SolutionRoot.."/bin")
	kind "ConsoleApp"

	files	{
		ProjectRoot.."/main/main.cpp"
	}

	filter "Debug*"
		links	{ "Ilargia-d", "Muon-d" }

	filter "Release*"
		links { "Ilargia", "Muon" }
