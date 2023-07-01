import random

count = 0
text = ''
items = 0
while(count < 90000000):
    x = random.randint(1, 100000)
    if(random.randint(1,100) <= 80 or items == 0):
        text += f'enqueue {x}\n'
        items += 1
    else:
        text += f'dequeue {x}\n'
        items -= 1
    count+=1
with open("in.txt", 'w') as f:
    f.write(text)

print(items)