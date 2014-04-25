#! /bin/bash

base=`pwd`

lcov -z -d ..
lcov -c -i -d .. -b $base -o base.info

find bin_rel -name 'ut*' -exec {} \;

lcov -c -d .. -b $base -o coverage.info

lcov -a base.info -a coverage.info -o total.info

lcov -r total.info '/usr/*' 'moc_*' '*Platform*' '*Containers*' '*Commands*' '*Helper*' '*Mock*' '*Test*' -o total.info

genhtml -o html total.info

rm -f *.info
