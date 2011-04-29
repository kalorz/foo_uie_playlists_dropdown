@echo off

D:\Apps\foobar2000.dev\foobar2000.exe /exit
ping 127.0.0.1 -n 1 > NUL

copy /Y D:\foobar2000\foobar2000\foo_uie_playlists_dropdown\Release\foo_uie_playlists_dropdown.dll D:\Apps\foobar2000.dev\components

start D:\Apps\foobar2000.dev\foobar2000.exe