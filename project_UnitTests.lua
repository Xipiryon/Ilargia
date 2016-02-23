
-- Unit Tests
-------------------------------------------

project "UnitTests"
	local ProjectRoot = os.getcwd()

	language "C++"
	targetname "UnitTests"
	targetdir (SolutionRoot.."/bin")
	kind "ConsoleApp"

	files	{
		ProjectRoot.."/unittests/main.cpp",
	}

	filter "Debug*"
		links	{ "Ilargia-d", "Muon-d" }

	filter "Release*"
		links { "Ilargia", "Muon" }
