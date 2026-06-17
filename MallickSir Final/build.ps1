# Build MathEngine on Windows (no CMake required)
$ErrorActionPreference = "Stop"

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $scriptDir

$gpp = Get-Command g++ -ErrorAction SilentlyContinue
if (-not $gpp) {
    $msysGpp = "C:\msys64\ucrt64\bin\g++.exe"
    if (Test-Path $msysGpp) {
        $env:Path = "C:\msys64\ucrt64\bin;" + $env:Path
    } else {
        Write-Error "g++ not found. Install MSYS2 (ucrt64) or add g++ to PATH."
    }
}

$sources = @(
    "array_ops.cpp",
    "stack_queue.cpp",
    "expression_engine.cpp",
    "singly_linked.cpp",
    "doubly_linked.cpp",
    "sorting.cpp",
    "bst.cpp",
    "bit.cpp",
    "heap.cpp",
    "graph.cpp",
    "symbolic.cpp",
    "math_engine.cpp",
    "terminal_ui.cpp"
)

Write-Host "Building MathEngine..."
& g++ -std=c++17 -I. -o MathEngine.exe @sources
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

Write-Host "Done. Run with: .\MathEngine.exe"
