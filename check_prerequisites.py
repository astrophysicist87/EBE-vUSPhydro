#! /usr/bin/env python
"""
    Print a list of tests to see whether all required tools for Ebe calculations
    are present.
"""

from os import getcwd, unlink, path
from subprocess import call

numberOfSpaces = 5

def printWarning(warningString):
    print("-"*(numberOfSpaces-2) + "> " + warningString)

def printMsg(message):
    print(" "*numberOfSpaces + message)

def checkCommand(cmdString, utilityName=None):
    """
        Try to execute "cmdString", then use "utilityName" to echo messages.
    """
    tempfile = open("response.txt", 'w')
    if not utilityName: utilityName=cmdString
    call("%s " % cmdString, shell=True, cwd=getcwd(), stdout = tempfile, stderr = tempfile)
    tempfile.close()
    if "command not found" in open("response.txt").readline():
        printWarning("%s *NOT* installed." % utilityName)
        unlink("response.txt")
        return False
    else:
        printMsg("%s installed." % utilityName)
        unlink("response.txt")
        return True

def checkModule(moduleName):
    """
        Try to import "moduleName", then echo messages.
    """
    try:
        __import__(moduleName)
        printMsg("python %s module installed." % moduleName)
        return True
    except:
        printWarning("python %s module *NOT* installed." % moduleName)
        return False

def checkEnvironment():
    """
        Check if the required compiler and running environment are complete.
        Return True if the environment is complete, otherwise return False.
    """
    finalMsgs = []

    print("Start checking...")
    print("-"*80)

    # check g++ and icpc
    if not checkCommand("g++") and not checkCommand("icpc"):
        finalMsgs.append("You need to install icpc or g++.")

    # check gfortran and ifort
    if not checkCommand("gfortran") and not checkCommand("ifort"):
        finalMsgs.append("You need to install ifort or gfortran.")

    # check make utility
    if not checkCommand("make"):
        finalMsgs.append("You need to install the make utility.")

    # check gsl
    if not checkCommand("gsl-config", "gsl"):
        finalMsgs.append("You need to install gsl library.")

    # check zip and unzip
    if not checkCommand("zip --help", "zip") or not checkCommand("unzip --help", "unzip"):
        finalMsgs.append("You need both zip and unzip utilities.")

    # check numpy
    if not checkModule("numpy"):
        finalMsgs.append("You need to install python numpy package.")

    # check hdf5
    if not checkCommand("h5fc") or not checkCommand("h5c++"):
        finalMsgs.append("You need to install hdf5 library.")

    # print final messages
    print("-"*80)
    if not finalMsgs:
        print("All essential packages installed. Test passed.")
        return True
    else:
        for msg in finalMsgs: print(msg)
        return False

def checkExecutables():
    """
        Check if all the executables are present, and compile them if not all of
        them are. Return True if all the executables can be successfully
        generated.
    """
    ebeNodeFolder = "EBE-Node"
    executables = (
        path.join("jaki_trento", "bin", "trento"),
        path.join("trento", "bin", "trento"),
        path.join("v-USPhydro", "vusphydro"),
    )

    # check for existence of all executables
    existenceFlag = True
    print("Checking existence of executables.")
    for exe in executables:
        if not path.exists(path.join(ebeNodeFolder, exe)):
            print("Executable %s not found." % exe)
            existenceFlag = False
            break
        else:
            print("Executable %s found." % exe)

    # compile if necessary and check again
    tempfile = open(path.join("utilities", "CompileRecord.txt"), "w")
    if not existenceFlag:
        print("Start building executables...")
        call("./compile_all.sh", shell=True, cwd="utilities", stdout = tempfile, stderr = tempfile)
        tempfile.close()
        unlink(path.join("utilities", "CompileRecord.txt"))

        # check for existence of all executables again
        existenceFlag = True
        print("Checking again existence of executables.")
        for exe in executables:
            if not path.exists(path.join(ebeNodeFolder, exe)):
                print("Executable %s still not found." % exe)
                existenceFlag = False
                return False

    print("All executables found.")
    return True

def greetings(selection):
    if selection==1:
        print(r"""
           
*-------------------------------------------------------------------*
|         __   _   __   __  __       ______                         |
|        / /  / | / /  / / / /      / ____/________  __  ______     |
|   __  / /  /  |/ /  / /_/ /      / / __/ ___/ __ \/ / / / __ \    |
|  / /_/ /  / /|  /  / __  /      / /_/ / /  / /_/ / /_/ / /_/ /    |
|  \____/  /_/ |_/  /_/ /_/       \____/_/   \____/\__,_/ .___/     |
|                                                      /_/          |
*-------------------------------------------------------------------*

        """)
    elif selection==2:
        print("""
*-------------------------------------------------------------------------------------------------------------*
|                                                                                                             |
|        .---.                                                   .-'''-.                                      |
|        |   |                                                  '   _    \                                    |
|        '---'   _..._      .                                 /   /` '.   \       _________   _...._          |
|        .---. .'     '.  .'|                .--./)          .   |     \  '       \        |.'      '-.       |
|        |   |.   .-.   .<  |               /.''\\   .-,.--. |   '      |  '       \        .'```'.    '.     |
|        |   ||  '   '  | | |              | |  | |  |  .-. |\    \     / /         \      |       \     \    |
|        |   ||  |   |  | | | .'''-.        \`-' /   | |  | | `.   ` ..' /_    _     |     |        |    |    |
|        |   ||  |   |  | | |/.'''. \       /("'`    | |  | |    '-...-'`| '  / |    |      \      /    .     |
|        |   ||  |   |  | |  /    | |       \ '---.  | |  '-            .' | .' |    |     |\`'-.-'   .'      |
|        |   ||  |   |  | | |     | |        /'""'.\ | |                /  | /  |    |     | '-....-'`        |
|     __.'   '|  |   |  | | |     | |       ||     ||| |               |   `'.  |   .'     '.                 |
|    |      ' |  |   |  | | '.    | '.      \'. __// |_|               '   .'|  '/'-----------'               |
|    |____.'  '--'   '--' '---'   '---'      `'---'                     `-'  `--'                             |
|                                                                                                             |
*-------------------------------------------------------------------------------------------------------------*
        """)


if __name__ == '__main__':
    checkEnvironment()
