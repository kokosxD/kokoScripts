import argparse

# Create a command line argument parser
arg_parser = argparse.ArgumentParser(description="Converts a string to a hexadecimal array")

# Input string
arg_parser.add_argument("-string", action="store", nargs="?", type=str, required=True, help="String to be converted to a hexadecimal array")

# Null terminate the given hexadecimal array
arg_parser.add_argument("-null_terminate", action="store_true", default=True, required=False, help="Hexadecimal array will be null terminated", dest="terminate")
arg_parser.add_argument("-no_terminate", action="store_false", required=False, help="Hexadecimal array won't be null terminated", dest="terminate")

# Escape the given string
arg_parser.add_argument("-escape", action="store_true", default=True, required=False, help="String will be escaped", dest="escape_string")
arg_parser.add_argument("-no_escape", action="store_false", required=False, help="String won't be escaped", dest="escape_string")

# Capitalize hexadecimal values
arg_parser.add_argument("-capitalize", action="store_true", default=True, required=False, help="Characters in hexadecimal values will be capitalizer", dest="capitalize_hex")
arg_parser.add_argument("-no_capitalize", action="store_false", required=False, help="Characters in hexadecimal values won't be capitalizer", dest="capitalize_hex")

# Parse command line arguments
parsed_args = arg_parser.parse_args()

# Check for valid string
if not parsed_args.string:
	raise TypeError("Missing string to be converted to a hexadecimal array!")

# Convert string to a hexadecimal array
input_string = bytes(parsed_args.string, "utf-8").decode("unicode_escape") if parsed_args.escape_string else parsed_args.string
hex_array = [format(ord(character), "x") for character in input_string]

# Capitalize hexadecimal array if specified
if parsed_args.capitalize_hex:
	hex_array = [hexadecimal.upper() for hexadecimal in hex_array]

# Null terminate hexadecimal array is specified
if parsed_args.terminate:
	hex_array.append("00")

# Show output
conv_hex_array = ", ".join(f"0x{hexadecimal}" for hexadecimal in hex_array)
print(f"Converted hexadecimal array\t==\t{{{conv_hex_array}}}")