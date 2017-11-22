@echo off
setlocal EnableDelayedExpansion

if [%YOUIENGINE_HOME%]==[] (
    pushd ..\..\..\..
    set YOUIENGINE_HOME=!CD!
    popd

    if NOT EXIST !YOUIENGINE_HOME!/tools/workflow/buildAssets.rb (
        echo The YOUIENGINE_HOME environment variable has not been set.
        echo Please set this environment variable to point to the installation directory
        echo of You.i Engine.
        exit /b 1
    )
)

ruby %YOUIENGINE_HOME%/tools/workflow/buildAssets.rb %*
exit /b %ERRORLEVEL%
