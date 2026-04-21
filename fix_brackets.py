import os

filepath = r'c:\Users\elhed\Desktop\2A14\Semestre 2\Projet C++\fin\fin\matieres.cpp'

with open(filepath, 'r', encoding='utf-8') as f:
    lines = f.readlines()

# The methods checkStockAlerts and checkAIQuality were mistakenly put between 1046 and 1164.
to_move = lines[1045:1164]
del lines[1045:1164]

lines.extend(['\n'])
lines.extend(to_move)

with open(filepath, 'w', encoding='utf-8') as f:
    f.writelines(lines)
    
print("Fixed successfully!")
