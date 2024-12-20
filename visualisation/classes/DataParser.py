import glob
import numpy as np
import re 

class DataParser:
    def __init__(self, fname_pattern, separator='\n'):
        self.fname_pattern = fname_pattern
        self.separator = separator
        self.all_data = []
        self.data_desc = [] 

    # for context 
    def __enter__(self):
        print(f"Parsing files matching pattern: {self.fname_pattern}")
        for fname in glob.glob(self.fname_pattern):
            print(f"Reading file: {fname}")
            with open(fname, 'r') as file:
                raw_data = file.read()
                if self.separator is not "":  
                    rows = raw_data.strip().split(self.separator)
                else: 
                    rows = raw_data 
                try:
                    data = [float(value) for value in rows]
                    self.all_data.append(data)
                    
                    match = re.search(r'[a-z]_(\d\.\d+)_(\d+)', fname)
                    if match:
                        desc = [float(match.group(1)), int(match.group(2))]
                        self.data_desc.append(desc)
                except ValueError as e:
                    print(f"Skipping invalid data in {fname}: {e}")
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        if exc_type is not None:
            print(f"An error occurred: {exc_value}")
        else:
            print("Data parsing completed successfully.")
    # for context 

    def array_to_np(self):
        converted_data = []
        for data in self.all_data:
            try:
                np_array = np.array([float(value) for value in data])
                converted_data.append(np_array)
            except ValueError as e:
                print(f"Error converting data to numpy array: {e}")
        return converted_data