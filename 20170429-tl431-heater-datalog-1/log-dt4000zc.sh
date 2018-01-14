#!/bin/bash

set -e

./log-dt4000zc.py | tee dt4000zc-$( date +%s ).csv
