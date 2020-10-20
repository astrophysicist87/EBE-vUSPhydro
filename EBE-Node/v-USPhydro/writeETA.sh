#!/bin/bash



sed 's,EIN,'"$1"',g ; s,eEIN,'e"$1"',g ;   s,_EIN,_'"$1"', ; s,/EIN,/'"$1"','  runETA.sh > runETA_"$1".sh

chmod +x runETA_"$1".sh

cd inputfiles

sed 's,EIN,'"$1"', ; s,eEIN,'e"$1"',g ;  s,_EIN,_'"$1"', ; s,/EIN,/'"$1"','  inputtrentoXeXeeta.dat > inputtrentoXeXeeta_21_e"$1".dat

sed 's,EIN,'"$1"', ; s,eEIN,'e"$1"',g ;  s,_EIN,_'"$1"', ; s,/EIN,/'"$1"','  inputtrentoPbPb2eta.dat > inputtrentoPbPb2eta_21_e"$1".dat

mkdir /projects/jn511_1/v-USPhydro/outputfiles/trento/PbPb5020TeV/shear/EOS21/e"$1"
mkdir /projects/jn511_1/v-USPhydro/df/out/trento/PbPb5020TeV/shear/EOS21/e"$1"
mkdir /projects/jn511_1/v-USPhydro/df/input/trento/PbPb5020TeV/shear/EOS21/e"$1"
mkdir /projects/jn511_1/v-USPhydro/outputfiles/trento/XeXe5440TeVdef/shear/EOS21/e"$1"
mkdir /projects/jn511_1/v-USPhydro/df/out/trento/XeXe5440TeVdef/shear/EOS21/e"$1"
mkdir /projects/jn511_1/v-USPhydro/df/input/trento/XeXe5440TeVdef/shear/EOS21/e"$1"



cd ..

sbatch runETA_"$1".sh
