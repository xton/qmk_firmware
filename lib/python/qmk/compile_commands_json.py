#!/usr/bin/env python

import sys
import re
import inspect
from pathlib import Path
import json
import shlex
from itertools import takewhile
from functools import lru_cache
from subprocess import check_output


from typing import TextIO, List, Dict

script_path = Path(inspect.getframeinfo(inspect.currentframe()).filename)
qmk_dir = str(script_path.parent.parent.resolve())


@lru_cache(maxsize=10)
def system_libs(binary: str):
    try:
        return list(Path(check_output(['which', binary]).rstrip().decode()).resolve().parent.parent.glob("*/include"))
    except Exception:
        return []


file_re = re.compile(r"""printf "Compiling: ([^"]+)""")
cmd_re = re.compile(r"""LOG=\$\(([^\)]+)\)""")

def parse_make_n(f: TextIO) -> List[Dict[str,str]]:
    state = 'start'
    this_file = None
    records = []
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
                args = shlex.split(this_cmd)
                args = list(takewhile(lambda x: x != '&&', args))
                args += ['-I%s' % s for s in system_libs(args[0])]
                new_cmd = ' '.join(shlex.quote(s) for s in args if s != '-mno-thumb-interwork')
                records.append({"directory": qmk_dir, "command": new_cmd, "file": this_file})
                state = 'start'

    return records

if __name__ == '__main__':
    with open(sys.argv[1]) as f:
        print(json.dumps(parse_make_n(f), indent=4))
