SET QHC="C:\Qt\5.1.0\mingw48_32\bin\qhelpgenerator"
SET DOXY="C:\Program Files\doxygen\bin\doxygen"
%DOXY% usbase.doxy
copy offline.css usbase\html\tabs.css
%QHC% usbase\html\index.qhp -o usbase.qch
pause
SET QHC=