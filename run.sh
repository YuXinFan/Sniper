if [ $# == 0 ]
then 
    echo "no arguments"
elif [ $# == 2 ]
then
    ./run-sniper -c ./myconfig/config-"$1".cfg -- ./mytest/"$2"
    miss_rate=`sed -n 38,38p sim.out`
    traffic=`sed -n 53,53p sim.out`
    echo $miss_rate
    echo $traffic
fi 
    