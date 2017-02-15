cc_file=src.cc
out_file=/tmp/cc.out

# reset
clear
g++ $cc_file -o $out_file
$out_file
