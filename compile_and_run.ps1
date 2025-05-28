Write-Host "Compiling"
cmake -G "MinGW Makefiles" -S . -B dist
cd dist
cmake --build .
if (Test-Path -Path app.exe){
    .\app.exe
}
cd ..
