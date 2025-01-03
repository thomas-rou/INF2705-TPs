zip -r INF2705_remise_tp2.zip *.cpp *.h *.glsl makefile *.txt shaders  && echo "remettre fichier INF2705_remise_tp2.zip fait avec zip"
IF %ERRORLEVEL% EQU 0 GOTO Fin

7z a -r INF2705_remise_tp2.7z *.cpp *.h *.glsl makefile *.txt shaders && echo "remettre fichier INF2705_remise_tp2.7z fait avec 7z"
IF %ERRORLEVEL% EQU 0 GOTO Fin

rar a INF2705_remise_tp2.rar *.cpp *.h *.glsl makefile *.txt shaders && echo "remettre fichier INF2705_remise_tp2.tar fait avec rar"
IF %ERRORLEVEL% EQU 0 GOTO Fin

tar cvf INF2705_remise_tp2.tar *.cpp *.h *.glsl makefile *.txt shaders && echo "remettre fichier INF2705_remise_tp2.tar fait avec tar"
IF %ERRORLEVEL% EQU 0 GOTO Fin

echo "Ne sait pas comment faire INF2705_remise_tp2..."

:Fin
