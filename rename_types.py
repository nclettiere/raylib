import json
import os
import re
import shutil
from glob import glob


def get_symbols_from_json(file_path):
    """Extract all relevant symbols from a rlparser-generated JSON file."""
    symbols = set()
    if not os.path.exists(file_path):
        print(f"Warning: {file_path} not found. Skipping.")
        return symbols

    with open(file_path, "r", encoding="utf-8") as f:
        data = json.load(f)

    # Defines: exclude header guards
    for define in data.get("defines", []):
        if define.get("type") != "GUARD":
            symbols.add(define["name"])

    # Structs
    for struct in data.get("structs", []):
        symbols.add(struct["name"])

    # Type aliases (e.g. Texture2D, Camera, Quaternion)
    for alias in data.get("aliases", []):
        symbols.add(alias["name"])

    # Enums (e.g. ConfigFlags, KeyboardKey)
    for enum in data.get("enums", []):
        symbols.add(enum["name"])

    # Functions (all public API functions)
    for func in data.get("functions", []):
        symbols.add(func["name"])

    return symbols


def rename_symbols_in_file(input_path, output_path, symbols):
    """Prefix all occurrences of symbols with 'rl_' using word boundaries."""
    with open(input_path, "r", encoding="utf-8") as f:
        content = f.read()

    # Sort longest first to prevent partial replacements (e.g., InitWindow before Window)
    sorted_symbols = sorted(symbols, key=len, reverse=True)

    for symbol in sorted_symbols:
        # \b ensures whole-word match (avoids matching inside other identifiers)
        pattern = r"\b" + re.escape(symbol) + r"\b"
        replacement = "rl_" + symbol
        content = re.sub(pattern, replacement, content)

    with open(output_path, "w", encoding="utf-8") as f:
        f.write(content)


def batch_rename_raylib_src(
    base_dir="raylib",
    src_subdirs=["src", "src/platforms"],
    output_base_dir="raylib_renamed",
    json_files=["raylib.json", "raymath.json"],
):
    """
    Renames all Raylib symbols in .c and .h files across src/ and src/platforms/.
    """
    # Collect all symbols
    all_symbols = set()
    for json_file in json_files:
        all_symbols.update(get_symbols_from_json(json_file))

    if not all_symbols:
        print("No symbols collected. Check that JSON files exist and are valid.")
        return

    print(f"Collected {len(all_symbols)} unique symbols to rename with 'rl_' prefix.\n")

    # Remove old output if exists
    if os.path.exists(output_base_dir):
        shutil.rmtree(output_base_dir)

    # Process each subdirectory
    processed_count = 0
    for subdir in src_subdirs:
        input_dir = os.path.join(base_dir, subdir)
        output_dir = os.path.join(output_base_dir, subdir)

        if not os.path.isdir(input_dir):
            print(f"Directory not found: {input_dir} -> skipping")
            continue

        os.makedirs(output_dir, exist_ok=True)

        # Find all .c and .h files
        pattern = os.path.join(input_dir, "*.[ch]")
        files = glob(pattern)

        for file_path in files:
            filename = os.path.basename(file_path)
            rel_path = os.path.relpath(file_path, base_dir)
            out_path = os.path.join(output_base_dir, rel_path)

            # Special renaming for main headers
            if filename == "raylib.h":
                out_path = os.path.join(output_base_dir, "src", "raylib.h")
                os.makedirs(os.path.dirname(out_path), exist_ok=True)
            elif filename == "raymath.h":
                out_path = os.path.join(output_base_dir, "src", "raymath.h")
                os.makedirs(os.path.dirname(out_path), exist_ok=True)

            rename_symbols_in_file(file_path, out_path, all_symbols)
            print(f"Renamed: {rel_path} → {os.path.relpath(out_path, output_base_dir)}")
            processed_count += 1

    print(f"\nDone! Processed {processed_count} files.")
    print(f"Renamed sources are in: {output_base_dir}/")
    print("\nNext steps:")
    print("1. Compile the renamed library from the 'raylib_renamed' folder.")
    print('2. In your project, #include "rl_raylib.h" instead of "raylib.h"')
    print(
        "3. All Raylib calls now need the rl_ prefix: rl_InitWindow(), rl_DrawTexture(), etc."
    )
    print(
        "4. Optionally add #define RL_NO_WIN32_CONFLICTS before including Windows headers if needed."
    )


# =============================================================================
# Run the script
# =============================================================================
if __name__ == "__main__":
    # Adjust paths if your folder structure is different
    batch_rename_raylib_src(
        base_dir=".",  # folder containing src/
        src_subdirs=["src", "src/platforms"],  # both main src and platforms
        output_base_dir="raylib_renamed",
        json_files=[
            "raylib.json",
            "raymath.json",
        ],  # place these in the script's directory
    )
