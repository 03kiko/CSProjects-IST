from pulp import *

firstLine = input()
firstLine = firstLine.split(" ")
items = int(firstLine[0])
bundles = int(firstLine[1])
capacity = int(firstLine[2])
info = []

for i in range(0, items):
    line = input()
    line = line.split(" ")
    info.append((int(line[0]), int(line[1]), [i])) # (profit, capacity, dependencies)

for i in range(items, items + bundles):
    line = input()
    line = line.split(" ")
    info[int(line[0]) - 1][2].append(i) # for each item in the bundle
    info[int(line[1]) - 1][2].append(i) # add the bundle as a dependency
    info[int(line[2]) - 1][2].append(i) # on the item
    info.append((int(line[3]),)) # profit of the bundle

prob = LpProblem("UbiquityInc", LpMaximize) # create problem

# create variables
vars = LpVariable.dict("vars", range(0, items + bundles), 0, None, LpInteger)

# objective function: vars[i] * profit[i] for each variable
prob += lpSum([vars[i] * info[i][0] for i in vars]), "objective"

# capacity restriction of each item, including bundles depending
# on that item, for instance: if item x has a capacity of 30
# and there is only one bundle y which needs item x
# then this will model x + y <= 30
for i in range(0, items):
    prob += lpSum([vars[j] for j in info[i][2]]) <= info[i][1]

# total capacity restriction of all items (bounded by factory capacity)
# each single item has a coefficient of 1 and a bundle a coefficient of 3
# so for 3 items and 1 bundle with a total capacity of 300 it would
# model: x1 + x2 + x3 + 3*b1 <= 300
prob += lpSum([vars[i] * (1 if i < items else 3) for i in vars]) <= capacity

# this will write all information to a file if you so wish
#prob.writeLP("lp.debug")

prob.solve()
