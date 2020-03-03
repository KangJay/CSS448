#!/bin/bash

for togfile in ../Tests/*.tog 
do
  echo "--------Running test on $togfile--------"
  echo "Press 'enter' to execute next file"
  ./rec $togfile > tempFile.txt
  echo "TokenDump.txt for $togfile"
  #echo "Press 'enter' to execute next file"
  cat TokenDump.txt 
  echo ""
done
echo "" 
echo "Done running all tests"
rm tempFile.txt
