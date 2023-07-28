gcc -c code_generator.c
code_generator.exe
gcc assembly.s -o out
out.exe
echo %errorlevel%
