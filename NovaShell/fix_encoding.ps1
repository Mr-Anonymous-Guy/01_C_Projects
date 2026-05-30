$c = Get-Content 'run.bat'
$c | Set-Content -Encoding Ascii 'run2.bat'
Move-Item -Force 'run2.bat' 'run.bat'
Write-Host "Done - run.bat re-encoded as ASCII CRLF"
