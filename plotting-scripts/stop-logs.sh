#!/bin/bash

set -e -o pipefail

cat "${1}"/pids | xargs -n1 kill
