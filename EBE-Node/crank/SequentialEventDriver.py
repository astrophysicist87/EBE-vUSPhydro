#! /usr/bin/env python
# This package performs a sequential calculations of a given number of events,
# after reading parameters from ParameterDict.py. The most important control
# parameters are set in controlParameterList, for other parameters see
# allParameterLists. This package is intended to be working-at-background thus
# only basic output are generated. When necessary, other functions given in the
# package for single executables can be invoked individually for more
# flexibilities.
# The main entry is the sequentialEventDriverShell function.

from os import path, getcwd, remove, makedirs
from sys import stdout, exit
from shutil import move, copy, copytree, rmtree
from glob import glob
from subprocess import call
import numpy as np
import re

class ExecutionError(Exception): pass # used to signal my own exception

# set global default parameters
allParameterLists = [
    'controlParameterList',
    'initialConditionGeneratorControl',
    'initialConditionGeneratorParameters',
    'hydroControl',
    'hydroParameters',
    'freezeOutControl',
    'freezeOutParameters',
]

controlParameterList = {
    'simulation_type'       :   'hydro', # 'hybrid' or 'hydro'
    'niceness'              :   10,       # range from 0 to 19 for process priority, 0 for the highest priority
    'numberOfEvents'        :   1, # how many sequential calculations
    'rootDir'               :   path.abspath('../'),
    'resultDir'             :   path.abspath('../finalResults'), # final results will be saved here, absolute
    'eventResultDirPattern' :   'event-%d', # %d->event_id, where event results are saved
    'eventResultDir'        :   None, # used to pass event result folder from sequentialEventDriverShell to others
    'buildCMD'              :   'make build',
    'cleanCMD'              :   'make clean',
}

initial_condition_control = {
    #
}

initialConditionGeneratorControl = {
    #'centrality': '0-5%',  # centrality bin
    # centrality cut variable: total_entropy or Npart
    #'cut_type': 'total_entropy',
    #'initial_condition_type': 'trento', # type of initial conditions
    # file path for the pre-generated initial condition files
    #'pre-generated_initial_file_path': 'initial_conditions', 
    # name pattern for the initial condition files
    #'pre-generated_initial_file_pattern': 'sd_event_[0-9]*_block.dat',  
    #'pre-generated_initial_file_read_in_mode': 2, # read in mode for VISH2+1
    'mainDir'                       :   'jaki_trento',
    'dataDir'                       :   'data', # where initial conditions are stored, relative
    'initialFiles'                  :   'ic*.dat',  #initial density profile filenames
    'saveICFile'                    :   True, # whether to save initial condition file
    'numberOfEventsParameterName'   :   'nev',
    'executable'                    :   'bin/trento',
}

initialConditionGeneratorParameters = {
    'projectile'                    :   'Pb',
    'target'                        :   'Pb',
}

hydroControl = {
    'mainDir'               :   'v-USPhydro',  # options - 'v-USPhydro', 'vusphydro'
    'initialConditionDir'   :   'inputfiles', # hydro initial condition folder, relative
    'initialConditionFile'  :   'ic0.dat', # IC filename
    'settingsFile'          :   'settings.inp', # settings filename
    'runNumber'             :   0,
    'resultDir'             :   'outputfiles', # hydro results folder, relative
    'resultFiles'           :   '*', # results files
    'freezeOutInputFile'    :   'dfinput.dat',
    'saveICFile'            :   True, # whether to save initial condition file
    'saveResultGlobs'       :   ['*'], 
                                # files match these globs will be saved
    'executable'            :   'vusphydro',
}

hydroParameters = {
    #
}

freezeOutControl = {
    'mainDir'               :   'df',
    'inputDir'              :   'input', # hydro initial condition folder, relative
    'inputFile'             :   'dfinput.dat', # settings filename
    'resultDir'             :   'out', # hydro results folder, relative
    'resultFiles'           :   '*', # results files
    'saveResultGlobs'       :   ['*'], 
                                # files match these globs will be saved
    'executable'            :   'fo',
}

freezeOutParameters = {
    #
}


def readInParameters():
    """ Overwrite default parameter lists with those in ParameterDict. """
    try:
        import ParameterDict
        for aParameterList in allParameterLists:
            if aParameterList in dir(ParameterDict):
                exec("%s.update(ParameterDict.%s)" % (aParameterList, aParameterList))
    except (IOError, SyntaxError):
        raise ExecutionError("Errors trying to open/read the ParameterDict.py file!")



def get_initial_condition_list():
    """
        return a list of initial condition file
    """
    file_list = []
    nev = controlParameterList['numberOfEvents']
    file_list = [afile for afile in generateInitialConditions(nev)]
    return(file_list)




def generateInitialConditions(numberOfEvents):
    """
        Generate initial conditions using Trento. It then yield the absolute
        path for all the initial conditions.
    """
    ProcessNiceness = controlParameterList['niceness']
    # set directory strings
    initialConditionDirectory = path.join(controlParameterList['rootDir'], 
                                 initialConditionGeneratorControl['mainDir'])
    initialConditionDataDirectory = path.join(initialConditionDirectory, 
                                     initialConditionGeneratorControl['dataDir'])
    initialConditionExecutable = initialConditionGeneratorControl['executable']
    
    print initialConditionDirectory
    print initialConditionDataDirectory
    print initialConditionExecutable
    
    #print 1/0

    # clean up the data subfolder for output
    cleanUpFolder(initialConditionDataDirectory)

    # check executable
    checkExistenceOfExecutable(path.join(initialConditionDirectory, initialConditionExecutable))
    
    nevParamName = initialConditionGeneratorControl['numberOfEventsParameterName']
    initialConditionGeneratorParameters[nevParamName] = (numberOfEvents)
    # form assignment string
    #assignments = formAssignmentStringFromDict(initialConditionGeneratorParameters)
    assignments = ' ' + initialConditionGeneratorParameters['projectile'] + ' '\
                  + initialConditionGeneratorParameters['target'] + ' '\
                  + str(numberOfEvents) + ' --output ' + initialConditionDataDirectory
    print 'assignments = ', assignments
    # form executable string
    executableString = ("nice -n %d ./" % (ProcessNiceness) 
                        + initialConditionExecutable + assignments)
    # execute!
    run(executableString, cwd=initialConditionDirectory)

    # yield initial conditions
    file_list = glob(path.join(initialConditionDataDirectory, 
                               initialConditionGeneratorControl['initialFiles']))
    for aFile in file_list:
        # then yield it
        yield path.join(initialConditionDataDirectory, aFile)


def hydroWithInitialCondition(aFile):
    """
        Perform a single hydro calculation with the given absolute path to an
        initial condition. Yield the result files.
    """
    ProcessNiceness = controlParameterList['niceness']
    # set directory strings
    hydroDirectory = path.join(controlParameterList['rootDir'], 
                               hydroControl['mainDir'])
    hydroICDirectory = path.join(hydroDirectory, 
                                 hydroControl['initialConditionDir'])
    hydroResultsDirectory = path.join(hydroDirectory, 
                                      hydroControl['resultDir'])
    hydroExecutable = hydroControl['executable']
    
    print 'Check 1'
    print hydroDirectory
    print hydroICDirectory
    print hydroResultsDirectory
    print hydroExecutable
    
    #print 1/0

    # check executable
    checkExistenceOfExecutable(path.join(hydroDirectory, hydroExecutable))

    # clean up initial and results folder
    #DON'T DO THIScleanUpFolder(hydroICDirectory)
    cleanUpFolder(hydroResultsDirectory)

    # check existence of the initial conditions
    if not path.exists(aFile):
        raise ExecutionError("Hydro initial condition file %s not found!" 
                             % aFile)

    # move initial condition to the designated folder
    move(aFile, path.join(hydroICDirectory, 
                          hydroControl['initialConditionFile']))

    # form assignment string
    assignments = formAssignmentStringFromDict(hydroParameters)
    assignments = ' ' + hydroControl['settingsFile'] + ' '\
                      + str(hydroControl['runNumber']) + ' 0'
    # form executable string
    executableString = ("nice -n %d ./" % (ProcessNiceness) 
                        + hydroExecutable + assignments)
    # execute!
    run(executableString, cwd=hydroDirectory)

    # yield result files
    worthStoring = []
    for aGlob in hydroControl['saveResultGlobs']:
        worthStoring.extend(glob(path.join(hydroResultsDirectory, aGlob)))
    file_list = glob(path.join(hydroResultsDirectory, 
                               hydroControl['resultFiles']))
    for aFile in file_list:
        # check if this file worth storing, then copy to event result folder
        if aFile in worthStoring:
            copy(aFile, controlParameterList['eventResultDir'])
        # yield it
        yield path.join(hydroResultsDirectory, aFile)

def freezeOutWithHydroResultFiles(fileList):
    """
        Perform freeze-out calculation using the given list of hydro result files,
        followed by resonance calculations.
    """
    ProcessNiceness = controlParameterList['niceness']
    # set directory strings
    FODirectory = path.join(controlParameterList['rootDir'], 
                            freezeOutControl['mainDir'])
    FOResultsDirectory = path.join(FODirectory, freezeOutControl['resultDir'])
    FOExecutable = freezeOutControl['executable']
    
    print 'Check 2'
    print FODirectory
    print FOResultsDirectory
    print FOExecutable
	
    # check executable
    checkExistenceOfExecutables( path.join(FODirectory, FOExecutable) )

    # clean up operation folder
    #cleanUpFolder(FOOperationDirectory)

    # check existence of hydro result files and move them to operation folder
    #for aFile in fileList:
    #    if not path.exists(aFile):
    #        raise ExecutionError("Hydro result file %s not found!" % aFile)
    #    else:
    #        move(aFile, FOOperationDirectory)
    #copy(path.join(iSDirectory, 'EOS', 'chosen_particles_s95pv1.dat'), 
    #     path.join(iSDirectory, 'EOS', 'chosen_particles.dat'))
    # N.B. - The 'withDecayPhotons' option is now apparently necessary
    #copy(path.join(iSDirectory, 'EOS', 'pdg-s95pv1_withDecayPhotons.dat'), 
    #     path.join(iSDirectory, 'EOS', 'pdg.dat'))
    #copy(path.join(iSDirectory, 'EOS', 'EOS_tables', 's95p-v1', 'EOS_particletable.dat'), 
    #     path.join(iSDirectory, 'EOS', 'EOS_particletable.dat'))

    # execute!
    run("nice -n %d ./" % (ProcessNiceness) + FOExecutable, 
        cwd=FODirectory)

    # save some of the important result files
    worthStoring = []
    for aGlob in freezeOutControl['saveResultGlobs']:
        worthStoring.extend(glob(path.join(FOResultsDirectory, aGlob)))
    for aFile in glob(path.join(FOResultsDirectory, "*")):
        if aFile in worthStoring:
            move(aFile, controlParameterList['eventResultDir'])
            
            
            

def formAssignmentStringFromDict(aDict):
    """
        Generate a parameter-equals-value string from the given dictionary. The
        generated string has a leading blank.
    """
    result = ""
    for aParameter in aDict.keys():
        result += " {}={}".format(aParameter, aDict[aParameter])
    return result

def generate_vUSPhydro_input_from_dict():
    hydroDirectory = path.join(controlParameterList['rootDir'], hydroControl['mainDir'])
    open(path.join(hydroDirectory, hydroControl['initialConditionDir'],
                   hydroControl['settingsFile']), "w").write(
    """h: %f  dt: %f
equationsofmotion: %s EOS: %s
%s
%s
low: %s
etaconst: %d
viscfac_bulk_shear: %f %f
t0: %f
freezeoutT: %f
%s %f
ICs: %s
QM_fluc: %d
output_folder: %s
%d %d
freezeout: %d
dfinputfilename: %s
df_cor_file: %s
range(ptmax,ptstepsize,phisteps): %s
hadronl_list: %s
had_check: %s
""" % (0.3, 0.05, 'shear+bulk', 'table', 'tempcharm.dat', 'dervcharm.dat',
       'off', 1, 0, 0.04, 0.6, 150, 'trento', 120, '.', 3, '.', 5, 5, -1,
       hydroControl['freezeOutInputFile'], 'input/MHlistall.dat', 'input/ptphipoints.dat',
       'input/resoweakPDG2016Plus.dat', 'input/numbers16p.dat'))
       

#(0.3, 0.05, 'shear+bulk', 'table', 'tempcharm.dat', 'dervcharm.dat',
#       'off', 1, 0, 0.04, 0.6, 150, 'trento', 120, 'trento/PbPb5020TeV/0',
#       3, 'trento/PbPb5020TeV/shear/EOS211/0', 5, 5, 1, 'trentoinputPbPb2_211_0.dat',
#       'input/MHlistall.dat', 'input/ptphipoints.dat', 'input/resoweakPDG2016Plus.dat',
#       'input/numbers16p.dat')

def cleanUpFolder(aDir):
    """ Delete all data files in the given directory. """
    if path.exists(aDir):
        try:
            run("rm -rf *", cwd=aDir, echo=False)
        except OSError:
            pass # very likely the the folder is already empty
    else:
        makedirs(aDir)


def checkExistenceOfExecutable(executableFilename):
    """ Check the existence of the executable file, and compile if not. """
    if not path.exists(executableFilename):
        # build then clean
        exec_path, exec_filename = path.split(executableFilename)
        run("make", cwd=exec_path)
        # if still cannot find the executable
        if not path.exists(executableFilename):
            raise ExecutionError(
                         "Cannot generate executable %s!" % executableFilename)

def checkExistenceOfExecutables(executableFilenames):
    """
        Check the existences of the executable files, and compile them if not.
        Will call the checkExistenceOfExecutable function.
    """
    for executableFilename in executableFilenames:
        checkExistenceOfExecutable(executableFilename)


def run(command, cwd=getcwd(), echo=True):
    """ Invoke a command from terminal and wait for it to stop. """
    if echo:
        print("-"*80)
        print("In "+cwd)
        print("Executing command: "+command)
        print("-"*80)
        stdout.flush()
    return call(command, shell=True, cwd=cwd)


def sequentialEventDriverShell():
    """
        Perform a sequential calculations for a given number of events.
        Parameters are read from dictionaries given by allParameterList.
    """
    try:
        # read parameters
        readInParameters()

        # create result folder
        resultDir = controlParameterList['resultDir']
        print('resultDir =', resultDir)
        if path.exists(resultDir):
            rmtree(resultDir)

        makedirs(resultDir)

        # get simulation type
        simulationType = controlParameterList['simulation_type']

        event_id = 0
        
        # generate initial conditions then loop over initial conditions
        initial_condition_list = get_initial_condition_list()
        print('initial_condition_list =', initial_condition_list)
        nev = len(initial_condition_list)


        # printcurrent progress to terminal
        stdout.write("PROGRESS: %d events out of %d finished.\n" 
                     % (event_id, nev))
        stdout.flush()
        #print(initial_condition_list)

        # loop over initial conditions
        for aInitialConditionFile in initial_condition_list:
            event_id += 1
            eventResultDir = path.join(resultDir, 
                controlParameterList['eventResultDirPattern'] % event_id)
            controlParameterList['eventResultDir'] = eventResultDir
            if path.exists(eventResultDir):
                rmtree(eventResultDir)
            makedirs(eventResultDir)
 			
            # print current progress to terminal
            print("Starting event %d..." % event_id)
                        
            #aInitialConditionFile = '/projects/jnorhos/plumberg/EBE-vUSPhydro/EBE-Node/v-USPhydro/inputfiles/settings.inp'
   	
   	    generate_vUSPhydro_input_from_dict()
        
            #copy(aInitialConditionFile, controlParameterList['eventResultDir'])
            print('Associating ' + aInitialConditionFile + ' with event ' + str(event_id))
            
            print(controlParameterList['rootDir'])
            
            hydroResultFiles = [aFile for aFile in hydroWithInitialCondition(aInitialConditionFile)]
            
            print 'hydroResultFiles:', hydroResultFiles
            freezeOutWithHydroResultFiles(hydroResultFiles)
    
            # print current progress to terminal
            stdout.write("PROGRESS: %d events out of %d finished.\n" 
                            % (event_id, nev))
            stdout.flush()
    
        # collect mostly used data into a database
        #collectEbeResultsToDatabaseFrom(resultDir)

    except ExecutionError as e:
        print("Errors encountered during execution, aborting.")
        raise
    finally:
        print("Thank you for using. Christopher Plumberg, 2020-10")


if __name__ == "__main__":
    sequentialEventDriverShell()
