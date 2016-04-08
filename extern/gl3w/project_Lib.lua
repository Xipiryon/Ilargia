
-- Library
-------------------------------------------

project "gl3w"
	local ProjectRoot = os.getcwd()

	language "C++"
	targetname "gl3w"
	targetdir (SolutionRoot.."/bin/lib")

	files {
		ProjectRoot.."/*.c",
		ProjectRoot.."/*.h",
	}

	links {
		"opengl32",
	}

	-- GL3W must be linked statically with no suffix
	filter "*"
		kind "StaticLib"
		targetsuffix ""