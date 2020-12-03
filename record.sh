echo $1  >> result_record
echo "raw"  >> result_record
./run-sniper -c ./config-raw.cfg -- ./mytest/"$1"
sleep 1s
sed -n 8,12p sim.out >> result_record
echo "new"  >> result_record
./run-sniper -c ./config-new.cfg -- ./mytest/"$1"
sed -n 8,12p sim.out >> result_record
echo "------" >> result_record