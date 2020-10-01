FOR /F "tokens=1 delims=" %%I IN ('DIR /A /B res.txt') DO ((MORE +46 "%%I">"%%I_")&(DEL /A /F /Q "%%I")&(REN "%%I_" "%%I"))
PAUSE