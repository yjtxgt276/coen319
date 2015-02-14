all: cpudec cpuenc 
cleanall:
	rm -fv *.exe *.out *.o *.cipher *.plain
cleanoutputs:
	rm -fv *.out *.cipher *.plain
cpuenc:
	gcc -std=gnu99 cpuenc.c cpuaeslib.c -o cpuenc.exe
cpudec:
	gcc -std=gnu99 cpudec.c cpuaeslib.c -o cpudec.exe
