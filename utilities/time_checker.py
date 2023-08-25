from threading import Thread
import os

def callgrind(i):
    os.system(f"valgrind --tool=callgrind ./test < ./open/open_{i}.txt > ./out/output{i}.txt")

for i in range(100,111):
    print("Running on file:",i)
    Thread(target=callgrind(i)).start()
    