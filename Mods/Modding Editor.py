from json import dump
def reduced_input(txt=""):
    text=input(txt)
    return text.strip().lower()
def check_state(meltingp, boilingp):
    if 20>meltingp:
        return "liquid"
    if 20>boilingp:
        return "gaseous"
    if 20<=boilingp:
        return "liquid"
    if 20<=meltingp:
        return "solid"
mod_name=reduced_input("What is the name of your mod?\n").replace(' ','_')
chemdict=dict()
reactions=dict()
n=1
print("You are now entering the chemical editor. Type in \"quit\" to exit the chemical editor.")
while True:
    name=reduced_input("What is the name of chemical "+str(n)+'?\n')
    if name=="quit":
        break
    melting=reduced_input("What is the melting point of chemical "+str(n)+'?\n')
    if melting=="quit":
        break
    melting=int(melting)
    boiling=reduced_input("What is the boiling point of chemical "+str(n)+'?\n')
    if boiling=="quit":
        break
    boiling=int(boiling)
    element=reduced_input("Is chemical "+str(n)+" an element?(y/n)\n")
    if element=="quit":
        break
    element=bool(element=="y")
    room_state=check_state(melting, boiling)
    chemdict[name]={
        "melting_point":melting,
        "boiling_point":boiling,
        "state":room_state,
        "element":element
        }
    n+=1
n=1
print("You are now entering the reaction editor. Type in \"quit\" to exit the reaction editor.")
while True:
    formula=reduced_input("What is the right side of the formula of reaction "+str(n)+'?\n')
    if formula=="quit":
        break
    reactants=reduced_input("What are the reactants of reaction "+str(n)+'''?(type in the lowercase
names of the chemicals seperated by a comma)\n''').split(',')
    reactants=[r.strip() for r in reactants]
    if "quit" in reactants:
        break
    high=reduced_input("What is the highest temperature reaction "+str(n)+' will react?\n')
    if high=="quit":
        break
    high=int(high)
    low=reduced_input("What is the lowest temperature reaction "+str(n)+' will react?\n')
    if low=="quit":
        break
    low=int(low)-1
    products=reduced_input("What are the products of reaction "+str(n)+'''?(type in the lowercase
names of the chemicals seperated by a comma)\n''').split(',')
    products=[p.strip() for p in products]
    if "quit" in products:
        break
    room_state=check_state(melting, boiling)
    reactions[formula]={
        "reactants":reactants,
        "temp max":high,
        "temp min":low,
        "results":products
        }
    n+=1

final={
    "chemicals":chemdict,
    "reactions":reactions
    }
with open(mod_name+'.json', 'w') as f:
    dump(final, f, indent=4)
print("Finished")
