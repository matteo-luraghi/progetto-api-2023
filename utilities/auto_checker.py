import os, time

i = 1
while(i < 101):
    print(f"Running script on file #{i}")
    os.system(f"/usr/bin/time ./30 < ./open/open_{i}.txt > out.txt")
    time.sleep(1)
    print("Differences:")
    os.system(f"diff ./open/open_{i}.output.txt ./out.txt")
    time.sleep(1)
    i += 1