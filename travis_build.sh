# Initialize sub modules
git submodule init
git submodule update
# Build Muon library
cd extern/Muon
chmod +x premake5
./premake5 gmake
make config=releasedll -j4
# "Install" Muon in a temporary folder
mkdir ../../tmp_install 2> /dev/null
./premake5 install --basedir="../../tmp_install"
# Go back to Ilargia
cd ../..
chmod +x premake5
# Generate YACC Parser files
./premake5 genparser
# Generate a gmake make file, specifying libs and includes are in
./premake5 gmake --basedir="./tmp_install"
