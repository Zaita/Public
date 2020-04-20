legs = 200
heads = 72

max_dogs = int(legs / 4 - (legs % 4))

for i in range(1, 73):
    if i * 4 > legs:
        continue;
    
    left_over_legs = legs - (i * 4)
    left_over_heads = heads - i

    if left_over_heads * 2 == left_over_legs:
        print('Dogs: ' + str(i) + ' and People: ' + str(left_over_heads))
        break
