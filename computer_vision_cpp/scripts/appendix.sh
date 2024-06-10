#!/bin/bash
rm appendix.txt
rm appendix.txt
touch appendix.txt
# shellcheck disable=SC2129

# Save Header Files

echo "# Header Files" >> appendix.txt

for filename in include/*.hpp; do
    printf "## %s\n" "$filename" >> appendix.txt
    echo "\`\`\`cpp" >> appendix.txt
    cat $filename >> appendix.txt
    printf "\n" >> appendix.txt
    echo "\`\`\`" >> appendix.txt
done

# Save Source Files
echo "# Source Files" >> appendix.txt
for filename in src/*.cpp; do
  printf "## %s\n" "$filename" >> appendix.txt
  echo "\`\`\`cpp" >> appendix.txt
  cat $filename >> appendix.txt
  printf "\n" >> appendix.txt
  echo "\`\`\`" >> appendix.txt
done


# Save CMake Files

echo "# CMakeFiles" >> appendix.txt
FILENAME="CMakeLists.txt";
printf "## %s\n" "$FILENAME" >> appendix.txt
echo "\`\`\` cmake" >> appendix.txt
cat $FILENAME >> appendix.txt
printf "\n \`\`\` \n" >> appendix.txt

echo "# Bash Scripts" >> appendix.txt
for filename in scripts/*.sh; do
  printf "## %s \n" "$filename" >> appendix.txt
  printf "\`\`\` bash \n" >> appendix.txt
  cat $filename >> appendix.txt
  printf "\n \`\`\` \n" >> appendix.txt
done

cat appendix.txt > appendix.md
echo "Appendix Generated -- Done!"