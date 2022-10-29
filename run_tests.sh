#!/bin/bash
make new >> comp.final_log
touch run.final_log
echo "" > run.final_log
files=./examples/corrected_codes/*.comp
echo 'files:'
printf '\t%s\n' ${files[@]}
for file in $files; do
  echo "TESTING $file ---" >> run.final_log
  ./comp $file 1 >> run.final_log
done