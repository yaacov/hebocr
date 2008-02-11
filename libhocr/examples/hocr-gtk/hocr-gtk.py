#!/usr/bin/env python
# -*- coding: utf-8 -*-

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# Generated with glade2py script
# glade2py script can be found at hocr web site http://hocr.berlios.de

# Copyright (C) 2008 Yaacov Zamir <kzamir_a_walla.co.il>

import sys, os
import threading
import time

import pygtk
pygtk.require('2.0')

import gtk, gtk.glade

from hocr import *

# set gettext support
app_name = "hocr-gtk"
locale_dir = '/usr/share/locale'
try:
    import locale
    import gettext
    locale.setlocale(locale.LC_ALL, "")
    gtk.glade.bindtextdomain(app_name, locale_dir)
    gettext.install(app_name, locale_dir, unicode=1)
except (IOError,locale.Error), e:
    print "WARNING: Can't load locale"
    _ = lambda x : x

# set global parmeters
glade_file = 'hocr-gtk.glade'
glade_dir = '/usr/share/hocr-gtk/glade/'

# program info
app_version = "0.1.0"
author_name = "Yaacov Zamir"
author_email = "<kzamir@walla.co.il>"
copyright = author_name + " " + author_email
comments = _("Hocr-GTK, Hebrew optical character recognition\ngraphical front end (GTK)\n\n")
comments += hocr_get_build_string()

# set global functions
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

def show_error_message(message):
	dlg = gtk.MessageDialog(type=gtk.MESSAGE_ERROR
				,message_format = message
				,buttons=gtk.BUTTONS_OK)
	dlg.run()
	dlg.destroy()

class ProgressSet(threading.Thread):
        "set the fraction of the progressbar"
        
        # thread event, stops the thread if it is set.
        stopthread = threading.Event()
        
        def run(self):
                "run while thread is alive."
                
                # importing the progressbar from the global scope
                global progressbar 
                global hocr_obj
                
                self.stopthread.clear()
                
                # main loop
                while not self.stopthread.isSet() :
                        # acquiring the gtk global mutex
                        gtk.gdk.threads_enter()
                        # set the fraction
                        progressbar.set_fraction(1.0 * hocr_obj.progress / 100.0)
                        # releasing the gtk global mutex
                        gtk.gdk.threads_leave()
                        
                        # delaying 
                        time.sleep(0.1)
                        
        def stop(self):
                "stop main loop"
                progressbar.set_fraction(1.0 * hocr_obj.progress / 100.0)
                self.stopthread.set()

class RunOCR(threading.Thread):
        def run(self):
                # importing the ocr object from the global scope
                global hocr_obj
                global menuitem_clear
                global textbuffer
                global textview
                
                ps = ProgressSet()
                
                # do ocr
                ps.start()
                hocr_obj.do_ocr()
                ps.stop()
                
                # set text
                if hocr_obj.get_text():
                    if menuitem_clear.get_active():
                        textbuffer.set_text(hocr_obj.get_text())
                    else:
                        textbuffer.insert_at_cursor(hocr_obj.get_text())
                textview.grab_focus()

# set main window class
class MainWindow:
    def __init__(self):
        global progressbar 
        global hocr_obj
        global menuitem_clear
        global textbuffer
        global textview
        
        # create widget tree ...
        xml = False
        if os.path.isfile(glade_file):
            xml = gtk.glade.XML(glade_file, 'window_main', app_name)
        else:
            if os.path.isfile(glade_dir + glade_file):
               xml = gtk.glade.XML(glade_dir + glade_file, 'window_main', app_name)
        
        if not xml:
            print "ERROR: Can't load glade GUI file, check your install."
            show_error_message(_("Can't load hocr's glade GUI file, check your install.\nExit program."))
            sys.exit(0)
        
        # connect handlers
        xml.signal_autoconnect(self)

        # widgets
        self.window_main = xml.get_widget('window_main')
        self.image = xml.get_widget('image')
        self.textview = xml.get_widget('textview')
        self.progressbar = xml.get_widget('progressbar')
        self.statusbar1 = xml.get_widget('statusbar1')
        progressbar = self.progressbar
        textview = self.textview
        
        # text 
        self.textbuffer =  self.textview.get_buffer()
        self.clipboard = gtk.Clipboard()
        self.textview.grab_focus()
        textbuffer = self.textbuffer
        
        # image
        self.preview = gtk.Image()
        self.filename = ""
        self.pixbuf = None
        self.zoom_factor = 1.0
        
        # edit
        self.menuitem_clear = xml.get_widget('menuitem_clear')
        self.menuitem_html = xml.get_widget('menuitem_html')
        self.menuitem_nikud = xml.get_widget('menuitem_nikud')
        self.menuitem_column_auto = xml.get_widget('menuitem_column_auto')
        self.menuitem_column_one = xml.get_widget('menuitem_column_one')
        menuitem_clear = self.menuitem_clear
        
        # ocr
        self.hocr_obj = Hocr()
        
        hocr_obj = self.hocr_obj
    
    # signal handlers
    def on_window_main_delete_event(self, widget, obj):
        "on_window_main_delete_event activated"
        gtk.main_quit()

    def on_imagemenuitem_new_activate(self, obj, event = None):
        "on_imagemenuitem_new_activate activated"
        chooser = gtk.FileChooserDialog(_("Open.."),
            None,
            gtk.FILE_CHOOSER_ACTION_OPEN,
            (gtk.STOCK_CANCEL, gtk.RESPONSE_CANCEL,
            gtk.STOCK_OPEN, gtk.RESPONSE_OK))
        chooser.set_preview_widget(self.preview)
        chooser.connect("update-preview", update_preview_cb, self.preview)
        
        response = chooser.run()
        if response == gtk.RESPONSE_OK:
            self.filename = chooser.get_filename()
            self.pixbuf = gtk.gdk.pixbuf_new_from_file (self.filename)
            
            factor = self.zoom_factor
            w = self.pixbuf.get_width()
            h = self.pixbuf.get_height()
            window_pixbuf = self.pixbuf.scale_simple(int(w * factor), int(h * factor), gtk.gdk.INTERP_NEAREST)
            self.image.set_from_pixbuf(window_pixbuf)
            self.progressbar.set_fraction(0)
            
        elif response == gtk.RESPONSE_CANCEL:
            print _('Closed, no files selected')
        
        chooser.destroy()
        
        self.textview.grab_focus()
        
    def on_imagemenuitem_save_activate(self, obj, event = None):
        "on_imagemenuitem_save_activate activated"
        chooser = gtk.FileChooserDialog(_("Save.."),
            None,
            gtk.FILE_CHOOSER_ACTION_SAVE,
            (gtk.STOCK_CANCEL, gtk.RESPONSE_CANCEL,
            gtk.STOCK_SAVE, gtk.RESPONSE_OK))
        
        response = chooser.run()
        if response == gtk.RESPONSE_OK:
            filename = chooser.get_filename()
            thefile = open(filename, 'w')
            thefile.write (self.textbuffer.get_text(self.textbuffer.get_start_iter(), self.textbuffer.get_end_iter()))
            thefile.close()
        elif response == gtk.RESPONSE_CANCEL:
            print _('Closed, no files selected')
        
        chooser.destroy()
        
        self.textview.grab_focus()
        
    def on_menuitem_apply_activate(self, obj, event = None):
        "on_menuitem_apply_activate activated"
        if not (self.filename and self.pixbuf) :
            return
           
        pix = ho_gtk_pixbuf_load(self.filename)

        if not pix:
            print "ERROR: Can't load image."
            show_error_message(_("Can't load image."))
            return
        
        # set ocr options
        self.hocr_obj.set_pixbuf(pix)
        self.hocr_obj.set_html(self.menuitem_html.get_active())
        self.hocr_obj.set_nikud(self.menuitem_nikud.get_active())
        if self.menuitem_column_auto.get_active():
              self.hocr_obj.set_paragraph_setup(0)
        else:
              self.hocr_obj.set_paragraph_setup(1)
        
        # run ocr
        ro = RunOCR()
        ro.start()
    
    def on_imagemenuitem_quit_activate(self, obj, event = None):
        "on_imagemenuitem_quit_activate activated"
        gtk.main_quit()

    def on_imagemenuitem_cut_activate(self, obj, event = None):
        "on_imagemenuitem_cut_activate activated"
        self.textbuffer.cut_clipboard(self.clipboard, True)

    def on_imagemenuitem_copy_activate(self, obj, event = None):
        "on_imagemenuitem_copy_activate activated"
        self.textbuffer.copy_clipboard(self.clipboard)

    def on_imagemenuitem_paste_activate(self, obj, event = None):
        "on_imagemenuitem_paste_activate activated"
        self.textbuffer.paste_clipboard(self.clipboard, None, True)

    def on_imagemenuitem_delete_activate(self, obj, event = None):
        "on_imagemenuitem_delete_activate activated"
        self.textbuffer.delete_selection(True, True)

    def on_menuitem_zoom_in_activate(self, obj, event = None):
        "on_menuitem_zoom_in_activate activated"
        self.zoom_factor *= 1.2
        
        if not self.pixbuf:
            return
        
        factor = self.zoom_factor
        w = self.pixbuf.get_width()
        h = self.pixbuf.get_height()
        window_pixbuf = self.pixbuf.scale_simple(int(w * factor), int(h * factor), gtk.gdk.INTERP_NEAREST)
        self.image.set_from_pixbuf(window_pixbuf)

    def on_menuitem_zoom_out_activate(self, obj, event = None):
        "on_menuitem_zoom_out_activate activated"
        self.zoom_factor *= 0.8
        
        if not self.pixbuf:
            return
        
        factor = self.zoom_factor
        w = self.pixbuf.get_width()
        h = self.pixbuf.get_height()
        window_pixbuf = self.pixbuf.scale_simple(int(w * factor), int(h * factor), gtk.gdk.INTERP_NEAREST)
        self.image.set_from_pixbuf(window_pixbuf)

    def on_menuitem_zoom_100_activate(self, obj, event = None):
        "on_menuitem_zoom_100_activate activated"
        self.zoom_factor = 1.0
        
        if not self.pixbuf:
            return
        
        factor = self.zoom_factor
        w = self.pixbuf.get_width()
        h = self.pixbuf.get_height()
        window_pixbuf = self.pixbuf.scale_simple(int(w * factor), int(h * factor), gtk.gdk.INTERP_NEAREST)
        self.image.set_from_pixbuf(window_pixbuf)
    
    def on_menuitem_best_fit_activate(self, obj, event = None):
        "on_menuitem_best_fit_activate activated"
        
        if not self.pixbuf:
            return
        
        width, height = self.window_main.get_size()
        w = self.pixbuf.get_width()
        h = self.pixbuf.get_height()
        # give image some leeway
        if width > 100:
            width -= 40;
        self.zoom_factor = 1.0 * width / w
                
        factor = self.zoom_factor
        
        window_pixbuf = self.pixbuf.scale_simple(int(w * factor), int(h * factor), gtk.gdk.INTERP_NEAREST)
        self.image.set_from_pixbuf(window_pixbuf)
    
    def on_imagemenuitem_about_activate(self, obj, event = None):
        "on_imagemenuitem_about_activate activated"
        dialog = gtk.AboutDialog()
        dialog.set_name(app_name)
        dialog.set_version(app_version)
        dialog.set_copyright(copyright)
        dialog.set_comments(comments)
        dialog.run()
        
        dialog.destroy()
        
        self.textview.grab_focus()
        
    def on_toolbutton_open_clicked(self, obj, event = None):
        "on_toolbutton_open_clicked activated"
        self.on_imagemenuitem_new_activate(self, None)

    def on_toolbutton_apply_clicked(self, obj, event = None):
        "on_toolbutton_apply_clicked activated"
        self.on_menuitem_apply_activate(self, None)

    def on_toolbutton_save_clicked(self, obj, event = None):
        "on_toolbutton_save_clicked activated"
        self.on_imagemenuitem_save_activate(self, None)

    def on_toolbutton_zoom_in_clicked(self, obj, event = None):
        "on_toolbutton_zoom_in_clicked activated"
        self.on_menuitem_zoom_in_activate(self, None)

    def on_toolbutton_zoom_out_clicked(self, obj, event = None):
        "on_toolbutton_zoom_out_clicked activated"
        self.on_menuitem_zoom_out_activate(self, None)

    def on_toolbutton_zoom_100_clicked(self, obj, event = None):
        "on_toolbutton_zoom_100_clicked activated"
        self.on_menuitem_zoom_100_activate(self, None)
    
    def on_toolbutton_best_fit_clicked(self, obj, event = None):
        "on_toolbutton_zoom_100_clicked activated"
        self.on_menuitem_best_fit_activate(self, None)
    
    def on_toolbutton_quit_clicked(self, obj, event = None):
        "on_toolbutton_quit_clicked activated"
        gtk.main_quit()

# run main loop
def main():
    main_window = MainWindow()
    main_window.window_main.show()
    gtk.main()

#Initializing the gtk's thread engine
gtk.gdk.threads_init()

# things we need global
progressbar = None
hocr_obj = None
menuitem_clear = None
textbuffer = None
textview = None

if __name__ == "__main__":
    main()

