# EBE-vUSPhydro

Event-by-event package for running Jaki Noronha-Hostler's hydrodynamics code v-USPhydro

Steps:
  (1) - Insert any relevant hydrodynamic parameters or settings into ParameterDict.py
  (2) - Run
             ./generateJobs_cluster.py [number of jobs] [number of events per job] [name of cluster/partition (e.g., qgp)]
  (3) - Run
             ./submitJobs_cluster.py
  (4) - Sit back and wait!
  (5) - All simulations carried out in PlayGround folder
  (6) - All simulation results copied to RESULTS folder
  (7) - Celebrate your Nobel prize
