Ilargia
====================

C++ Modular Game Engine

[![Travis](https://travis-ci.org/Xipiryon/Ilargia.svg)](https://travis-ci.org/Xipiryon/Ilargia)
[![AppVeyor](https://ci.appveyor.com/api/projects/status/kt1ylr5d8d48pfe0/branch/master?svg=true)](https://ci.appveyor.com/project/Xleek/haize/branch/master)

[![Coverity](https://scan.coverity.com/projects/8400/badge.svg)](https://scan.coverity.com/projects/xipiryon-ilargia)

How to compile
====================

You will need the following:
- A C++11 compiler
- Premake5 (See bottom of this README)
- Muon Library ( http://github.com/Xleek/Muon )
- Optionally, an IDE
- Optionally, Git

The premake build system is used to generate IDE solutions or makefiles.
It's been choosed over CMake because it is very small (and can be fearlessly embedded in the repository),
it uses a well-known language (Lua) rather than a custom cryptic one, which makes writing build scripts very easy.

1) Follow the Muon installation steps
2) Get the engine from its Git repository, or download a zip file from Github
3) Run premake5 for the platform / target you want to build
4) You should now see your project / make file at the root of the repository

Example with Visual Studio 2013:
1) Open a command prompt in the repository
2) Run `premake5.exe vs2013`
3) A set of Visual Studio file ( Ilargia.sln, Ilargia.vcproj, ...) are now located in the repository root folder
4) Open the Ilargia.sln file
5) Build the solution
6) Run the `premake5.exe install` command. _(Depending on where you want to install, you may require administrator rights)_

If you've pulled a new version of the project, it is recommanded to regenerate the solution using premake and rebuild.
Unless major changes, you should not have the need to rebuild & reinstall the library.

Note
====================

* The `premake5 install` command will install header and libraries (.dll, .lib, .so, ...) files
	* On Windows, they're installed under the C:\Premake folder
	* On Unix, they're installed under the /usr/local/ folder _(and may require the use of `sudo` )_

License
====================
This project is licenced under the Creative Commons - Attribution & Share Alike licence

[![Licence](https://i.creativecommons.org/l/by-sa/4.0/88x31.png)](http://creativecommons.org/licenses/by-sa/4.0/)

Premake5
====================

You can get the source from the premake-dev repo and build it yourself, or directly download the executable at:
http://premake.github.io/download.html

The repository also have both Unix and Windows premake executable, resepctively premake5 and premake5.exe.
(You may require to `chmod +x premake5` on Unix)
