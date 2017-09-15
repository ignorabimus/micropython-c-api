call "%VS140COMNTOOLS%vsvars32.bat"

msbuild toy-example.vcxproj /p:configuration=release /p:platform=win32
msbuild toy-example.vcxproj /p:configuration=debug /p:platform=win32
