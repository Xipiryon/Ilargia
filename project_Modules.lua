
-- Modules
-------------------------------------------

local function scandir(directory)
    local i, t, popen = 0, {}, io.popen
    local pfile = popen('ls -a "'..directory..'"')
    for filename in pfile:lines() do
        if filename ~= "." and filename ~= ".." then
            i = i + 1
            t[i] = filename
        end
    end
    pfile:close()
    return t
end

local projectsCount = { ["Added"] = 0, ["Skipped"] = 0 }
local folderToExclude = {}

-- Specify skip folder here, format is:
-- folderToExclude[FOLDER_NAME] = true
if _OPTIONS["renderer"] ~= "opengl" then folderToExclude["OpenGLRenderer"] = true end

print('Looking for additional modules ...')
for _,project in pairs(scandir('modules/')) do
    if folderToExclude[project] then
        print('  -  '..project)
        projectsCount.Skipped = projectsCount.Skipped + 1
    else
        -- Load project file
        include("modules/"..project.."/project_Config.lua")
        -- Extract Project name and Project folder
        --table.insert(G_ModuleToBuild, { ["Folder"] = project, ["Name"] = G_ProjectName } )
        print('  +  '..project)
        projectsCount.Added = projectsCount.Added + 1
    end
end
print('... Done (Added: '..projectsCount.Added..', Skipped: '..projectsCount.Skipped..')')
