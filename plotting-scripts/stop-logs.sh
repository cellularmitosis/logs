#!/bin/bash

set -e -o pipefail

test -n "${1}"
cat "${1}"/pids | xargs -n1 kill
