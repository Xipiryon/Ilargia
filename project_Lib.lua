
-- Library
-------------------------------------------

project "IlargiaLibrary"
	local ProjectRoot = os.getcwd()

	language "C++"
	targetname "Ilargia"
	targetdir (SolutionRoot.."/bin/lib")

	files {
		ProjectRoot.."/src/**.cpp",
		ProjectRoot.."/include/**.hpp",
	}
	filter "Debug*"
		links	{ "Muon-d" }
	filter "Release*"
		links { "Muon" }

	filter "*DLL"
		defines { "ILARGIA_EXPORTS" }

