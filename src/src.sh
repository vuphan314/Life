cc_files="src.cc game.cc"
out_file=/tmp/cc.out

reset
# clear
g++ -O3 $cc_files -o $out_file
$out_file
