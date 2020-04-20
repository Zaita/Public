
for i in range(100, 1000):
    base9 = ''
    base10 = i
    # Convert to base 9    
    for j in range(0,3):
        r = base10 % 9        
        base10 = (base10 - r) / 9        
        base9 = str(int(r)) + base9    

    # compare base 10 to reversed base9
    if str(i) == str(base9)[::-1]:
        print('Found: ' + str(i) + '^10 == ' + str(base9) + '^9')
        break


        
    
    
