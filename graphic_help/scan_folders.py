
import os
import sys
import shutil
import argparse
import numpy as np
import matplotlib.pyplot as plt
import csv # to read and write comma separated values (csv)

def give_averages(points):
    mean_points = []
    for i in points:
        mean_points.append(sum(i)/len(i))
    return mean_points

def extract_info_from_files(list_of_files):
    """
    In charge of extracting the info. stored in the files
    """
    file_numb = -1
    row_numb = -1
    points = []
    kD = 0
    kS = []
    for i in range(199):
        temp = []
        points.append(temp)
    
    print(points)
    # Iterate all over the list of all files
    for file_name in list_of_files:
        file_numb = file_numb+1
        row_numb = -1
        print(file_numb)

        # Open the file assuming it is a CSV file
        with open(file_name) as csv_file:
            print(file_name)
            csv_reader = csv.reader(csv_file, delimiter=':')
            
            # For each row in the file do the following
            for row in csv_reader:
                row_numb = row_numb + 1
                # Get the number of columns
                n_columns = len(row)
                #print(f"The number of columns is: {nrows}")
                
                # Get each value from the current row
                for i in range(n_columns):
                    row[i]=float(row[i])
                
                if(file_numb == 0):
                    kS.append(row[1])
                    kD = row[0]
                    
                points[row_numb].append(row[2])
    
    return([give_averages(points),kD,kS])
                    #print(f'\t{row[0]} {row[1]} {row[2]}')
                    # SILVIA: En este punto puedes tener acceso a cada
                    # uno de los datos de tu archivo por medio del
                    # arreglo 'row'
                    
def scan_for_files(root_folder, ext):
    """In charge of counting the number of files with extension 'ext' in the
    root folder; also consider subfolders
    """
    # Count the number of files with extension 'ext'
    sum_ext_files = 0
    
    # A list with the full path of the files with extension 'ext'
    files_with_ext = list()
    
    # List of ALL files in root_folder
    list_of_all_files = list()

    # Loop over all subdirectories in the root_folder and store the
    # FULL PATH of each file in the list
    for (dir_path, dir_names, file_names) in os.walk(root_folder):
        list_of_all_files += [os.path.join(dir_path, file) for file in file_names]
    
    # Filter the files with extension 'ext'
    for entry in list_of_all_files:
        # Check for all possible extensions
        for extension in ext:
            if entry.endswith(extension):
                print(f"Filter file ending with '{ext}': {entry}")
                files_with_ext.append(entry)
                sum_ext_files = sum_ext_files + 1
    
    return sum_ext_files, files_with_ext

def main():
    if len(sys.argv) < 2:
        print("Usage: scan_folders.py [--root_folder] [--ext]")
        sys.exit()
    
    # Create the parser to deal with the arguments
    parser = argparse.ArgumentParser("Scan folder and look for files with the given extension")
    
    # Set the positional arguments
    parser.add_argument("--root_folder", type=str, help="The root folder to search for given extensions", required=True)
    parser.add_argument("--ext", type=str, nargs='+', help="The list of extensions to look for and delete", required=True, choices=['txt', 'dat'])
    
    # parse args
    args = parser.parse_args()

    # --------------------------------------------------------------
    # Information
    # --------------------------------------------------------------
    print("Search for '%s' files in '%s' directory and all subdirectories ...\n" % (args.ext, args.root_folder))
    
    print("Scanning ...")
    
    #n_type_files, files_found = scan_for_files_of_a_type(root_folder, type_file, ignore_in_pathlist_ignore_has_in_filename)
    n_files, files_list = scan_for_files(args.root_folder, args.ext)
    #n_type_files, files_found = scan_for_files_of_a_type()
    
    # --------------------------------------------------------------
    # Output results
    # --------------------------------------------------------------
    if n_files > 0:
        print("List of files '%s':" % args.ext)
        for item in files_list:
            print(item)

        print("\nNumber of '%s' files: %d\n" % (args.ext, n_files))
        
        # SILVIA: Revisa lo que hace esta función, ahí te dejé unos
        # comentarios
        final_points = extract_info_from_files(files_list)
        
    else:
        print("No '%s' files found" % args.ext)
        
    
    plt.style.use('ggplot')
    plt.rc('axes',edgecolor='black')
    plt.rc('axes', facecolor = 'white')
    plt.rc('grid', color = 'grey')
    plt.rc('grid', alpha = 0.3)
    
    plt.plot(final_points[2],final_points[0])
    
    plt.xlabel("Static Field sensitivity parameter $K_S$")
    plt.ylabel("Time")
    plt.title("Dynamic Field sensitivity parameter "+ str(final_points[1]))
    
    plt.savefig(args.root_folder)
    print("[Done]\n")

'''
Usage:
  ./scan_folders.py root_folder extension_type
'''
if __name__ == '__main__':
    # Run the main function
    main()
    

