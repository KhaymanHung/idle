param(
    [ValidateSet('Debug', 'Release')][string]$Configuration = 'Release',
    [string]$QtPrefix = "",
    [ValidateSet('MinGW Makefiles', 'Ninja')][string]$Generator = 'MinGW Makefiles'
)

$ErrorActionPreference = 'Stop'

$repoRoot = Split-Path -Parent $PSScriptRoot

$preferredQtPaths = @(
    "$repoRoot/vendor/Qt/5.15.2/mingw64",
    "$repoRoot/vendor/Qt/5.15.2/mingw81_64",
    "$repoRoot/vendor/Qt/5.15.2/mingw_64",
    'C:/Qt/5.15.2/mingw64',
    'C:/Qt/5.15.2/mingw81_64',
    'C:/Qt/Tools/mingw1120_64'
)

$hasGcc = $null -ne (Get-Command g++ -ErrorAction SilentlyContinue)
$hasMingwMake = $null -ne (Get-Command mingw32-make -ErrorAction SilentlyContinue)
$hasNinja = $null -ne (Get-Command ninja -ErrorAction SilentlyContinue)

if ($Generator -eq 'MinGW Makefiles' -and -not ($hasGcc -or $hasMingwMake)) {
    throw "未找到 MinGW/GCC toolchain。請先安裝 MinGW 或 GCC。"
}

if ($Generator -eq 'Ninja' -and -not $hasNinja) {
    throw "未找到 Ninja。請先安裝 Ninja，或改用 MinGW Makefiles。"
}

if ($QtPrefix -and (Test-Path $QtPrefix)) {
    $resolvedQtPrefix = $QtPrefix
} else {
    $resolvedQtPrefix = $preferredQtPaths | Where-Object { $_ -and (Test-Path $_) } | Select-Object -First 1
}

if (-not $resolvedQtPrefix) {
    throw "找不到 Qt 5 MinGW 版本。請將 Qt MinGW 版放在 vendor/Qt/5.15.2/mingw* 或設定 -QtPrefix。"
}

$buildDir = "$repoRoot/build/windows-mingw"

cmake -S $repoRoot -B $buildDir -G $Generator -DCMAKE_BUILD_TYPE=$Configuration -DCMAKE_PREFIX_PATH="$resolvedQtPrefix"
cmake --build $buildDir --parallel
