import os
import re

def analyze_cpp_project(directory, exclude_paths=None):
    stats = {
        "files": 0,
        "lines": 0,
        "comments": 0,
        "classes": 0,
        "functions": 0,
    }

    if exclude_paths is None:
        exclude_paths = []

    # Compile regex patterns
    cpp_file_pattern = re.compile(r".*\.(cpp|h|hpp|c|cc)$")
    class_pattern = re.compile(r"^\s*class\s+\w+")
    function_pattern = re.compile(r"^\s*[a-zA-Z_][a-zA-Z0-9_]*\s+\w+\s*\([^)]*\)\s*\{?")
    comment_pattern = re.compile(r"^\s*(//|/\*|\*|\*/)")
    
    # Normalize exclude paths
    exclude_paths = [os.path.abspath(path) for path in exclude_paths]

    for root, _, files in os.walk(directory):
        # Skip excluded paths
        if any(os.path.abspath(root).startswith(excluded) for excluded in exclude_paths):
            continue

        for file in files:
            if cpp_file_pattern.match(file):
                stats["files"] += 1
                with open(os.path.join(root, file), 'r', encoding='utf-8', errors='ignore') as f:
                    for line in f:
                        stats["lines"] += 1
                        if comment_pattern.match(line):
                            stats["comments"] += 1
                        elif class_pattern.match(line):
                            stats["classes"] += 1
                        elif function_pattern.match(line):
                            stats["functions"] += 1

    return stats

# Usage
project_directory = "./"
exclude_directories = ["./include/librdkafka"]

statistics = analyze_cpp_project(project_directory, exclude_paths=exclude_directories)
print(f"Statistics for {project_directory}:")
for key, value in statistics.items():
    print(f"{key.capitalize()}: {value}")
