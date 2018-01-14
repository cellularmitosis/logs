#!/bin/bash

set -e

wget https://bitbucket.org/kuzavas/dmm_es51922/raw/7ebf1f82e3d984ea74362534663699edd7aec883/dmm_es51922.py
mv dmm_es51922.py orig.py
echo '#!/usr/bin/env python' > shebang.py
cat shebang.py orig.py > dmm_es51922.py
rm shebang.py orig.py
chmod +x dmm_es51922.py
