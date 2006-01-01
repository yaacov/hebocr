#! /usr/bin/perl

# use the hocr perl module
use hocr;

# get path from user
if ($#ARGV == 0)
{
	# create a new Hocr object from pnm picture file
	$h = hocrc::new_Hocr($ARGV[0]);

	# print out text
	print hocrc::Hocr_do_ocr ($h);
}
else
{
	print "USAGE: hocr_perl.pl path_to_pnm_file\n";
}
