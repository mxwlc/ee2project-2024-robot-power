#!/bin/bash
rm appendix.txt
touch appendix.txt
# shellcheck disable=SC2129

# Save Header Files
echo "================================================" >> appendix.txt
echo "Header Files" >> appendix.txt
echo "================================================" >> appendix.txt
for filename in include/*.hpp; do
    echo "--------------------" >> appendix.txt
    echo "$filename" >> appendix.txt
    echo "--------------------" >> appendix.txt
    cat $filename >> appendix.txt
done

# Save Source Files
echo "================================================" >> appendix.txt
echo "Source Files" >> appendix.txt
echo "================================================" >> appendix.txt
for filename in src/*.cpp; do
  echo "--------------------" >> appendix.txt
  echo "$filename" >> appendix.txt
  echo "--------------------" >> appendix.txt
  cat $filename >> appendix.txt
  echo "" >> appendix.txt
done


# Save CMake Files
echo "================================================" >> appendix.txt
echo "CMakeFiles" >> appendix.txt
echo "================================================" >> appendix.txt
FILENAME="CMakeLists.txt";
echo "--------------------" >> appendix.txt
echo "$FILENAME" >> appendix.txt
echo "--------------------" >> appendix.txt
cat $FILENAME >> appendix.txt

