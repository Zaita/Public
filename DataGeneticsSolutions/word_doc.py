value = ''
for i in range(1, 10001):
    value = value + str(i) + ' '

value = value.replace('0', ' ')
values = value.split(' ')

total = 0;
for num in values:
    if num.lstrip().rstrip().strip() == '':
        continue
    total += int(num)

print ('Total: ' + str(total))

