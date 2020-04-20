import itertools

digits = [ '1', '2', '3', '4', '5', '6', '7', '8', '9' ]
size = len(digits)

for perms in itertools.permutations(digits, size):
    perm = ''.join(perms)
    for x in range(1, size):
        numerator = perm[:x]
        denom = perm[x::]

        ans = int(denom) / int(numerator)
        if ans == 3:
            print (perm + ' = ' + numerator + ' / ' + denom)
        
            
            
        
            
        
    

