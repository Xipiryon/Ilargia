
-- Modules
-------------------------------------------

-- Specify the list of module to build within the engine
-- You can simply comment / uncomment to build / unbuild
G_ModuleToBuild = {
	{ ["Folder"] = "ColorConsole",		["Name"] = "?"},
	{ ["Folder"] = "OpenGLRenderer",	["Name"] = "?"},
	{ ["Folder"] = "InputConsole",		["Name"] = "?"},
	--"LogHTML"
	--"Network"
}

print('Building additional modules...')
for _,project in pairs(G_ModuleToBuild) do
	-- Load project file
	dofile("./modules/"..project.Folder.."/project_Lib.lua");
	-- Extract Project name
	project.Name = G_ProjectName;
	print('... '..project.Name)
end
