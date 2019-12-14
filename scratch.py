import std
import statistics as stat 

check_num = [1, 2, 4, 7]

test1 = std.standard_dev(check_num)
test2 = stat.stdev(check_num)

print(f"Stats std num {test1}")
print(f"C --- std num {test2}")

import sumC
sumC.uni_reg([1, 2, 3, 4], [1, 2, 3])
sumC.uni_reg([1, 2, 3, 4], [1, 2, 3, 4])
sumC.uni_reg([1, 2, 3, 4], [1, 2, 3, 4], eta = 10, epochs = 100)
sumC.uni_reg([1, 2, 3, 4], [1, 2, 3, 4], eta = -1, epochs = 100)
sumC.uni_reg([1, 2, 3, 4], [1, 2, 3, 4], eta = 0.05, epochs = 1000)
