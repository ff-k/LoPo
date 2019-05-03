@echo off

pushd bin
pushd x86

if "%Platform%" == "X64" (
    popd
    pushd x64
)

set KDBParams=%*
set KDBMesh=%1
call set KDBOptions=%%KDBParams:*%1=%%

call kdb.exe -m ..\..\%KDBMesh% %KDBOptions%

popd
popd