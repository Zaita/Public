import itertools

digits = [ '1', '2', '3', '4', '5', '6', '7', '8', '9' ]
for outer_perm in itertools.permutations(digits, 5):
    value = int(''.join(outer_perm))
    total = 0
    for perm in itertools.permutations(outer_perm, 3):
        total += int(''.join(perm))
    if total == value:
        print('Answer: ' + str(value))
