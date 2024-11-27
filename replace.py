import re

pattern = r"(out(w|b))\((.*), (.*)\);"

matches = re.findall(pattern, "outw(0x3c, 0x55); outb(0x3c, 0x55);")

function = matches[0][0]
arg1 = matches[0][2]
arg2 = matches[0][3]

print(f"{function}({arg2}, {arg1});")

print(matches[0][3])
