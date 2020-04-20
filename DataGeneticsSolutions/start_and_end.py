number = 15
while True:
    number += 1
    last_digit = str(number)[-1:]
    if last_digit != '6':
        continue

    small = int(number / 10)
    large = int('6' + str(small))   
    
    if large % number == 0:
        print('--')
        print(str(number) + ' || ' + str(small) + ' : ' + str(large))
        print('Division: ' + str(int(large / number)))
        if int(large / number) == 4:
            print('Answer: ' + str(number))        
            break

    
    
