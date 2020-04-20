max_modes = 4

clicks = 1
while True:
    found_answer = True
    for mode in range(1, max_modes + 1):
        if clicks % mode != 0: # 1 = On, 0 == Off
            found_answer = False
            break
    if found_answer:
        print('Answer is: ' + str(clicks))
        break

    clicks += 1
       
f
