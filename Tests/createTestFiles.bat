@echo off
setlocal enabledelayedexpansion

set "targetDir=testData"
set "numFiles=1024"
set "strLength=15"

if not exist "%targetDir%" mkdir "%targetDir%"

set "characters=abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()"

for /l %%i in (1, 1, %numFiles%) do (
    set "filename=!targetDir!\file_%%i.txt"
    
    rem Generating a random string of letters and numbers
    set "text="
    for /l %%j in (1, 1, %strLength%) do (
        set /a "rand=!Random! %% 71"
        for %%c in (!rand!) do set "text=!text!!characters:~%%c,1!"
    )

    echo !text! > !filename!
)

echo Files created successfully in %targetDir%.







