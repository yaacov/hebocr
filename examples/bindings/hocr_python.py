#!/usr/bin/env python
# -*- coding: utf-8 -*-

# 
# hocr_python.py
#
# example file for libhocr.
# use: python hocr_python.py IMAGE_FILENAME
#
# Copyright (C) 2008  Yaacov Zamir <kzamir@walla.co.il>
#

#  
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

# for user arg
import sys

from hocr import *

# load the picture pointed to by argv[1]
pix = ho_pixbuf_pnm_load(sys.argv[1])

# create a new hocr object
hocr_obj = Hocr(pix)

# do ocr
hocr_obj.do_ocr()

# print text
print hocr_obj.get_text()
