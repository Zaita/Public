from datetime import timedelta

brightness = [ 6, 2, 5, 5, 4, 5, 6, 3, 7, 6 ]

h_hour = 0
h_min = 0
l_hour = 0
l_min = 0


brightest_value = 0
brightest_time = ''
dimmest_value = 4 * 7
dimmest_time = ''

for hour_int in range(0, 24):    
    hour_str = str(hour_int)

    for minute_int in range(0, 60):
        minute_str = str(minute_int)
        if minute_int < 10:
            minute_str = '0' + minute_str

        current_brightness = 0
        for s in hour_str:
            current_brightness += brightness[int(s)]
        for s in minute_str:
            current_brightness += brightness[int(s)]
        if current_brightness > brightest_value:
            brightest_value = current_brightness
            brightest_time = hour_str + ':' + minute_str
            h_hour = hour_int
            h_min = minute_int
        elif current_brightness < dimmest_value:
            dimmest_value = current_brightness
            dimmest_time = hour_str + ':' + minute_str
            l_hour = hour_int
            l_min = minute_int

print('--')
print('Brightest Time: ' + brightest_time + ' = Value ' + str(brightest_value))
print('Darkest Time: ' + dimmest_time + ' = Value ' + str(dimmest_value))

t1 = timedelta(hours=h_hour, minutes=h_min)
t2 = timedelta(hours=l_hour, minutes=l_min)
difference = t1 - t2
print('Difference: ' + str(difference))
