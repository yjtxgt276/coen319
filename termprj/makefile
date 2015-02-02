all: encryption decryption 
test: keySchedule byteSub shiftRow mixCol addRoundKey
encryption:
	nvcc -rdc=true encrypt.cu keySchedule.cu byteSub.cu shiftRow.cu mixCol.cu addRoundKey.cu ioUtil.cu -o encrypt.exe
decryption:
	nvcc -rdc=true decrypt.cu keySchedule.cu byteSub.cu shiftRow.cu mixCol.cu addRoundKey.cu ioUtil.cu -o decrypt.exe
keySchedule:
	nvcc keyScheduleTestdrive.cu keySchedule.cu -o keySchedule.exe
byteSub:
	nvcc -rdc=true byteSub.cu byteSubTestdrive.cu -o byteSub.exe
shiftRow:
	nvcc -rdc=true shiftRow.cu shiftRowTestdrive.cu -o shiftRow.exe
mixCol:
	nvcc -rdc=true mixCol.cu mixColTestdrive.cu -o mixCol.exe
addRoundKey:
	nvcc -rdc=true addRoundKey.cu addRoundKeyTestdrive.cu -o addRoundKey.exe
cleanall:
	rm -fv *.exe *.out *.o *.cipher *.plain
cleanoutputs:
	rm -fv *.out *.cipher *.plain
