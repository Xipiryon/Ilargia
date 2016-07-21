
-- Modules
-------------------------------------------

function scandir(directory)
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

-- Specify the list of module to build within the engine
-- You can simply comment / uncomment to build / unbuild
G_ModuleToBuild = {}

print('Building additional modules...')
for _,project in pairs(scandir('modules/')) do
	-- Load project file
	dofile("modules/"..project.."/project_Lib.lua");
	-- Extract Project name and Project folder
	table.insert(G_ModuleToBuild, { ["Folder"] = project, ["Name"] = G_ProjectName } )
	print('... '..G_ProjectName)
end
