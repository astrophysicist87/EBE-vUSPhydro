#!/bin/bash
#PBS -d /projects/jnorhos/v-USPhydro/df
#PBS -l walltime=00:30:00
#PBS -N XeXedef_21
#PBS -l nodes=1:ppn=1
#PBS -q secondary
#PBS -j oe
cd /projects/jnorhos/v-USPhydro/df

rm $PBS_O_WORKDIR/logs/*
rm $PBS_O_WORKDIR/runs/*

for (( i=0; i<=34; i++))
    do
    $PBS_O_WORKDIR/vns/fo trentoinputXeXedef_21_"$i".dat 0 999 pi+.dat 0.5 3 1 trentoXeXedef5440mul 0 > runs/XeXedef_21_"$i".dat
    done
  wait


  $PBS_O_WORKDIR/VNPT_XeXedef_21_pi+_0.5.sh
