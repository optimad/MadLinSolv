#/bin/python

#TODO: modify to match MadLinSolv test

import argparse
import re
import os
import shlex
import sys

from collections import OrderedDict
from subprocess import check_output

# Set arguments
parser = argparse.ArgumentParser(description='Test driver.')
parser.add_argument('--command', dest='command', type=str, required=True,
                    help='the command that will be run')
parser.add_argument('--expected', dest='expected', type=str, required=True,
                    help='the expected results')

# Parse arguents
args = parser.parse_args()

# Run the test
output = check_output(shlex.split(args.command),env=os.environ)

print
print " -------------- TEST OUTPUT --------------"
print output
print " -----------------------------------------"

# Get expected values
expected = args.expected.split(";")

expected_iteration = expected[0]

expected_results = OrderedDict()
expected_results['time']          = expected[1]
expected_results['residual_L2']   = expected[2]
expected_results['residual_Linf'] = expected[3]

# Parse the output
time          = None
residual_L2   = None
residual_Linf = None

for line in iter(output.splitlines()):
    line = re.sub(r'^[ ]*#[0-9]*\ ::', '', line).strip()
    if line.startswith("iter"):
        residuals = line.split("|")


        iteretion = residuals[0].replace("iter", "").strip()
        if iteretion == expected_iteration:
            results = OrderedDict()
            results['time']          = residuals[1].replace("time", "").strip()
            results['residual_L2']   = residuals[2].replace("R L2", "").strip()
            results['residual_Linf'] = residuals[3].replace("R Linf", "").strip().split("in", 1)[0].strip()
            break

# Check the values
print
print " -------------- TEST RESULT --------------"

status = 0
for key in results.keys():
    value          = results[key]
    expected_value = expected_results[key]

    print " Checking '%s' variable:" % (key)
    print "    Value          : ", value
    print "    Expected value : ", expected_value
    if value == expected_value:
        print "    Check status   : PASSED"
    else:
        print "    Check status   : FAILED"
        status = 1

print " -----------------------------------------"

if status == 0:
    print "            TEST PASSED"
else:
    print "            TEST FAILED"

print " -----------------------------------------"

sys.exit(status)
