
-- Library
-------------------------------------------

project "Ilargia"
	local ProjectRoot = os.getcwd()

	language "C++"
	targetname "Ilargia"
	targetdir (SolutionRoot.."/bin/lib")

	if not os.is("windows") then
		linkoptions {"-ldl"}
	end

	if _OPTIONS["buildmuon"] then
		dependson("Muon")
	end

	files {
		ProjectRoot.."/src/**",
		ProjectRoot.."/include/**",
	}


	defines { "ILARGIA_EXPORTS" }

	filter "Debug*"
		links	{ "Muon-d" }

	filter "Release*"
		links { "Muon-r" }

	filter "Final*"
		links { "Muon-f" }

	filter "*DLL"
		if os.is("windows") then
			postbuildcommands { string.gsub("copy "..SolutionRoot.."/bin/lib/Ilargia*.dll "..SolutionRoot.."/bin/", "/", "\\") }
		else
			postbuildcommands { "find "..SolutionRoot.."/bin/lib/ -name libIlargia*.so -exec cp {} "..SolutionRoot.."/bin/ \\;" }
		end

	filter {}
