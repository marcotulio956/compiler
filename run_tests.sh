#!/bin/bash
touch run_corrected_tests.final_log
touch run_faulting_tests.final_log
echo "" > run_corrected_tests.final_log
echo "" > run_faulting_tests.final_log
files1=./examples/corrected_codes/*.comp
files2=./examples/*.comp

echo 'files1:'
printf '\t%s\n' ${files1[@]}
for file in $files1; do
  echo "TESTING $file ---" >> run_corrected_tests.final_log
  ./comp $file 1 >> run_corrected_tests.final_log
done

echo 'files2:'
printf '\t%s\n' ${files2[@]}
for file in $files2; do
  echo "TESTING $file ---" >> run_faulting_tests.final_log
  ./comp $file 1 >> run_faulting_tests.final_log
done