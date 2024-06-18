#!/bin/bash
echo "------Converting Images------"

file_with_extension() {
    local dir="$1"
    local ext="$2"

    if [[ $ext != .* ]]; then
        ext=".$ext"
    fi

    # Iterate through all files in the directory
    for file in "$dir"/*; do
        if [[ -f $file && $file == *$ext ]]; then
            return 0
        fi
    done

    return 1
}

if file_with_extension "markers/" "bmp"; then
  for filename in markers/*.bmp; do
    convert "$filename" "${filename%.bmp}".png
  done
  echo "Deleting .bmp files"
  rm markers/*.bmp
else
  echo "No .bmp files"
fi

echo "------Converting Images - Done------"
