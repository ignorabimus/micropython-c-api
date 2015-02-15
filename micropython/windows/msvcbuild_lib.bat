call "%VS120COMNTOOLS%vsvars32.bat"

msbuild micropython.vcxproj /p:configuration=librelease /p:platform=win32
msbuild micropython.vcxproj /p:configuration=libdebug /p:platform=win32
