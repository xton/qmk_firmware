#!/usr/bin/env python

import sys
import re
from pathlib import Path
import json
import shlex
from functools import lru_cache
from subprocess import check_output

from qmk.constants import QMK_FIRMWARE

from typing import TextIO, List, Dict


@lru_cache(maxsize=10)
def system_libs(binary: str):
    """Find the system include directory that the given build tool uses.

    Only tested on OSX+homebrew so far.
    """

    try:
        return list(Path(check_output(['which', binary]).rstrip().decode()).resolve().parent.parent.glob("*/include"))
    except Exception:
        return []


file_re = re.compile(r"""printf "Compiling: ([^"]+)""")
cmd_re = re.compile(r"""LOG=\$\((.+?)\&\&""")


def parse_make_n(f: TextIO) -> List[Dict[str, str]]:
    """parse the output of `make -n <target>`

    This function makes many assumptions about the format of your build log.
    This happens to work right now for qmk.
    """

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
                args += ['-I%s' % s for s in system_libs(args[0])]
                new_cmd = ' '.join(shlex.quote(s) for s in args if s != '-mno-thumb-interwork')
                records.append({"directory": str(QMK_FIRMWARE.resolve()), "command": new_cmd, "file": this_file})
                state = 'start'

    return records


if __name__ == '__main__':
    with open(sys.argv[1]) as f:
        print(json.dumps(parse_make_n(f), indent=4))
