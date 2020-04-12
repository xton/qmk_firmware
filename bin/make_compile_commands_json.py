#!/usr/bin/env python

import sys
import re
import inspect
from pathlib import Path
import json

script_path = Path(inspect.getframeinfo(inspect.currentframe()).filename)
qmk_dir = str(script_path.parent.parent.resolve())


# false || printf "Compiling: quantum/quantum.c" | awk '{ printf "%-99s", $0; }'

file_re = re.compile(r"""printf "Compiling: ([^"]+)""")
cmd_re = re.compile(r"""LOG=\$\(([^\)]+)\)""")

state = 'start'
this_file = None
records = []
with open(sys.argv[1]) as f:
    for line in f:
        if state == 'start':
            m = file_re.search(line)
            if m:
                this_file = m.group(1)
                state = 'cmd'

        if state == 'cmd':
            m = cmd_re.search(line)
            if m:
                # we have a hit!
                this_cmd = m.group(1)
                records.append({"directory": qmk_dir, "command": this_cmd, "file": this_file})
                state = 'start'

print(json.dumps(records, indent=4))

