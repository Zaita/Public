target = 1000000

for i in range(1,500001):    
    if target % i != 0: continue    
    if '0' in str(i): continue

    j = int(target / i)
    if '0' in str(j): continue        

    print ('Factors: ' + str(i) + ' : ' + str(j))
    exit
