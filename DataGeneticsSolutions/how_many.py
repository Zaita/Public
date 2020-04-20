import itertools

digits = [ '1', '2', '3', '4', '5', '6', '7', '8', '9', '0']
size = len(digits)

count = 0
for perm in itertools.permutations(digits, size):
    if perm[0] == '0':
        continue
    number = int(''.join(perm))
    if number % 11111 == 0:
        count += 1

print('Count: ' + str(count))

    
