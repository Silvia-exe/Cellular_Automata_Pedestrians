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
    plt.show()

graph_field("staticField")
graph_field("initialPosition")
graph_field("finalPosition")


