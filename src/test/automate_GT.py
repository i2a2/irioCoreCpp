#!/bin/python3
import argparse
import subprocess
import os 
import re
import sys
import textwrap
from xml.dom import minidom

if __name__ != "__main__":
    print("This file cannot be used as a module", file=sys.stderr)
    exit(1)

binaryPaths = {
    "irioCore Unitary": "c++/unittests/irioCore/test_ut_irioCore",
    "irioCore Functional": "c++/irioCore/test_irioCore",
    "irioCoreCpp Unitary": "c++/unittests/irioCoreCpp/test_ut_irioCoreCpp",
    "irioCoreCpp Functional": "c++/irioCoreCpp/test_irioCoreCpp",
    "BFP": "c++/bfp/test_bfp",
}

def searchSerial(device, order):
    if device not in ['7961','7965','7966','7975','9159']:
        raise Exception("Invalid device type")
    
    results = re.findall(f'--Model Name:[^\\n]*{device}[^\\n]*\\n\\s*--Serial Number:\\s*(\\S+)\\n', subprocess.check_output(['lsrio']).decode())

    if len(results) == 0:
        raise Exception("No devices found")
    else:
        return results[max(min(int(order) - 1, len(results) - 1), 0)]

def runCommand(binary, filterText, RIODevice, RIOSerial, Verbose, Coupling, MaxCounter, Summary=False):
    cwd = os.getcwd()
    os.chdir(os.path.dirname(binary))

    if "UART" in filterText:
        print("\033[91m[TEST] Warning: The UART tests need human interaction \033[00m")
        Summary = False

    failed = None
    passed = None
    command = f"\
env -S RIOSerial={RIOSerial} \
env -S RIODevice={RIODevice} \
env -S VerboseInit={1 if Verbose=='true' else 0} \
env -S VerboseTest={1 if Verbose=='true' else 0} \
env -S Coupling={Coupling} \
env -S maxCounter={MaxCounter} \
    ./{os.path.basename(binary)} --gtest_filter={filterText}" 

    process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, shell=True, bufsize=1)
    while True:
        line = process.stdout.readline()

        if not line:
           break

        line = line.rstrip().decode('utf-8')

        if not Summary:
            print(line.rstrip())

        failmatch = re.findall(r"\[\s+FAILED\s+\] (\d+) test.*", line)
        if len(failmatch) > 0:
            failed = int(failmatch[0])

        passmatch = re.findall(r"\[\s+PASSED\s+\] (\d+) test.*", line)
        if len(passmatch) > 0:
            passed = int(passmatch[0])

    process.kill()
    if passed is None:
        passed = 0
    if failed is None:
        failed = 0

    os.chdir(cwd)
    return (passed, failed + passed, failed == 0)


# Parse arguments
parser = argparse.ArgumentParser(
    prog="run_irioCore.py",
    description='Execute funcional tests of the C irioCore library',
    formatter_class=argparse.RawDescriptionHelpFormatter,
    epilog=textwrap.dedent("""
        Examples:
            python3 run_irioCore.py -i test_plan.xml -o results.xml
            python3 run_irioCore.py -d 7966 -s 0x123ABC -b c++/irioCore/test_irioCore -v -f "FlexRIO*"
        """),
)

# Parameters for file-based test plans
parser.add_argument('-i', '--input-file', help='XML file that contains the test plan', nargs='?')
parser.add_argument('-o', '--output-file', help='XML file that contains the results of the plan. If a value is not provided, the same file is used', nargs='?')
parser.add_argument('-S', '--summary',help='Summarize the execution', action='store_true')

# Parameters for command-line based test plans
parser.add_argument('-d', '--RIODevice',help='RIO device model. Use lsrio command to display it. If no serial number is provided, the first device is selected',choices=['7961','7965','7966','7975','9159'], default='7966')
parser.add_argument('-s', '--RIOSerial',help='RIO device serial number. Use $lsrio command to display it')
parser.add_argument('--device-number',help='If no RIOSerial is provided and there are multiple devices, select which one to use. Bounded between 0 and the number of devices',default='1')
parser.add_argument('--iterations',help='Select number of iterations of the tests. Pass a negative number to run indefinitely.',default='1')
parser.add_argument('--shuffle',help='Shuffle the test execution', action='store_true')
parser.add_argument('-c', '--coupling',help='Coupling mode for mod5761 test. 1 = DC, 0 = AC',choices=['1','0','DC','AC'],default='0')
parser.add_argument('-v', '--verbose',help='Print all traces', action='store_true')
parser.add_argument('--verbose-init',help='Print driver startup trace', action='store_true')
parser.add_argument('--verbose-test',help='Print test traces', action='store_true')
parser.add_argument('-f', '--filter', help='Filter the text execution', )
parser.add_argument('-l', '--list', help='List all the tests', action='store_true')
parser.add_argument('--max-counter',help='Max counter for the IMAQ tests',default='65536')
parser.add_argument('-b', '--binary', help='Select the test binary to run', required=False)

args = parser.parse_args()

if len(sys.argv)==1:
    parser.print_usage()
    sys.exit(1)

if args.input_file is None:

    if args.binary is None:
        print("Binary is required for command-line based test plans", file=sys.stderr)
        exit(-1)
    if not os.path.exists(args.binary):
        print(f"Could not find binary {args.binary}")
        exit(-1)

    os.chdir(os.path.dirname(args.binary))
    if args.list:
        command = f"./{os.path.basename(args.binary)} --gtest_list_tests"
    else:
        if args.RIOSerial is not None:
            serial = args.RIOSerial
        else:
            serial = searchSerial(args.RIODevice, args.device_number)

        gfilter = (" --gtest_filter=" + args.filter) if args.filter is not None else ""
        gshuffle = " --gtest_shuffle" if args.shuffle else ""
        giterations = (" --gtest-repeat=" + args.iterations) if args.iterations != '1' else ""
        summary = r' 2>&1 | grep -A9999 "Global test environment tear-down"' if args.summary else ""

        command = f"\
    env -S RIOSerial={serial} \
    env -S RIODevice={args.RIODevice} \
    env -S VerboseInit={int(args.verbose_init or args.verbose)} \
    env -S VerboseTest={int(args.verbose_test or args.verbose)} \
    env -S Coupling={args.coupling} \
    env -S maxCounter={args.max_counter} \
    ./{os.path.basename(args.binary)}{gfilter}{giterations}{gshuffle}{summary}" 

    if args.verbose or args.verbose_test:
        print(f"Running command: {command}")

    os.system(command)
else:
    # File-based test plan
    if not os.path.exists(args.input_file):
        print(f"Could not find the file {args.input_file}")
        exit(-1)

    summary = True if args.summary else False

    tree = minidom.parse(args.input_file)
    targetFile = os.path.abspath(args.output_file if args.output_file is not None else args.input_file)

    os.chdir(os.path.dirname(os.path.abspath(sys.argv[0])))

    for test in tree.getElementsByTagName('test'):
        try:
            testName   = test.getElementsByTagName('name')[0].firstChild.data
            filterText = test.getElementsByTagName('TestFilter')[0].firstChild.data
            RIODevice  = test.getElementsByTagName('RIODevice')[0].firstChild.data
            RIOSerial  = test.getElementsByTagName('RIOSerial')[0].firstChild.data
            binary = binaryPaths.get(test.getElementsByTagName('TestType')[0].firstChild.data)
        except:
            print("Malformatted test in XML, omitting it")
            continue

        if binary is None:
            print("Incorrect test type, omitting test")
            continue

        try: # Optional argument
            verbose = test.getElementsByTagName('verbose')[0].firstChild.data
        except:
            verbose = False 

        try: # Optional argument
            coupling = test.getElementsByTagName('DC')[0].firstChild.data
        except:
            coupling = 'DC' 

        try: # Optional argument
            maxIterations = test.getElementsByTagName('maxIMAQIterations')[0].firstChild.data
        except:
            maxIterations = '65536' 

        if filterText is None or RIODevice is None or RIOSerial is None or verbose is None:
            print("Malformatted test in XML, omitting it")
            continue
        
        if summary: 
            print(f"Running test \"{testName}\"")

        result = runCommand(binary, filterText, RIODevice, RIOSerial, verbose, coupling, maxIterations, summary)

        if summary:
            pass_word = "\033[32mPASS\033[00m"
            fail_word = "\033[91mFAIL\033[00m"
            print(f"{pass_word if result[2] else fail_word} - {result[0]}/{result[1]}")

        resultElement = test.getElementsByTagName("results")
        if resultElement:
            resultElement[0].firstChild.data = f"{result[0]}/{result[1]}";
        else:
            resultElement = tree.createElement("results")
            resultElement.appendChild(tree.createTextNode(f"{result[0]}/{result[1]}"))
            test.appendChild(resultElement)

        summaryElement = test.getElementsByTagName("summary")
        if summaryElement:
            summaryElement[0].firstChild.data = "PASS" if result[2] else "FAIL"
        else:
            summaryElement = tree.createElement("summary")
            summaryElement.appendChild(tree.createTextNode("PASS" if result[2] else "FAIL"))
            test.appendChild(summaryElement)

    with open(targetFile, 'wt') as fd:
        fd.write("".join([s for s in tree.toprettyxml().strip().splitlines(True) if s.strip("\r\n").strip()]))