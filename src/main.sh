cc_files="src.cc game.cc"
out_file=/tmp/life.out

# reset
# clear
g++ -o $out_file -Ofast $cc_files
$out_file
