import random

count = 0
x = 0
command = ''
text = ''
added = []
added.append(12)

while(count < 60000):
    if(len(added) == 0 or random.randint(0,100) <= 50):
        x = random.randint(1, 2147483647)
        command = 'aggiungi'    
        added.append(x)
    else:
        x = added.pop()
        command = 'rimuovi'

    text += f"{command} {x}\n"
    count += 1

text += "exit 123\n"
with open("in.txt", 'w') as f:
    f.write(text)