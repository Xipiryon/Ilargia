
-- Library
-------------------------------------------

project "IlargiaLibrary"
	local ProjectRoot = os.getcwd()

	language "C++"
	targetname "Ilargia"
	targetdir (SolutionRoot.."/bin/lib")

	if not os.is("windows") then
		linkoptions {"-ldl"}
	end

	files {
		ProjectRoot.."/src/**.cpp",
		ProjectRoot.."/include/**.hpp",
		ProjectRoot.."/src/**SharedLibrary.*pp",
	}
	filter "Debug*"
		links	{ "Muon-d" }
		defines { "ILARGIA_DEBUG" }

	filter "Release*"
		links { "Muon" }

	filter "*DLL"
		defines { "ILARGIA_EXPORTS" }

