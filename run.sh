OPT="opt"
if [ $1 = $OPT ]
then
./run-sniper -c ../Lab0/config-opt.cfg -- ../Lab0/lab0.exe
else 
./run-sniper -c ../Lab0/config-lru.cfg -- ../Lab0/lab0.exe
fi
