
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

	if os.is("windows") then
		postbuildcommands { string.gsub("copy bin/lib/Ilargia*.dll bin/", "/", "\\") }
	else
		postbuildcommands { "cp bin/lib/Ilargia*.so bin/" }
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

