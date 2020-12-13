config="config"
file1="./mytest/no_false_sharing.exe"
file2="./mytest/half_false_sharing.exe"
file3="./mytest/full_false_sharing.exe"
outputfile="./benchmark.out"
echo "Benchmark" > $outputfile
echo "###### $testfile ######" >> $outputfile
title=`echo "|    | 16KB,16B | 16KB,32B | 16KB,64B | 32KB,16B | 32KB,32B | 32KB,64B | 64KB,16B | 64KB,32B | 64KB,64B|"`
echo $title >> $outputfile
cutline=`echo "|----| ----     | ----     | ----     | ----     | ----     | ----     | ----     | ----     | ----    |"`
echo $cutline >> $outputfile
missRateTable="$title\n$cutline"
for policy in msi mesi mesif
do 
    for testfile in $file1 $file2 $file3
    do 
        line="|$policy,$testfile|"
        missRateTable="$missRateTable\n$line"
        for cache_size in 32
        do 
            for block_size in 64
            do 
                configfile="./myconfig/$config-${cache_size}kb-${block_size}B.cfg"
                ./change_coherency_policy.sh $configfile 23 $policy
                ./run-sniper -c $configfile -- $testfile
                missrate1=`sed -n 38,38p sim.out | cut -d "|" -f2`
                missrate2=`sed -n 38,38p sim.out | cut -d "|" -f3`
                missrate3=`sed -n 38,38p sim.out | cut -d "|" -f4`
                missrate4=`sed -n 38,38p sim.out | cut -d "|" -f5`
                traffic1=`sed -n 53,53p sim.out | cut -d "|" -f2`
                traffic2=`sed -n 53,53p sim.out | cut -d "|" -f3`
                traffic3=`sed -n 53,53p sim.out | cut -d "|" -f4`
                traffic4=`sed -n 53,53p sim.out | cut -d "|" -f5`
                line="${line}${traffic1},${traffic2},${traffic3},${traffic4}|"
                missRateTable="${missRateTable}${missrate1},${missrate2},${missrate3},${missrate4}|"
            done
        done
        echo $line >> $outputfile
    done
done

echo "\n------miss rate table------\n" >> $outputfile
echo $missRateTable >> $outputfile