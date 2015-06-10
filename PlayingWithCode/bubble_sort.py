import random

array = []
for i in range(0, 200):
  array.append(int(random.random() * 200))

print('Array Size: ' + str(len(array)))
print('Unsorted Array:')
print(array)

clean_pass = False
passes = 0
while not clean_pass:
  clean_pass = True
  passes += 1
  for i in range(0, 199):
    if array[i] > array[i + 1]:
      temp = array[i]
      array[i] = array[i + 1]
      array[i + 1] = temp
      clean_pass = False
      
print('Sorted Array:')
print(array)
print('Passes: ' + str(passes))
