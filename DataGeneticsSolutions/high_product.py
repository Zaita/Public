"""
This code is using binary comparisons to turn on
digits from each side of the equation to check
all possible values quickly. We have limited
all numbers to be descending in digits

We start with 9876543210 and then increment a counter
doing a binary comparison:

e.g
#1 (1)     x == 9 : y == 876543210
#2 (2)     x == 8 : y == 976543210
#3 (2+1)   x == 98 : y == 76543210
#4 (4)     x == 7 : y == 986543210
#5 (4+1)   x == 97 : y == 86543210
#6 (4+2)   x == 87 : y == 96543210
#7 (4+2+1) x == 987 : y == 6543210
#8 (8)     x == 6 : y == 987543210
#9 (8+1)   x == 96 : y == 87543210
#10 (8+2)  x == 86 : y == 97543210
"""
max_x = 0
max_y = 0
max_prod = 0

for i in range(1, 1024):
    x = ''
    y = ''

    binary = 1
    for j in range(0, 10):
        value = 9 - j
        
        if i & binary == binary:
            x = x + str(value)
        else:
            y = y + str(value)
        binary *= 2

    if x == '' or y == '':
        continue
    
    prod = int(x) * int(y)
    if (prod > max_prod):
        max_x = x
        max_y = y
        max_prod = prod

print (str(max_x) + ' * ' + str(max_y) + ' = ' + str(max_prod))

    
    
    
