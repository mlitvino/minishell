#!/bin/bash
# filepath: /home/mlitvino/Current/minishell/filter.sh

# Temporary file to store filtered output
temp_file=$(mktemp)

# Run mstest, filter lines with ❌ or section headers, and remove ANSI escape sequences
bash "/home/$USER/42_minishell_tester/tester.sh" m \
  | sed -r 's/\x1B\[[0-9;]*[mK]//g' \
  | grep -E '❌|^\s*#\s*\*{76}\s*$|^\s*#\s*[A-Z ]+\s*#\s*$' \
  > "$temp_file"

# Process each line in the temp file
while IFS= read -r line; do
  # Check if the line is a section header using grep
  if echo "$line" | grep -qE '^\s*#\s*\*{76}\s*$|^\s*#\s*[A-Z ]+\s*#\s*$'; then
    echo "$line"
    echo
    continue
  fi

  # Print the original error line
  echo "$line"
  echo

  # Extract the file path and line number using regex
  if [[ $line =~ ([^[:space:]]+):([0-9]+) ]]; then
    file_path="${BASH_REMATCH[1]}"
    line_number="${BASH_REMATCH[2]}"

    # Check if the file exists
    if [[ -f "$file_path" ]]; then
      # Read the specific line from the file
      file_line=$(sed -n "${line_number}p" "$file_path")
      echo "	line - [$file_line]"
    else
      echo "Error: File not found - $file_path"
    fi
  else
    echo "Error: Could not extract file path and line number from: $line"
  fi
  echo
done < "$temp_file"

# Clean up the temporary file
rm -f "$temp_file"
