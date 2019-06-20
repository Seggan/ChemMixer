def dict_reduce(parent_dict, child_dict):
    for key in child_dict.keys():
        del parent_dict[key]
    return parent_dict


def amount(obj, array):
    num = 0
    for entry in array:
        if entry == obj:
            num += 1
    return num


def sublist(child, parent):
    parent_dict = {entry: amount(entry, parent) for entry in parent}
    child_dict = {entry: amount(entry, child) for entry in child}
    for entry in child_dict.keys():
        if entry in parent_dict.keys():
            if child_dict[entry] < parent_dict[entry]:
                return False
        else:
            return False
    return True


class Beaker():
    """A class to represent a beaker"""

    def __init__(self, chemdict):
        from json import load
        self.dc = [chem for chem in chemdict.keys() if chemdict[chem]["element"]]
        self.contents = list()
        with open("reactions.json", 'r') as r:
            self.reactions = load(r)
        self.cont = False

    def add(self, chem):
        self.contents.append(chem)

    def extract(self, chem):
        if chem in self.contents:
            del self.contents[self.contents.index(chem)]

    def reset(self):
        """Empties the beaker"""
        self.contents = list()

    def react(self, temp, chemdict):
        """Checks for reactions and if there are any, reacts them"""
        for a in self.reactions.keys():
            if sublist(self.reactions[a]["reactants"], self.contents) and (
                    self.reactions[a]["temp min"] < temp <= self.reactions[a]["temp max"]):
                for c in self.reactions[a]["reactants"]:
                    self.extract(c)
                for d in self.reactions[a]["results"]:
                    self.add(d)
        for chemical in chemdict.keys():
            if chemical in self.contents:
                if (chemdict[chemical]["soluble"] and "water" in self.contents) and 0 < temp < 100:
                    chemdict[chemical]["state"] = "aqueous"
                else:
                    chemdict[chemical]["state"] = "solid"
                    if temp >= chemdict[chemical]["melting_point"]:
                        chemdict[chemical]["state"] = "liquid"
                    if temp >= chemdict[chemical]["boiling_point"]:
                        chemdict[chemical]["state"] = "gaseous"

    def show_contents(self, temp, chemdict):
        """Shows the contents of the beaker"""
        accounted = list()
        solution = False
        txt = "Your beaker is at " + str(temp) + ''' degrees Celsius and has the
following chemicals inside:'''
        for chemical in self.contents:
            if chemdict[chemical]["state"] == "aqueous":
                solution = True
        for chemical in self.contents:
            if chemical not in accounted:
                count = amount(chemical, self.contents)
                if count == 1:
                    mole = ' mole of '
                else:
                    mole = ' moles of '
                if chemical == "water" and not solution:
                    txt += '\n' + str(count) + mole + chemdict[chemical]["state"] + ' ' + chemical
                if chemical != "water":
                    txt += '\n' + str(count) + mole + chemdict[chemical]["state"] + ' ' + chemical
                accounted.append(chemical)
        return txt

    def refresh(self):
        from json import load
        with open("reactions.json", 'r') as r:
            self.reactions = load(r)


def change_states(temp, chemdict, contents):
    for chemical in chemdict.keys():
        if (chemdict[chemical]["soluble"] and "water" in contents) and 0 < temp < 100:
            chemdict[chemical]["state"] = "aqueous"
        else:
            chemdict[chemical]["state"] = "solid"
            if temp >= chemdict[chemical]["melting_point"]:
                chemdict[chemical]["state"] = "liquid"
            if temp >= chemdict[chemical]["boiling_point"]:
                chemdict[chemical]["state"] = "gaseous"


def mod_load(mod):
    import os
    from json import load, dump
    with open(os.path.join(os.getcwd(), 'Mods', mod + '.json'), 'r') as f:
        d = load(f)
    with open('chemicals.json', 'r') as f:
        c = load(f)
    with open('reactions.json', 'r') as f:
        r = load(f)
    c.update(d["chemicals"])
    r.update(d["reactions"])
    with open('chemicals.json', 'w') as f:
        dump(c, f, indent=4)
    with open('reactions.json', 'w') as f:
        dump(r, f, indent=4)


def mod_unload(mod):
    import os
    from json import load, dump
    with open(os.path.join(os.getcwd(), 'Mods', mod + '.json'), 'r') as f:
        d = load(f)
    with open('chemicals.json', 'r') as f:
        c = load(f)
    with open('reactions.json', 'r') as f:
        r = load(f)
    c = dict_reduce(c, d["chemicals"])
    r = dict_reduce(r, d["reactions"])
    with open('chemicals.json', 'w') as f:
        dump(c, f, indent=4)
    with open('reactions.json', 'w') as f:
        dump(r, f, indent=4)
