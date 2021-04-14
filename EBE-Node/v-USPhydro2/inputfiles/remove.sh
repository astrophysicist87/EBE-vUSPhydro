#!/bin/bash

#for ((m=4; m<=4; m++)) 
#do 
#    for ((n=1000; n<=30000; n++)) 
#    do 
#        rm UUE200GeV/"$m"/ic"$n".dat	
#    done
#done

for ((n=1000; n<=30000; n++))
    do
        rm trento/UUD200GeV/0/ic"$n".dat
    done
