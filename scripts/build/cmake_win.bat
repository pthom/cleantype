set THIS_DIR=%~dp0
set SOURCE_DIR=%~dp0\..\..
cmake %SOURCE_DIR% -DCLEANTYPE_ALL_OPTIONS=ON -G"Visual Studio 15 2017 Win64"
