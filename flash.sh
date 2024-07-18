#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
# Flash using mspdebug
echo -e  "Programming using mspdebugger"

mspdebug rf2500 "prog $SCRIPT_DIR/bin/main.out"

