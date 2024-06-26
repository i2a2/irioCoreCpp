#!/bin/python3
import os
import sys
import argparse
import re
import textwrap

serial_regex = r"^(?:0[xX])?[0-9A-Fa-f]{8}$"
allowed_devices = ["7854", "7966", "7965", "7961", "9159"]

patterns = [ r"%%%COMMONSERIAL%%%", r"%%%COMMONDEVICE%%%", 
             r"%%%DAQSERIAL%%%",    r"%%%DAQDEVICE%%%", 
             r"%%%IMAQSERIAL%%%",   r"%%%IMAQDEVICE%%%", 
             r"%%%DIOSERIAL%%%",    r"%%%DIODEVICE%%%", 
            #  r"%%%CRIOSERIAL%%%",   r"%%%CRIODEVICE%%%", 
             r"%%%VERBOSE%%%" ]
tests = [ "Common", "DAQ", "IMAQ", "DIO", 
        #  "CRIO" 
        ] 

def process_serial(serial: str):
    if serial.lower().startswith("0x"):
        return "0x" + serial[2:].upper()
    else:
        return serial.upper()

if __name__ != "__main__":
    print("This file cannot be used as a module", file=sys.stderr)
    exit(1)

parser = argparse.ArgumentParser(
    prog="generateFunctional.py",
    description='Generate functional test suites from templates for irioCore library. If no file provided, it will search for a templates folder on the current folder and create all the tests in the current folder.',
    formatter_class=argparse.RawDescriptionHelpFormatter,
    epilog=textwrap.dedent("""
        Examples:
            python3 generateFunctional.py
            python3 generateFunctional.py -i templates -o out
            python3 generateFunctional.py --common-serial 0x1234ABCD --common-device 7966 
                                          --daq-serial    0x1234ABCD --daq-device    7966 
                                          --imaq-serial   0x1234ABCD --imaq-device   7966 
                                          --dio-serial    0x1234ABCD --dio-device    7965
                                          --verbose 1
        """),
        # --crio-serial   0x1234ABCD --crio-device   9159 
)

fileMgr = parser.add_argument_group("File management")
fileMgr.add_argument('-i', '--input-folder', help='Folder with the templates to generate the tests', type=str, required=False)
fileMgr.add_argument('-o', '--output-folder', help='Folder where the tests should be generated', type=str, required=False)

substVars = parser.add_argument_group("Substitution variables. Either use all of none of them")
substVars.add_argument('--common-serial', help='Serial number of the hardware to use on the common tests. Format 0x1234ABCD', required=False)
substVars.add_argument('--common-device', help='Device identifier of the hardware to use on the common tests. One of [ 7966, 7965, 7961, 9159 ]', required=False)
substVars.add_argument('--daq-serial',    help='Serial number of the hardware to use on the DAQ tests. Format 0x1234ABCD', required=False)
substVars.add_argument('--daq-device',    help='Device identifier of the hardware to use on the DAQ tests. One of [ 7966, 7965, 7961, 9159 ]', required=False)
substVars.add_argument('--imaq-serial',   help='Serial number of the hardware to use on the IMAQ tests. Format 0x1234ABCD', required=False)
substVars.add_argument('--imaq-device',   help='Device identifier of the hardware to use on the IMAQ tests. One of [ 7966, 7965, 7961, 9159 ]', required=False)
substVars.add_argument('--dio-serial',    help='Serial number of the hardware to use on the DIO tests. Format 0x1234ABCD', required=False)
substVars.add_argument('--dio-device',    help='Device identifier of the hardware to use on the DIO tests. One of [ 7966, 7965, 7961, 9159 ]', required=False)
# substVars.add_argument('--crio-serial', help='Serial number of the hardware to use on the CRIO tests. Format 0x1234ABCD', required=False)
# substVars.add_argument('--crio-device', help='Device identifier of the hardware to use on the CRIO tests. One of [ 7966, 7965, 7961, 9159 ]', required=False)
substVars.add_argument('--verbose', help='Verbosity to use on all the tests. Can use True/False, true/false or 1/0', required=False)

args = parser.parse_args()
input_folder = None
output_folder = None

# Parse input folder, if given. Else, check if templates folder exists
if args.input_folder is not None:
    if not os.path.isdir(args.input_folder):
        print(f"Error: {args.input_folder} Does not exist or is not a folder", file=sys.stderr)
        exit(1)
    else:
        input_folder = os.path.abspath(args.input_folder)
else:
    if not os.path.isdir("templates"):
        print("Error: templates folder does not exist", file=sys.stderr)
        exit(1)
    else:
        input_folder = os.path.abspath("./templates")

# Parse output folder, if given. Else, use current directory
if args.input_folder is not None:
    if not os.path.isdir(args.output_folder):
        os.mkdir(args.output_folder)
    output_folder = os.path.abspath(args.output_folder)
else:
    output_folder = os.getcwd()

# Parse automation variables
automation_vars = [ args.common_serial, args.common_device, 
                    args.daq_serial,    args.daq_device, 
                    args.imaq_serial,   args.imaq_device, 
                    args.dio_serial,    args.dio_device, 
                #    args.crio_serial, args.crio_device, 
                    args.verbose, ]

# Create substitution dict
subst_dict = {}
if any(automation_vars):
    if not all(automation_vars):
        print("Error: All variables must be defined for automatic template substitution", file=sys.stderr)
        exit(-1)

    for (i, t) in enumerate(tests):
        serial = automation_vars[2*i]
        dev    = automation_vars[2*i + 1]

        if re.match(serial_regex, serial) is None:
            print(f"Error: Invalid {t} serial", file=sys.stderr)
            exit(-1)

        if dev not in allowed_devices:
            print(f"Error: Invalid {t} device", file=sys.stderr)
            exit(-1)

        automation_vars[2*i] = process_serial(serial)

    if automation_vars[-1].lower() not in ["y", "yes", "true", "1", "n", "false", "0", "no"]:
        print("Error: Invalid verbose value", file=sys.stderr)
        exit(-1)

    automation_vars[-1] = "true" if automation_vars[-1] in ["y", "yes", "true", "1"] else "false"


    subst_dict = { k: v for (k, v) in zip(patterns, automation_vars) }
else:
    for (i, t) in enumerate(tests):
        dev = ""
        while dev not in allowed_devices:
            dev = input(f"Enter the device identifier for the {t} tests [ 7966, 7965, 7961, 9159 ]: ")

        serial = ""
        while re.match(serial_regex, serial) is None:
            serial = input(f"Enter the serial number for the {t} tests (Format 0x1234ABCD):") 

        subst_dict[patterns[2*i]] = process_serial(serial)
        subst_dict[patterns[2*i + 1]] = dev
        
    subst_dict[patterns[-1]] = "true" if input("Should the tests be verbose? [y/N]:").lower() == "y" else "false"

for file in os.listdir(input_folder):
    content = ""

    with open(os.path.join(input_folder, file), "r") as f:
        content = f.read()

    for (k, v) in subst_dict.items():
        content = re.sub(k, v, content)

    with open(os.path.join(output_folder, file), "w") as of:
         of.write(content)