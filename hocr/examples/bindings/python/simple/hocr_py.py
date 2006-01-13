#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys

# use the hocr python module
from hocr import *

# get pnm picture file path from user
if len(sys.argv) > 1:
    
    # create a new Hocr object from pnm picture file
    hocr_engien = Hocr (sys.argv[1])
    
    # turn nikud off
    hocr_engien.set_opt_n (0)
    
    # print out text
    print hocr_engien.do_ocr ()
else:
    print "USAGE: hocr_py path_to_pnm_picture"
