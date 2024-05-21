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

def runCommand(binary, filterText, RIODevice, RIOSerial, Verbose, Coupling, MaxCounter, Summary=False, suiteName=None, shuffle=False, iterations='1', verboseTest=False, verboseInit=False):
    cwd = os.getcwd()
    os.chdir(os.path.dirname(binary))

    if "UART" in filterText:
        print("\033[91m[TEST] Warning: The UART tests need human interaction \033[00m")
        Summary = False

    gshuffle = " --gtest_shuffle" if shuffle else ""
    giterations = (" --gtest_repeat=" + iterations) if args.iterations != '1' else ""
    
    failed = None
    passed = None
    command = f"\
env -S RIOSerial={RIOSerial} \
env -S RIODevice={RIODevice} \
env -S VerboseInit={1 if Verbose=='true' or verboseInit else 0} \
env -S VerboseTest={1 if Verbose=='true' or verboseTest else 0} \
env -S Coupling={Coupling} \
env -S maxCounter={MaxCounter} \
    ./{os.path.basename(binary)} --gtest_filter={filterText} {giterations} {gshuffle}" 

    if suiteName is not None:
        print('\n' + (' ' + suiteName + ' ').center(80, '='))

    process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, shell=True, bufsize=1)

    fail_matched = False
    failed_tests = []

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
            fail_matched = True

        passmatch = re.findall(r"\[\s+PASSED\s+\] (\d+) test.*", line)
        if len(passmatch) > 0:
            passed = int(passmatch[0])

        if fail_matched:
            failed_tests.append(line)

    process.kill()
    if passed is None:
        passed = 0
    if failed is None:
        failed = 0

    filtered_tests = []
    for s in failed_tests:
        testMatch = re.match(r"\[  FAILED  \] ([^\d].*)", s)
        if testMatch is not None:
            filtered_tests.append(testMatch.groups()[0])

    os.chdir(cwd)
    return (passed, failed + passed, failed == 0, filtered_tests)

def writeCmdXML(binary, filter, device, serial, passed_cnt, total_cnt, passed_bool, failed_tests, output_file):
    doc = minidom.Document()

    root = doc.createElementNS("http://www.testLocation.com/test", "testplan")
    root.setAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance")
    root.setAttribute("xsi:schemaLocation", "http://www.testLocation.com/test ../testSchema.xsd")
    doc.appendChild(root)

    test_element = doc.createElement("test")

    name_element = doc.createElement("name")
    name_text = doc.createTextNode("Command-line test")
    name_element.appendChild(name_text)
    test_element.appendChild(name_element)

    test_type_element = doc.createElement("TestType")
    test_type_text = doc.createTextNode("Custom binary")
    test_type_element.appendChild(test_type_text)
    test_element.appendChild(test_type_element)

    binary_element = doc.createElement("Binary")
    binary_text = doc.createTextNode(os.path.abspath(binary))
    binary_element.appendChild(binary_text)
    test_element.appendChild(binary_element)

    test_filter_element = doc.createElement("TestFilter")
    test_filter_text = doc.createTextNode(filter)
    test_filter_element.appendChild(test_filter_text)
    test_element.appendChild(test_filter_element)

    rio_device_element = doc.createElement("RIODevice")
    rio_device_text = doc.createTextNode(device)
    rio_device_element.appendChild(rio_device_text)
    test_element.appendChild(rio_device_element)

    rio_serial_element = doc.createElement("RIOSerial")
    rio_serial_text = doc.createTextNode(serial)
    rio_serial_element.appendChild(rio_serial_text)
    test_element.appendChild(rio_serial_element)

    resultElement = doc.createElement("results")
    resultElement.appendChild(doc.createTextNode(f"{passed_cnt}/{total_cnt}"))
    test_element.appendChild(resultElement)

    summaryElement = doc.createElement("summary")
    summaryElement.appendChild(doc.createTextNode("PASS" if passed_bool else "FAIL"))
    test_element.appendChild(summaryElement)

    if not passed_bool:
        failedTestsElement = doc.createElement("failedTests")
        for failed_test in failed_tests:
            failedTestElement = doc.createElement("testName")
            failedTestElement.appendChild(doc.createTextNode(failed_test))
            failedTestsElement.appendChild(failedTestElement)
        test_element.appendChild(failedTestsElement)

    root.appendChild(test_element)

    with open(output_file, 'wt') as fd:
        fd.write("".join([s for s in doc.toprettyxml().strip().splitlines(True) if s.strip("\r\n").strip()]) + '\n')

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
parser.add_argument('-i', '--input-file', help='XML file that contains the test plan')
output_group = parser.add_mutually_exclusive_group(required=False)
output_group.add_argument('-a', '--append-file', help='Append the results to the input file instead of producing a new one. Cannot be used with -o/--output-file', action='store_true')
output_group.add_argument('-o', '--output-file', help='XML file that contains the results of the plan. Cannot be used with -a/--append-file')
parser.add_argument('-S', '--summary',help='Summarize the execution', action='store_true')

# Parameters for command-line based test plans
parser.add_argument('-d', '--RIODevice',help='RIO device model. Use lsrio or lsni command to display it.',choices=['7961','7965','7966','7975','9159'], default='7966')
parser.add_argument('-s', '--RIOSerial',help='RIO device serial number. Use lsrio or lsni command to display it')
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

    if args.list:
        command = f"./{args.binary} --gtest_list_tests"
        exit(0)

    if args.RIOSerial is not None:
        serial = args.RIOSerial
    else:
        print("Serial number is required for command-line based test plans", file=sys.stderr)
        exit(-1)

    if args.RIODevice is not None:
        device = args.RIODevice
    else:
        print("Device model is required for command-line based test plans", file=sys.stderr)
        exit(-1)

    filterText = args.filter if args.filter is not None else "*"

    (passed_cnt, total_cnt, passed_bool, failed_tests) = runCommand(
        binary      = args.binary,
        filterText  = filterText,
        RIODevice   = device,
        RIOSerial   = serial,
        Verbose     = args.verbose,
        Coupling    = args.coupling,
        MaxCounter  = args.max_counter,
        Summary     = args.summary,
        suiteName   = None,
        shuffle     = args.shuffle,
        iterations  = args.iterations,
        verboseTest = args.verbose_test,
        verboseInit = args.verbose_init
    )

    if args.summary:
        pass_word = "\033[32mPASS\033[00m"
        fail_word = "\033[91mFAIL\033[00m"
        print(f"{pass_word if passed_bool else fail_word} - Passed {passed_cnt}/{total_cnt}")
        if (not passed_bool):
            print("Failed tests:")
            for failed_test in failed_tests:
                print(f"\t{failed_test}")

    if args.output_file is not None:
        writeCmdXML(
            args.binary, filterText, device, serial, passed_cnt, total_cnt, passed_bool, failed_tests, args.output_file
        )


else:
    # File-based test plan
    if not os.path.exists(args.input_file):
        print(f"Could not find the file {args.input_file}")
        exit(-1)

    summary = True if args.summary else False

    tree = minidom.parse(args.input_file)
    targetFile = os.path.abspath(args.output_file if not args.append_file else args.input_file)

    os.chdir(os.path.dirname(os.path.abspath(sys.argv[0])))

    for test in tree.getElementsByTagName('test'):
        try:
            testName   = test.getElementsByTagName('name')[0].firstChild.data
            filterText = test.getElementsByTagName('TestFilter')[0].firstChild.data
            RIODevice  = test.getElementsByTagName('RIODevice')[0].firstChild.data
            RIOSerial  = test.getElementsByTagName('RIOSerial')[0].firstChild.data
            binary = binaryPaths.get(test.getElementsByTagName('TestType')[0].firstChild.data, test.getElementsByTagName('Binary')[0].firstChild.data)
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
        
        (passed_cnt, total_cnt, passed_bool, failed_tests) = runCommand(
            binary      = binary,
            filterText  = filterText,
            RIODevice   = RIODevice,
            RIOSerial   = RIOSerial,
            Verbose     = verbose,
            Coupling    = coupling,
            MaxCounter  = maxIterations,
            Summary     = summary,
            suiteName   = testName,
        )

        if summary:
            pass_word = "\033[32mPASS\033[00m"
            fail_word = "\033[91mFAIL\033[00m"
            print(f"{pass_word if passed_bool else fail_word} - Passed {passed_cnt}/{total_cnt}")
            if (not passed_bool):
                print("Failed tests:")
                for failed_test in failed_tests:
                    print(f"\t{failed_test}")

        resultElement = test.getElementsByTagName("results")
        if resultElement:
            resultElement[0].firstChild.data = f"{passed_cnt}/{total_cnt}";
        else:
            resultElement = tree.createElement("results")
            resultElement.appendChild(tree.createTextNode(f"{passed_cnt}/{total_cnt}"))
            test.appendChild(resultElement)

        summaryElement = test.getElementsByTagName("summary")
        if summaryElement:
            summaryElement[0].firstChild.data = "PASS" if passed_bool else "FAIL"
        else:
            summaryElement = tree.createElement("summary")
            summaryElement.appendChild(tree.createTextNode("PASS" if passed_bool else "FAIL"))
            test.appendChild(summaryElement)

        failedTestsElement = test.getElementsByTagName("failedTests")
        if failedTestsElement:
            failedTestsElement[0].parentNode.removeChild(failedTestsElement[0])

        if not passed_bool:
            failedTestsElement = tree.createElement("failedTests")
            for failed_test in failed_tests:
                failedTestElement = tree.createElement("testName")
                failedTestElement.appendChild(tree.createTextNode(failed_test))
                failedTestsElement.appendChild(failedTestElement)
            test.appendChild(failedTestsElement)

    with open(targetFile, 'wt') as fd:
        fd.write("".join([s for s in tree.toprettyxml().strip().splitlines(True) if s.strip("\r\n").strip()]) + '\n')