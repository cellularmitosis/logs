#!/usr/bin/env python

import sys
i = 0
for b in sys.stdin.read():
    if ord(b) > 127:
        sys.stderr.write('binary at byte %i: %s\n' % (i, b))
        sys.exit(1)
    i += 1

