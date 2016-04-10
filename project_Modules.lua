
-- Modules
-------------------------------------------

-- Specify the list of module to build within the engine
-- You can simply comment / uncomment to build / unbuild
G_ModuleToBuild = {
	"ColorConsole",
	"InputConsole",
	--"LogHTML"
	--"Network"
}

for _,project in pairs(G_ModuleToBuild) do
	print('Building Module: "'..project..'"')
	dofile("./modules/"..project.."/project_Lib.lua");
end
