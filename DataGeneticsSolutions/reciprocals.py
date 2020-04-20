import math
import itertools

digits = []

def search():
    for perm in itertools.combinations(digits, 6):        
        total = 0.0
        for x in perm:
            total += (1 / x)
            if total > 1.0:
                break
        if total == 1.0:
            print('Solution: ' + str(perm))
            return True
    return False

max_digit = 6
while True:
    digits = []
    for i in range(1, max_digit + 1):
        digits.append(i)
    print('Max Digit: ' + str(max_digit))
    if search():
        break
    max_digit += 1
    



    
