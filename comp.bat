set PATH=C:\psyq\bin
set PSYQ_PATH=C:\psyq\bin

set prog=MAIN
set address=$80100000
::set param=-DCDROM_RELEASE
set lib=..\..\psxlib

echo off
cls
ccpsx -I%lib%\. %param% -O2 -Xo%address% %prog%.c %lib%\System.c %lib%\Sprite.c %lib%\Sound.c %lib%\DataManager.c -o%prog%.cpe,,%prog%.map
pause

cpe2x %prog%.cpe
del %prog%.cpe
