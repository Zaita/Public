import math
answers = []

buckets = []
for i in range(1, 18):
    buckets.append([i])

eraseables = []
new_buckets = []
for iteration in range(1, 18):
    for bucket in eraseables:
        if bucket in buckets:
            buckets.remove(bucket)
        if len(buckets) == 1:
            answer = bucket
    eraseables.clear()   
    
    for bucket in new_buckets:
        buckets.append(bucket)
    new_buckets.clear()  
    
    print('Potential Buckets: ' + str(len(buckets)))
    for bucket in buckets:
        if len(bucket) == 17:
            print("Found complete sequence")
            answers.append(bucket)
        last_value = bucket[len(bucket) - 1]
        potentials = []
        for i in range(1, 18):
            if i in bucket:
                continue

            if math.sqrt(last_value + i) % 1 == 0:
                potentials.append(i)
        if len(potentials) == 0:
            eraseables.append(bucket)
        if len(potentials) == 1:
            bucket.append(potentials[0])
        if len(potentials) > 1:
            for value in potentials:
                new_bucket = list(bucket)
                new_bucket.append(value)
                new_buckets.append(new_bucket)
                eraseables.append(bucket)

for answer in answers:
    print('Answer: ' + ''.join(str(answer)))
            
