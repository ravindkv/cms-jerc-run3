import re
import json
import os

def read_twiki_content(file_path):
    """
    Reads the TWiki page content from a local file.
    """
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            return f.read()
    except FileNotFoundError:
        raise Exception(f"The file {file_path} does not exist.")
    except Exception as e:
        raise Exception(f"An error occurred while reading the file: {e}")

def parse_tables(raw_content):
    """
    Parses the raw TWiki content and extracts tables.
    Returns a list of dictionaries containing table metadata and rows.
    """
    tables = []
    lines = raw_content.splitlines()
    current_table = None
    table_header = []

    for line in lines:
        # Identify table headers, e.g., '---++ Table of manual updates in 2024'
        header_match = re.match(r'^---\+\+ Table of manual updates in (\d{4})', line)
        if header_match:
            if current_table:
                tables.append(current_table)
            year = header_match.group(1)
            current_table = {
                'year': year,
                'rows': []
            }
            table_header = []
            continue

        # Identify table lines starting with '|'
        if line.startswith('|') and current_table is not None:
            # Split the line by '|' but ignore the first empty split
            columns = [col.strip() for col in line.strip('|').split('|')]
            if not table_header:
                # This is the header row
                table_header = columns
            else:
                # This is a data row
                row = {}
                for idx, col in enumerate(columns):
                    if idx >= len(table_header):
                        # Prevent index errors if rows have more columns than headers
                        continue
                    # Clean the header names to be JSON-friendly
                    header = re.sub(r'\s+', '_', table_header[idx].strip('*').lower())
                    # Remove any TWiki macros or markup
                    col_clean = re.sub(r'%[^%]+%|\[\[([^\]]+)\]\]', r'\1', col)
                    # Split multiple entries separated by commas and strip spaces
                    if ',' in col_clean:
                        col_clean = [item.strip() for item in col_clean.split(',')]
                    else:
                        # Attempt to convert numeric fields to integers
                        col_clean = col_clean.strip()
                        if col_clean.isdigit():
                            col_clean = int(col_clean)
                    row[header] = col_clean
                current_table['rows'].append(row)

    # Append the last table if exists
    if current_table:
        tables.append(current_table)

    return tables

def save_tables_as_json(tables, output_dir='output'):
    """
    Saves each table as a separate JSON file in the specified output directory.
    """
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    for table in tables:
        year = table['year']
        # Define the JSON file name. You can customize this as needed.
        json_filename = f"conditions_{year}.json"
        json_path = os.path.join(output_dir, json_filename)

        with open(json_path, 'w', encoding='utf-8') as json_file:
            json.dump(table['rows'], json_file, indent=4)

        print(f"Saved table for year {year} to {json_path}")

def main():
    # Path to your local TWiki raw content file
    file_path = 'twiki_raw.txt'  # Replace with your actual file path

    # Output directory for JSON files
    output_directory = 'output'  # You can change this as needed

    try:
        print("Reading TWiki page from local file...")
        raw_content = read_twiki_content(file_path)
        print("Parsing tables...")
        tables = parse_tables(raw_content)
        if not tables:
            print("No tables found in the TWiki page.")
            return
        print(f"Found {len(tables)} table(s). Saving to JSON...")
        save_tables_as_json(tables, output_dir=output_directory)
        print("All tables have been successfully saved.")
    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    main()

