with(open("./archivio_test_aperti/open_111.txt", "r")) as f:
    text = f.readlines()

stazioni = []

for line in text:
    if "aggiungi-stazione" in line:
        stazione = line.split(' ')[1]
        if stazione not in stazioni:
            stazioni.append(stazione)
    if "demolisci-stazione" in line:
        stazione = line.split(' ')[1]
        if stazione in stazioni:
            index = stazioni.index(stazione)
            stazioni.pop(index)

print(len(stazioni))