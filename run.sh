OPT="opt"
if [ $1 = $OPT ]
then
./run-sniper -c ../Lab0/config-opt.cfg -- ./test/exe/$2.exe
else 
./run-sniper -c ../Lab0/config-lru.cfg -- ./test/exe/$2.exe
fi
