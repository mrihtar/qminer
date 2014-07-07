set REL_PATH=v8\third_party\python_26
set PYTHON26=
rem // save current directory and change to target directory
pushd %REL_PATH%
rem // save value of CD variable (current directory)
set PYTHON26=%CD%
rem // restore original directory
popd

set path=%PYTHON26%;%path%
svn co http://v8.googlecode.com/svn/branches/3.26/ v8

cd v8

svn co http://gyp.googlecode.com/svn/trunk build/gyp
svn co http://src.chromium.org/svn/trunk/tools/third_party/python_26@89111 third_party/python_26
svn co http://src.chromium.org/svn/trunk/deps/third_party/cygwin@231940 third_party/cygwin
svn co https://src.chromium.org/chrome/trunk/deps/third_party/icu46 third_party/icu


third_party\python_26\python.exe build\gyp_v8 -Dtarget_arch=x64 -Dcomponent=shared_library

"c:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\IDE\devenv.com" /build "Release|x64" build\All.sln
"c:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\IDE\devenv.com" /build "Debug|x64" build\All.sln

cd ..

copy /y v8\build\Release\lib\v8_base.x64.lib
copy /y v8\build\Release\lib\v8_snapshot.lib
copy /y v8\build\Release\lib\icui18n.lib
copy /y v8\build\Release\lib\icuuc.lib

md debug
cd debug
copy /y ..\v8\build\Debug\lib\v8_base.x64.lib
copy /y ..\v8\build\Debug\lib\v8_snapshot.lib
copy /y ..\v8\build\Debug\lib\icui18n.lib
copy /y ..\v8\build\Debug\lib\icuuc.lib

cd ..
