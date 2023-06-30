import os, time

i = 1
while(i < 101):
    print(f"Running script on file #{i}")
    os.system(f"./main-hash < ./open/open_{i}.txt > out.txt")
    time.sleep(1.5)
    print("Differences:")
    os.system(f"diff ./open/open_{i}.output.txt ./out.txt")
    time.sleep(3)
    i += 1