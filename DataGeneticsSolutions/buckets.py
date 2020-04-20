import math
import itertools

buckets = []
buckets.append([])
buckets.append([])
buckets.append([])

digits = [ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 ]
size = len(digits)

def check_bucket(index, value):
    this_bucket = True
    for existing in buckets[index]:
        abs_value = abs(existing - value)
        if abs_value in buckets[index]:
            print('Not bucket ' + str(index) + ' because of ' + str(existing))
            this_bucket = False
            break
    if this_bucket:
        print('Bucket: ' + str(index))
    return this_bucket

def start():
    last_bucket_index = 0
    for value in digits:
        print('-- Value: ' + str(value))
        if check_bucket(last_bucket_index, value):            
            buckets[last_bucket_index].append(value)
        else:
            for i in range(0, 3):
                if check_bucket(i, value):
                    buckets[i].append(value)
                    last_bucket_index = i
                    break

start()    
if len(buckets[0]) + len(buckets[1]) + len(buckets[2]) == size:
    print('--> Found Solution')    
else:
    print('--> No Solution')
    
print('Bucket 0: ' + str(buckets[0]))
print('Bucket 1: ' + str(buckets[1]))
print('Bucket 2: ' + str(buckets[2]))
    
            

            
    
