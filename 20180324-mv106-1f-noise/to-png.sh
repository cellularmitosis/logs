#!/bin/bash

set -e

mogrify -format png *.bmp
rm *.bmp
