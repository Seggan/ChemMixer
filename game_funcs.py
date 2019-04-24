def codify(text):
    fi=text
    fi=fi.strip()
    fi=fi.lower()
    return fi
class Beaker():
    """A class to represent a beaker for the game"""
    def __init__(self):
        from json import load
        self.chemicals=list()
        reactions=dict()
        with open("reactions.json",'r') as r:
            self.reactions=load(r)
        self.cont=False
    def add(self,chemical,chemdict):
        """Adds a chemical"""
        if codify(chemical) in chemdict.keys():
            self.chemicals.append(codify(chemical))
        else:
            self.cont=True
    def extract(self,chemi):
        """Removes a chemical"""
        if chemi:
            if chemi in self.chemicals:
                self.chemicals.remove(chemi)
            else:
                self.cont=True
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
            s2=set(self.chemicals)
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
        for chemical in self.chemicals:
            print(chemdict[chemical]["state"],chemical)

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
        
        
                  
    
        
    
            
