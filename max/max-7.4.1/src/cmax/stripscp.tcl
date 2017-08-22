#!/usr/local/dist/bin/tclsh

# This script removes the source code position tags from
# generated (c)max files
# The files will become smaller and the code will be (almost)
# readable.
# Use this script only to inspect the code generated by cmax as
# the source code position tags are important for analysing
# error messages.
# 
# An original version of the processed file is placed in
# /tmp/tmpfile.stripscp
#
# (c) Copyright 1998 by Aurel Huber, TU Muenchen


set sourcefile [lindex $argv 0]

set tmpfile "/tmp/tmpfile.stripscp"

exec mv $sourcefile $tmpfile

set srcfid [open $tmpfile r]
set destfid [open $sourcefile w]

while {[gets $srcfid line] >=0} {
  regsub -all {[$][0-9]+,[0-9]+,["][^"]*["][$]} $line {} newline
  puts $destfid $newline
}

close $srcfid
close $destfid


