even = 0
odd = 0
total = 0
for die_1 in range(1, 7):
    for die_2 in range(1, 7):
        for die_3 in range(1, 7):
            total += 1
            if (die_1 * die_2 * die_3) % 2 != 0:
                print (str(die_1) + '*' + str(die_2) + '*' + str(die_3))
                odd += 1

print ("")
print ( str(odd) + ' / ' + str(total))

if total % odd == 0:
    total = total / odd
    odd = odd / odd

print ( str(odd) + ' / ' + str(total))

