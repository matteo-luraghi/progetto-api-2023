import os, time

i = 1
while(i < 101):
    print(f"Running script on file #{i}")
    os.system(f"./main < ./archivio_test_aperti/open_{i}.txt > out.txt")
    time.sleep(1.5)
    print("Differences:")
    os.system(f"diff ./archivio_test_aperti/open_{i}.output.txt ./out.txt")
    time.sleep(3)
    i += 1
    os.system("clear")