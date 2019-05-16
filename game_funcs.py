from tkinter import *
def codify(text):
    fi=text
    fi=fi.strip()
    fi=fi.lower()
    return fi
class CreativeBeaker():
    """A class to represent a beaker for creative mode"""
    def __init__(self):
        from json import load
        self.contents=list()
        self.reactions=dict()
        with open("reactions.json",'r') as r:
            self.reactions=load(r)
        self.cont=False
    def add(self,chemical):
        """Adds a chemical"""
        self.contents.append(codify(chemical))
    def extract(self,chemi):
        """Removes a chemical"""
        if chemi:
            self.contents.remove(chemi)
        else:
            print("\n\tSorry, but there is nothing to remove.\n")
    def reset(self):
        """Empties the beaker"""
        for c in range(0,len(self.chemicals)+1):
            self.chemicals=list()
    def react(self,temp,chemdict):
        """Checks for reactions and if there are any, reacts them"""
        tmp=0
        for a in self.reactions.keys():
            s1=set(self.reactions[a]["reactants"])
            s2=set(self.contents)
            if s1.issubset(s2) and (temp>self.reactions[a]["temp min"] and temp<=self.reactions[a]["temp max"]):
                for c in self.reactions[a]["reactants"]:
                    self.extract(c)
                for d in self.reactions[a]["results"]:
                    self.add(d,chemdict)
                print("\nA reaction has occurred.")
    def contents(self,temp,chemdict):
        """Shows the contents of the beaker"""
        print("\nYour beaker is at ",temp,''' degrees Celsius and has the
following chemicals inside:''')
        for chemical in self.contents:
            print(chemdict[chemical]["state"],chemical)
class DiscoveryBeaker(CreativeBeaker):
    def __init__(self):
        super().__init__()
        self.discovered_chemicals=["oxygen","hydrogen","nitrogen","helium",
                                   "sulfur","aluminum","lithium","chlorine"
                                   ]
def change_states(temp,chemdict):
    for chem in chemdict.keys():
        if temp>chemdict[chem]["melting_point"]:
            chemdict[chem]["state"]="liquid"
        if temp>chemdict[chem]["boiling_point"]:
            chemdict[chem]["state"]="gaseous"
        if temp<=chemdict[chem]["boiling_point"]:
            chemdict[chem]["state"]="liquid"
        if temp<=chemdict[chem]["melting_point"]:
            chemdict[chem]["state"]="solid"
