
-- Executable
-------------------------------------------

project "Ilargia_Executable"

    dependson("Ilargia_Core")

    language "C++"
    targetname "Ilargia"
    targetdir (SolutionRoot.."/bin")

    files {
        SolutionRoot.."/main/main.cpp"
    }

    links {
        "Muon_Core",
        "Ilargia_Core"
    }

    filter "Debug*"
        kind "ConsoleApp"

    filter "Release*"
        kind "ConsoleApp"

    filter "Final*"
        kind "WindowedApp"

    filter {}
