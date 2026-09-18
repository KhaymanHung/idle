param(
    [ValidateSet('Debug', 'Release')][string]$Configuration = 'Release',
    [string]$QtPrefix = 'D:/Qt/5.15.2/msvc2019_64'
)

$ErrorActionPreference = 'Stop'

if (-not (Test-Path $QtPrefix)) {
    throw "Qt 路徑不存在: $QtPrefix"
}

cmake -S . -B build/windows -G "Visual Studio 17 2022" -DCMAKE_BUILD_TYPE=$Configuration -DCMAKE_PREFIX_PATH="$QtPrefix"
cmake --build build/windows --config $Configuration --parallel
