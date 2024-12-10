from math import sqrt

def formatBin(binVal : str) -> str:
    return binVal[2:].zfill(7)

with open("sqrts.txt", "w") as f:
  for i in range(0, 8192):
     f.write(formatBin(bin(round(sqrt(i))) + "\n"))