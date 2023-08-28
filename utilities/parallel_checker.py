import subprocess
import os
l=[]
for i in range(1,101):
    print("Running on file:",i)
    s="./open/open_"+str(i)+".txt"
    input=open(s,"r")
    output=open(f"./out/output{i}.txt","w")
    proc=subprocess.run("./30",stdin=input,stdout=output)
    output.close()

    s="./open/open_"+str(i)+".output.txt"
    proc=subprocess.run(["diff",s,f"./out/output{i}.txt"],capture_output=True)
    if proc.stdout.decode() != "":
        l.append(i)
    #os.system("clear")

print(l)
print(len(l))