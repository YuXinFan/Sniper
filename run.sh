OPT="opt"
if [ $1 = $OPT ]
then
./run-sniper -c ../Lab0/config-opt.cfg -- ./test/fft/fft.exe
else 
./run-sniper -c ../Lab0/config-lru.cfg -- ./test/fft/fft.exe
fi
