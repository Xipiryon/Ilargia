------------------------------
-- Global Variable
------------------------------

-- Prefix Install specify where it should look for custom
-- project header & library files
if G_Install == nil then

	G_Install = {}
	-- Default Value on Windows & Unix Platform
	if os.is("windows") then
		G_Install.Root = "C:/Premake/"
	else
		G_Install.Root = "/usr/local/"
	end
end
if not G_Install.Header then G_Install.Header = G_Install.Root.."include" end
if not G_Install.Lib then G_Install.Lib = G_Install.Root.."lib" end

if SolutionRoot == nil then
	SolutionRoot = os.getcwd()
end

-- Default value for _OPTIONS
if _OPTIONS["renderer"] == nil then _OPTIONS["renderer"] = "opengl" end
------------------------------
-- Solution
------------------------------

solution "Ilargia"
	startproject "Ilargia_Executable"
	configurations { "DebugDLL", "DebugLib", "ReleaseLib", "ReleaseDLL", "FinalLib", "FinalDLL" }

	implibdir "bin/lib"
	if os.is("windows") then
		buildoptions { "" }
	else
		buildoptions { "--std=c++11" }
	end

	-- If option exists, then override G_Install
	if _OPTIONS["installdir"] then
		G_Install.Root = _OPTIONS["installdir"]
		G_Install.Header = _OPTIONS["installdir"].."/include"
		G_Install.Lib = _OPTIONS["installdir"].."/bin/lib"
		print("Install directory has been overwritten to '"..G_Install.Root.."'")
	end

	includedirs {
		SolutionRoot.."/include",
		G_Install.Header,
		-- External dependencies
		SolutionRoot.."/extern/gl3w",
		SolutionRoot.."/extern/picojson",
	}

	if os.is("windows") then
		includedirs {
			SolutionRoot.."/extern/dirent",
		}
	end


	flags {
		"NoImplicitLink",
		"NoIncrementalLink",
		"NoEditAndContinue",
	}

	-- Add external include
	if _OPTIONS["buildmuon"] then
		includedirs { SolutionRoot.."/extern/Muon/include" }
	end
	if _OPTIONS["buildhaize"] then
		includedirs { SolutionRoot.."/extern/Haize/include" }
	end

	libdirs {
		SolutionRoot.."/bin/lib",
		G_Install.Lib
	}

	filter "Debug*"
		targetsuffix "-d"
		optimize "Debug"
		flags   { "Symbols" }
		defines { "ILARGIA_DEBUG"}

	filter "Release*"
		targetsuffix "-r"
		optimize "Speed"
		flags   { "Symbols" }

	filter "Final*"
		targetsuffix "-f"
		optimize "Full"
		flags   { "LinkTimeOptimization" }

	filter  "*Lib"
		kind "StaticLib"
		flags { "StaticRuntime" }
		defines { "ILARGIA_STATIC" }
		if _OPTIONS["buildmuon"] then defines { "MUON_STATIC" } end
		if _OPTIONS["buildhaize"] then defines { "HAIZE_STATIC" } end

	filter  "*DLL"
		kind "SharedLib"

	filter {}

------------------------------
-- Project
------------------------------

-- Muon
if _OPTIONS["buildmuon"] then
	include("extern/Muon/project_Lib")
end
-- Haize
if _OPTIONS["buildhaize"] then
	include("extern/Haize/project_Lib")
end
-- Ilargia
if _OPTIONS["renderer"] == "opengl" then
	include("extern/gl3w/project_Lib")
end
include("project_Modules")
include("project_Lib")
include("project_Exe")

if _OPTIONS["unittests"] then
	include("project_UnitTests")
end

------------------------------
-- Options
------------------------------

newoption {
	trigger     = "installdir",
	value       = "PATH",
	description = "Folder to search lib & include; default: '"..G_Install.Root.."'",
}

newoption {
	trigger     = "unittests",
	description = "Enable compilation of unit tests",
}

newoption {
	trigger     = "buildmuon",
	description = "Add Muon external project to the solution",
}

newoption {
	trigger     = "buildhaize",
	description = "Add Haize external project to the solution",
}

newoption {
   trigger     = "renderer",
   value       = "API",
   description = "Choose a particular 3D API for rendering (default: opengl)",
   allowed = {
      { "opengl",    "OpenGL" },
      { "none",  "No Renderer" }
   }
}
------------------------------
-- Actions
------------------------------

newaction {
	trigger	 = "install",
	description = "Install developpment files & library",
	execute = function ()
		print("** Installing Header files in: "..G_Install.Header.." **")

		local incDir = SolutionRoot.."/include/"
		local libDir = SolutionRoot.."/bin/lib/"

		-- Create required folders
		local dirList = os.matchdirs(incDir.."**")
		for _,fdir in pairs(dirList) do
			local destDir = G_Install.Header..string.sub(fdir, #incDir)

			if(not os.isdir(destDir)) then
				if os.mkdir(destDir) then print("Creating "..destDir) end
			end
		end

		-- Copy files
		local fileList = os.matchfiles(incDir.."**")
		for _,fpath in pairs(fileList) do
			local destFile = G_Install.Header..string.sub(fpath, #incDir)
			if os.copyfile(fpath, destFile) then print("Installing "..destFile) end
		end


		-- LIBRARY
		print("** Installing Library files in: "..G_Install.Lib.." **")
		local destDir = G_Install.Lib

		-- Create required folders
		if(not os.isdir(destDir)) then
			if os.mkdir(destDir) then print("Creating "..destDir) end
		end

		local exts = {}
		if os.is("windows") then
			exts[0] = ".dll"
			exts[1] = ".lib"
		else
			exts[0] = ".so"
			exts[1] = ".a"
		end

		-- Copy files
		for _,ext in pairs(exts) do
			local fileList = os.matchfiles(libDir.."**"..ext)
			for _,fpath in pairs(fileList) do
				local destFile = G_Install.Lib..string.sub(fpath, #libDir)
				if os.copyfile(fpath, destFile) then print("Installing "..destFile) end
			end
		end
	end
}

if os.is("windows") then
	newaction {
		trigger	 = "getlib",
		description = "Retrieve libraries from 'installdir' and put them in bin/",
		execute = function ()
			print("** Retrieving files from: "..G_Install.Lib.." **")

			local libDir = G_Install.Lib

			local destDir = "bin"

			-- Create required folders
			if(not os.isdir(destDir)) then
				if os.mkdir(destDir) then print("Creating "..destDir) end
			end

			-- Copy files
			local fileList = os.matchfiles(libDir.."**Muon**dll")
			for _,fpath in pairs(fileList) do
				local destFile = destDir..string.sub(fpath, 1+#G_Install.Lib)
				if os.copyfile(fpath, destFile) then print("Copying "..fpath.." to "..destDir) end
			end
		end
	}
end
