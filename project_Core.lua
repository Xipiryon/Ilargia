
-- Library
-------------------------------------------

project "Ilargia_Core"

    if _OPTIONS["buildmuon"] then dependson { "Muon_Core" } end

    dependson { "gl3w" }
    language "C++"
    targetname "Ilargia"
    targetdir (SolutionRoot.."/bin/lib")

    if not os.is("windows") then linkoptions {"-ldl"} end

    files {
        SolutionRoot.."/src/Ilargia/**",
        SolutionRoot.."/include/Ilargia/**",
    }

    defines { "ILARGIA_EXPORTS" }

    links { "Muon_Core" }

    filter "*DLL"
        if os.is("windows") then
            postbuildcommands { string.gsub("copy "..SolutionRoot.."/bin/lib/Ilargia*.dll "..SolutionRoot.."/bin/", "/", "\\") }
        else
            postbuildcommands { "find "..SolutionRoot.."/bin/lib/ -name libIlargia*.so -exec cp {} "..SolutionRoot.."/bin/ \\;" }
        end

    filter {}
