@echo off

pushd bin
REM pushd x86
REM 
REM if "%Platform%" == "X64" (
REM     popd
    pushd x64
REM )
REM 
REM set KDBParams=%*
REM set KDBMesh=%1
REM call set KDBOptions=%%KDBParams:*%1=%%

REM call kdb.exe -m ..\..\%KDBMesh% %KDBOptions%

set LoPoMeshes=..\..\assets\meshes\ceiling.obj ..\..\assets\meshes\cube_inward.obj ..\..\assets\meshes\cube_outward.obj ..\..\assets\meshes\cylinder.obj ..\..\assets\meshes\flitri.obj ..\..\assets\meshes\floor.obj ..\..\assets\meshes\icosphere.obj ..\..\assets\meshes\plane.obj ..\..\assets\meshes\platform.obj ..\..\assets\meshes\pyramid.obj ..\..\assets\meshes\sphere.obj
call kdb.exe --meshes 11 %LoPoMeshes%

REM  0: ceiling.obj 
REM  1: cube_inward.obj 
REM  2: cube_outward.obj 
REM  3: cylinder.obj 
REM  4: flitri.obj 
REM  5: floor.obj 
REM  6: icosphere.obj 
REM  7: plane.obj 
REM  8: platform.obj 
REM  9: pyramid.obj 
REM 10: sphere.obj

popd
popd