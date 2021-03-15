import numpy as np
import argparse
import matplotlib.pyplot as plt

def graph_field(file_name):
    
    infile = open(file_name+".txt", 'r')
    matrix = []
    temp = []
    temp_numb = []
    for word in infile:
        temp_numb.clear()
        temp = word.rstrip('\n').split(':')
        for i in temp:
            temp_numb.append(float(i))
        matrix.append(temp_numb[:])
    
    plt.imshow(matrix, cmap='gray', interpolation='nearest')
    plt.colorbar()
    plt.title("")  
    
    plt.show()

def graph_field_color(file_name):
    
    infile = open(file_name+".txt", 'r')
    matrix = []
    temp = []
    temp_numb = []
    for word in infile:
        temp_numb.clear()
        temp = word.rstrip('\n').split(':')
        for i in temp:
            temp_numb.append(float(i))
        matrix.append(temp_numb[:])
    
    plt.imshow(matrix, cmap='inferno', interpolation='nearest')
    plt.colorbar
    plt.show()

def graph_dynField_mean(file_name, num_files):
    
    for j in range(1,num_files):
        infile = open(file_name + str(j) + ".txt", 'r')
        matrix = []
        temp = []
        temp_numb = []
        for word in infile:
            temp_numb.clear()
            temp = word.rstrip('\n').split(':')
            for i in temp:
                temp_numb.append(float(i))
            matrix.append(temp_numb[:])
        matrix_temp = np.array(matrix)
        
        if j == 1:
            matrix_mean = matrix_temp
            
        else:
            matrix_mean += matrix_temp
            
    matrix_mean /= num_files
            
    plt.imshow(matrix_temp, cmap='coolwarm', interpolation='nearest')
    plt.colorbar()
    plt.savefig("figure" + str(num_files))
    plt.show()
    
def main():
    
    graph_field("iteration3800")

if __name__ == '__main__':
    main()
