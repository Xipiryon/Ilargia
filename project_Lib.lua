
-- Library
-------------------------------------------

project "Ilargia_Core"
	local ProjectRoot = os.getcwd()


	if _OPTIONS["buildmuon"] then
		dependson { "Muon_Core" }
	end

	dependson { "gl3w" }
	language "C++"
	targetname "Ilargia"
	targetdir (SolutionRoot.."/bin/lib")

	if not os.is("windows") then
		linkoptions {"-ldl"}
	end

	-- Core Engine files
	files {
		ProjectRoot.."/src/Ilargia/*",
		ProjectRoot.."/include/Ilargia/*",
	}
	-- Engine files
	for _,folder in pairs({ "Core", "Component", "Manager", "System", "System/TinyXML2", "Type" }) do
		files {
			ProjectRoot.."/src/Ilargia/"..folder.."/*",
			ProjectRoot.."/include/Ilargia/"..folder.."/*",
		}
	end
	-- Renderer files
	if _OPTIONS["renderer"] == "opengl" then
		files {
			ProjectRoot.."/src/Ilargia/Graphics/OpenGL/*",
			ProjectRoot.."/include/Ilargia/Graphics/OpenGL/*",
		}
		links {
			"opengl32",
			"gl3w",
		}
	end

	defines { "ILARGIA_EXPORTS" }

	links { "Muon_Core" }

	filter "*DLL"
		if os.is("windows") then
			postbuildcommands { string.gsub("copy "..SolutionRoot.."/bin/lib/Ilargia*.dll "..SolutionRoot.."/bin/", "/", "\\") }
		else
			postbuildcommands { "find "..SolutionRoot.."/bin/lib/ -name libIlargia*.so -exec cp {} "..SolutionRoot.."/bin/ \\;" }
		end

	filter {}
