#!/bin/bash                                                                                                                                                             

#1- DIRECTORY
#2- FILENAME
#3- EVENTS0
#4- EVENTSF
#5- GRID


sed 's,DIRECTORY,'"$1"',; s,FILENAME,'"$1"', ; s,EVENTS0,'"$3"', ; s,EVENTSF,'"$4"', ; s,GRID,'"$5"','  ECC.sh > ECC_"$1".sh

chmod +x ECC_"$1".sh

sbatch -A qgp  ECC_"$1".sh
