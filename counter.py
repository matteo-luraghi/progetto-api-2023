test = {}

for i in range(1, 112):
    text = open(f"./open/open_{i}.txt", "r")

    lines = text.readlines()

    dritto = 0
    backwards = 0

    for line in lines:
        if "pianifica-percorso" in line:
            start = line.split()[1]
            end = line.split()[2]
            if start > end:
                dritto += 1
            else:
                backwards += 1
    test[i] = [dritto, backwards]

for el in test:
    print(f"#{el}\nDritti: {test[el][0]} Backwards: {test[el][1]}")