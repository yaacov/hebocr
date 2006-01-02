#!/usr/bin/python
# -*- coding: utf-8 -*-

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

# Copyright (C) 2006 Yaacov Zamir

import gtk
import gtk.glade
from hocr import *

# create widget tree ...
xml = gtk.glade.XML('pygtk-hocr.glade')

image = xml.get_widget('image')
text = xml.get_widget('textview')
buffer = text.get_buffer()
preview = gtk.Image()

def update_preview_cb(file_chooser, preview):
    filename = file_chooser.get_preview_filename()
    try:
      pixbuf = gtk.gdk.pixbuf_new_from_file_at_size(filename, 128, 128)
      preview.set_from_pixbuf(pixbuf)
      have_preview = True
    except:
      have_preview = False
    file_chooser.set_preview_widget_active(have_preview)
    return

def on_window_delete_event(widget, obj):
    "Quit the application"
    gtk.main_quit()

def on_button_open_clicked(obj):
    chooser = gtk.FileChooserDialog("Open..",
                               None,
                               gtk.FILE_CHOOSER_ACTION_OPEN,
                               (gtk.STOCK_CANCEL, gtk.RESPONSE_CANCEL,
                                gtk.STOCK_OPEN, gtk.RESPONSE_OK))
    chooser.set_preview_widget(preview)
    chooser.connect("update-preview", update_preview_cb, preview)
    
    response = chooser.run()
    if response == gtk.RESPONSE_OK:
        filename = chooser.get_filename()
        pixbuf = gtk.gdk.pixbuf_new_from_file (filename)
        image.set_from_pixbuf(pixbuf)
        
    elif response == gtk.RESPONSE_CANCEL:
        print 'Closed, no files selected'
    
    chooser.destroy()

def on_button_apply_clicked(obj):
    hocr_engine = Hocr ()
    pixbuf = image.get_pixbuf()
    
    hocr_engine.set_height (pixbuf.get_height ());
    hocr_engine.set_width (pixbuf.get_width ());
    hocr_engine.set_rowstride (pixbuf.get_rowstride ());
    hocr_engine.set_n_channels (pixbuf.get_n_channels ());
    hocr_engine.set_brightness (100);
    
    hocr_engine.copy_pixels (pixbuf.get_pixels(), 
        pixbuf.get_height () * pixbuf.get_rowstride ());
    
    buffer.set_text(hocr_engine.do_ocr ())

def on_button_save_clicked(obj):
    chooser = gtk.FileChooserDialog("Save..",
                               None,
                               gtk.FILE_CHOOSER_ACTION_SAVE,
                               (gtk.STOCK_CANCEL, gtk.RESPONSE_CANCEL,
                                gtk.STOCK_SAVE, gtk.RESPONSE_OK))
    
    response = chooser.run()
    if response == gtk.RESPONSE_OK:
        filename = chooser.get_filename()
        file = open(filename, 'w')
        file.write (buffer.get_text(buffer.get_start_iter(), buffer.get_end_iter()))
        file.close()
    elif response == gtk.RESPONSE_CANCEL:
        print 'Closed, no files selected'
    
    chooser.destroy()

# connect handlers
xml.signal_autoconnect(locals())

gtk.main()
