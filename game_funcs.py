def dict_reduce(parent_dict, child_dict):
    for key in child_dict.keys():
        del parent_dict[key]
    return parent_dict
class Beaker():
    """A class to represent a beaker for creative mode"""
    def __init__(self, chemdict):
        from json import load
        self.dc=[chem for chem in chemdict.keys() if chemdict[chem]["element"]]
        self.contents=list()
        with open("reactions.json",'r') as r:
            self.reactions=load(r)
        self.cont=False
    def add(self, chem):
        self.contents.append(chem)
    def extract(self, chem):
        if chem in self.contents:
            del self.contents[self.contents.index(chem)]
    def reset(self):
        """Empties the beaker"""
        self.contents=list()
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
                    self.add(d)
    def show_contents(self,temp,chemdict):
        """Shows the contents of the beaker"""
        txt="Your beaker is at "+str(temp)+''' degrees Celsius and has the
following chemicals inside:'''
        for chemical in self.contents:
            txt+='\n'+chemdict[chemical]["state"]+' '+chemical
        return txt

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
