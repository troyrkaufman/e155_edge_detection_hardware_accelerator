import sys

def bin_to_header(bin_file_path, header_file_path, array_name):
    # Open the binary file in read-binary mode
    with open(bin_file_path, 'rb') as bin_file:
        # Read all the content of the binary file
        bin_data = bin_file.read()

    # Open the header file for writing
    with open(header_file_path, 'w') as header_file:
        # Write the header guard
        header_file.write("#ifndef IMAGE_DATA_H\n")
        header_file.write("#define IMAGE_DATA_H\n\n")

        # Write the array definition
        header_file.write(f"unsigned char {array_name}[] = {{\n")
        
        # Iterate over the binary data and write each byte in the C array format
        for i, byte in enumerate(bin_data):
            # Write the byte as a hexadecimal value
            if i != len(bin_data) - 1:
                header_file.write(f"0x{byte:02X}, ")
            else:
                header_file.write(f"0x{byte:02X}\n")
        
        # End the array and close the header guard
        header_file.write("};\n\n")
        header_file.write("#endif // IMAGE_DATA_H\n")

    print(f"Binary data has been written to {header_file_path}")

if __name__ == "__main__":
    # Ensure you provide the correct binary file and output header file names
    if len(sys.argv) != 4:
        print("Usage: python bin_to_header.py <input_bin_file> <output_header_file> <array_name>")
    else:
        bin_file_path = sys.argv[1]
        header_file_path = sys.argv[2]
        array_name = sys.argv[3]
        bin_to_header(bin_file_path, header_file_path, array_name)
