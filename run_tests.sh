#!/bin/bash
make clean
make
for file in ./examples/*.comp; do
  $(./comp $file)
done