import matplotlib.pyplot as plt

infile = open("staticField.txt", 'r')
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

infile = open("initialPosition.txt", 'r')
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

infile = open("finalPosition.txt", 'r')
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

