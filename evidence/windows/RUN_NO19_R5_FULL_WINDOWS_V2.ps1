param(
    [string]$ZipPath = "",
    [int]$ExpectedMaxMinutes = 120
)

$ErrorActionPreference = "Stop"
$ProgressPreference = "SilentlyContinue"

$ExpectedZipSha256 = "FAF59AC907F6FEFFBB52B7848A5F92E8B3366CBB6C3D9473710C06ED6A2CB59C"
$ExpectedBaseName = "NO19_EXACT_NINE_CERTIFIER_REVIEW_R5_20260923.zip"

$Expected = [ordered]@{
    "NO19_CASE_II_DD" = @{
        orbits=822; second=449280; pairs=9615680; local=0; bsc=0
    }
    "NO19_CASE_II_DQ" = @{
        orbits=822; second=4703040; pairs=44971752; local=88; bsc=0
    }
    "NO19_CASE_I_PP" = @{
        orbits=3030; second=1333920; pairs=91957456; local=677; bsc=0
    }
    "NO19_CASE_I_PQ_PSING_QANY" = @{
        orbits=3030; second=4703040; pairs=136030656; local=677; bsc=0
    }
    "NO19_CASE_I_PQ_PPERM_QSING" = @{
        orbits=5520; second=1074240; pairs=70824720; local=430; bsc=0
    }
    "NO19_CASE_O_PP" = @{
        orbits=569; second=1281600; pairs=23496800; local=0; bsc=0
    }
    "NO19_CASE_O_PQ_PSING_QANY" = @{
        orbits=569; second=4616640; pairs=34903200; local=0; bsc=0
    }
    "NO19_CASE_O_PQ_PPERM_QSING" = @{
        orbits=1186; second=987840; pairs=20328240; local=0; bsc=0
    }
}

function Get-Metric([string]$Text, [string]$Name) {
    $m = [regex]::Match($Text, "(?m)^" + [regex]::Escape($Name) + "=(\d+)\s*$")
    if (-not $m.Success) { throw "Missing metric: $Name" }
    return [int64]$m.Groups[1].Value
}

function Assert-Eq($actual, $expected, [string]$label) {
    if ($actual -ne $expected) {
        throw "STOP_MISMATCH $label expected=$expected actual=$actual"
    }
}

$downloads = Join-Path $env:USERPROFILE "Downloads"
if ([string]::IsNullOrWhiteSpace($ZipPath)) {
    $exact = Join-Path $downloads $ExpectedBaseName
    if (Test-Path -LiteralPath $exact) {
        $ZipPath = $exact
    } else {
        $cand = Get-ChildItem -LiteralPath $downloads -File |
            Where-Object { $_.Name -like "NO19_EXACT_NINE_CERTIFIER_REVIEW_R5_20260923*.zip" } |
            Sort-Object LastWriteTime -Descending |
            Select-Object -First 1
        if (-not $cand) {
            throw "R5 ZIP not found in Downloads."
        }
        $ZipPath = $cand.FullName
    }
}
$ZipPath = (Resolve-Path -LiteralPath $ZipPath).Path

Write-Host "=== NO19 R5 FULL WINDOWS REPRODUCTION V2 (stderr-safe) ==="
Write-Host "ZIP=$ZipPath"
$zipSha = (Get-FileHash -LiteralPath $ZipPath -Algorithm SHA256).Hash.ToUpperInvariant()
Write-Host "ZIP_SHA256=$zipSha"
if ($zipSha -ne $ExpectedZipSha256) {
    throw "STOP_ZIP_SHA256_MISMATCH expected=$ExpectedZipSha256 actual=$zipSha"
}
Write-Host "PASS_ZIP_SHA256"

$stamp = Get-Date -Format "yyyyMMdd_HHmmss"
$root = Join-Path $downloads "NO19_R5_FULL_RUN_$stamp"
New-Item -ItemType Directory -Path $root -Force | Out-Null
Expand-Archive -LiteralPath $ZipPath -DestinationPath $root -Force
$work = Join-Path $root "pkg"
if (-not (Test-Path -LiteralPath $work)) {
    throw "pkg directory missing after extraction."
}
Set-Location $work

$portableHeader = @'
#pragma once
#if defined(_MSC_VER)
static inline unsigned no19_ctz(unsigned x) {
    unsigned n = 0;
    while ((x & 1u) == 0u) { x >>= 1u; ++n; }
    return n;
}
static inline int no19_popcount(unsigned x) {
    int c = 0;
    while (x) { x &= (x - 1u); ++c; }
    return c;
}
#define __builtin_ctz no19_ctz
#define __builtin_popcount no19_popcount
#endif
'@
Set-Content -LiteralPath ".\NO19_MSVC_BUILTINS.hpp" -Value $portableHeader -Encoding ASCII

$compilerKind = $null
$compiler = $null
if (Get-Command g++.exe -ErrorAction SilentlyContinue) {
    $compilerKind = "g++"
    $compiler = (Get-Command g++.exe).Source
} elseif (Get-Command clang++.exe -ErrorAction SilentlyContinue) {
    $compilerKind = "clang++"
    $compiler = (Get-Command clang++.exe).Source
} else {
    $vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    if (-not (Test-Path -LiteralPath $vswhere)) {
        throw "No g++, clang++, or Visual Studio compiler found."
    }
    $vs = & $vswhere -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
    if ([string]::IsNullOrWhiteSpace($vs)) {
        throw "Visual Studio C++ Build Tools not found."
    }
    $vcvars = Join-Path $vs "VC\Auxiliary\Build\vcvars64.bat"
    if (-not (Test-Path -LiteralPath $vcvars)) {
        throw "vcvars64.bat not found: $vcvars"
    }
    $compilerKind = "msvc"
    $compiler = $vcvars
}

Write-Host "COMPILER_KIND=$compilerKind"
Write-Host "COMPILER=$compiler"

$cppSources = @(
    "NO19_CASE_II_DD.cpp",
    "NO19_CASE_II_DQ.cpp",
    "NO19_CASE_I_PP.cpp",
    "NO19_CASE_I_PQ_PSING_QANY.cpp",
    "NO19_CASE_I_PQ_PPERM_QSING.cpp",
    "NO19_CASE_O_PP.cpp",
    "NO19_CASE_O_PQ_PSING_QANY.cpp",
    "NO19_CASE_O_PQ_PPERM_QSING.cpp",
    "NO19_UNARY_SAFE_ENUMERATOR.cpp",
    "NO19_UNARY_COUNTS_INDEPENDENT_V2.cpp",
    "NO19_SINGULAR_COUNTS_THIRD_BRUTE.cpp",
    "NO19_II_DD_ORBIT_AUDIT_INDEPENDENT.cpp",
    "NO19_ALL_ORBIT_QUOTIENT_AUDIT_INDEPENDENT.cpp"
)

Write-Host "[1/5] Compiling C++ certifiers..."
foreach ($src in $cppSources) {
    $base = [IO.Path]::GetFileNameWithoutExtension($src)
    $exe = "$base.exe"
    if ($compilerKind -eq "g++" -or $compilerKind -eq "clang++") {
        & $compiler -O3 -std=c++17 $src -o $exe
        if ($LASTEXITCODE -ne 0) { throw "Compile failed: $src" }
    } else {
        $cmd = "`"$compiler`" >nul && cl /nologo /O2 /std:c++17 /EHsc /FI`"NO19_MSVC_BUILTINS.hpp`" `"$src`" /Fe:`"$exe`""
        cmd.exe /d /c $cmd
        if ($LASTEXITCODE -ne 0) { throw "MSVC compile failed: $src" }
    }
}
Write-Host "PASS_COMPILE_ALL"

$runStart = Get-Date
$summary = @()
$totalPairs = [int64]0
$totalLocal = [int64]0
$totalBsc = [int64]0

Write-Host "[2/5] Running all 8 exhaustive branch certifiers SEQUENTIALLY..."
foreach ($name in $Expected.Keys) {
    $exe = ".\$name.exe"
    $log = Join-Path $work "$name.FRESH_RERUN.log"
    Write-Host ""
    Write-Host "=== RUN $name ==="
    $sw = [Diagnostics.Stopwatch]::StartNew()
    $lines = & cmd.exe /d /c "$exe 2>&1"
    $exit = $LASTEXITCODE
    $lines | Tee-Object -FilePath $log | ForEach-Object { Write-Host $_ }
    $sw.Stop()
    if ($exit -ne 0) { throw "Branch failed: $name exit=$exit" }

    $text = ($lines | Out-String)
    $orbits = Get-Metric $text "A_orbit_reps"
    $second = Get-Metric $text "B_unary_candidates"
    $pairs = Get-Metric $text "pairs_full_checked"
    $local = Get-Metric $text "local_survivors"
    $bsc = Get-Metric $text "bsc_survivors"

    Assert-Eq $orbits $Expected[$name].orbits "$name.orbits"
    Assert-Eq $second $Expected[$name].second "$name.second"
    Assert-Eq $pairs $Expected[$name].pairs "$name.pairs"
    Assert-Eq $local $Expected[$name].local "$name.local"
    Assert-Eq $bsc $Expected[$name].bsc "$name.bsc"

    $totalPairs += $pairs
    $totalLocal += $local
    $totalBsc += $bsc
    $summary += [pscustomobject]@{
        Branch=$name
        Orbits=$orbits
        SecondLetters=$second
        FullPairs=$pairs
        LocalSurvivors=$local
        BSCSurvivors=$bsc
        Seconds=[math]::Round($sw.Elapsed.TotalSeconds,3)
    }
    Write-Host "PASS_BRANCH $name seconds=$([math]::Round($sw.Elapsed.TotalSeconds,2))"
}

Assert-Eq $totalPairs 432128504 "TOTAL.pairs"
Assert-Eq $totalLocal 1872 "TOTAL.local"
Assert-Eq $totalBsc 0 "TOTAL.bsc"
Write-Host "PASS_ALL_8_BRANCHES"
Write-Host "TOTAL_FULL_PAIRS=$totalPairs"
Write-Host "TOTAL_LOCAL_SURVIVORS=$totalLocal"
Write-Host "TOTAL_BSC_SURVIVORS=$totalBsc"

Write-Host "[3/5] Running exact-9 and affine Python audits..."
$py = (Get-Command python.exe -ErrorAction Stop).Source
$pyScripts = @(
    "NO19_EXACT9_WITNESS_CHECK.py",
    "NO19_DIRECT_WORD_AUDIT.py",
    "NO19_AFFINE_BASIS_CROSSCHECK.py",
    "NO19_LOCAL_SURVIVOR_CROSSBRANCH_SPOTCHECK.py"
)
foreach ($s in $pyScripts) {
    Write-Host "=== $s ==="
    $pyCmd = "`"$py`" `"$s`" 2>&1"
    $pyLines = & cmd.exe /d /c $pyCmd
    $pyExit = $LASTEXITCODE
    $pyLines | Tee-Object -FilePath "$s.FRESH_RERUN.log" | ForEach-Object { Write-Host $_ }
    if ($pyExit -ne 0) { throw "Python audit failed: $s exit=$pyExit" }
}
Write-Host "PASS_PYTHON_AUDITS"

Write-Host "[4/5] Running independent C++ count/orbit audits..."
$extraExes = @(
    "NO19_UNARY_SAFE_ENUMERATOR",
    "NO19_UNARY_COUNTS_INDEPENDENT_V2",
    "NO19_SINGULAR_COUNTS_THIRD_BRUTE",
    "NO19_II_DD_ORBIT_AUDIT_INDEPENDENT",
    "NO19_ALL_ORBIT_QUOTIENT_AUDIT_INDEPENDENT"
)
foreach ($name in $extraExes) {
    Write-Host "=== $name ==="
    $extraExe = ".\$name.exe"
    $extraLines = & cmd.exe /d /c "$extraExe 2>&1"
    $extraExit = $LASTEXITCODE
    $extraLines | Tee-Object -FilePath "$name.FRESH_RERUN.log" | ForEach-Object { Write-Host $_ }
    if ($extraExit -ne 0) { throw "Independent audit failed: $name exit=$extraExit" }
}
Write-Host "PASS_INDEPENDENT_CPP_AUDITS"

Write-Host "[5/5] Writing final evidence..."
$runEnd = Get-Date
$elapsed = New-TimeSpan -Start $runStart -End $runEnd

$summary | Export-Csv -NoTypeInformation -Encoding UTF8 ".\NO19_R5_FRESH_BRANCH_SUMMARY.csv"
$summary | Format-Table -AutoSize | Out-String | Set-Content ".\NO19_R5_FRESH_BRANCH_SUMMARY.txt" -Encoding UTF8

$final = @"
NO19_R5_FULL_WINDOWS_REPRODUCTION
ZIP_SHA256=$zipSha
COMPILER_KIND=$compilerKind
TOTAL_FULL_PAIRS=$totalPairs
TOTAL_LOCAL_SURVIVORS=$totalLocal
TOTAL_BSC_SURVIVORS=$totalBsc
ELAPSED_SECONDS=$([math]::Round($elapsed.TotalSeconds,3))
ELAPSED_MINUTES=$([math]::Round($elapsed.TotalMinutes,3))
PASS_ALL_8_BRANCHES=true
PASS_PYTHON_AUDITS=true
PASS_INDEPENDENT_CPP_AUDITS=true
FINAL_PASS=true
"@
Set-Content ".\NO19_R5_FULL_WINDOWS_FINAL.txt" -Value $final -Encoding ASCII

$evidenceZip = Join-Path $downloads "NO19_R5_FULL_WINDOWS_EVIDENCE_$stamp.zip"
Compress-Archive -Path `
    ".\*.FRESH_RERUN.log", `
    ".\NO19_R5_FRESH_BRANCH_SUMMARY.csv", `
    ".\NO19_R5_FRESH_BRANCH_SUMMARY.txt", `
    ".\NO19_R5_FULL_WINDOWS_FINAL.txt" `
    -DestinationPath $evidenceZip -Force

$evSha = (Get-FileHash -LiteralPath $evidenceZip -Algorithm SHA256).Hash
Write-Host ""
Write-Host "========================================"
Write-Host "FINAL_PASS=true"
Write-Host "TOTAL_FULL_PAIRS=$totalPairs"
Write-Host "TOTAL_LOCAL_SURVIVORS=$totalLocal"
Write-Host "TOTAL_BSC_SURVIVORS=$totalBsc"
Write-Host "ELAPSED_MINUTES=$([math]::Round($elapsed.TotalMinutes,2))"
Write-Host "EVIDENCE_ZIP=$evidenceZip"
Write-Host "EVIDENCE_ZIP_SHA256=$evSha"
Write-Host "========================================"

if ($elapsed.TotalMinutes -gt $ExpectedMaxMinutes) {
    Write-Warning "Run exceeded the expected $ExpectedMaxMinutes minute window, but completed successfully."
}
