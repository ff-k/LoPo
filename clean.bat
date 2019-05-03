@echo off

pushd bin
pushd x86

if "%Platform%" == "X64" (
    popd
    pushd x64
)

call del kdb_*obj
call del kdb.pdb
call del kdb.exe

popd
popd