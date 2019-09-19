class Chemical:
    def __init__(self, name, chemdict):
        self.name = name
        self.melting = chemdict[name]["melting_point"]
        self.boiling = chemdict[name]["boiling_point"]
        self.soluble = chemdict[name]["soluble"]
        self.element = chemdict[name]["element"]
        self.parent = chemdict[name]["parent"]
        self.state = None
        self.change_states(20, False)

    def change_states(self, temp, in_water):
        if (self.soluble and in_water) and (0 < temp < 100):
            self.state = "aqueous"
        else:
            self.state = "solid"

            if temp >= self.melting:
                self.state = "liquid"

            if temp >= self.boiling:
                self.state = "gaseous"


def load_chemicals():
    from json import load
    with open("Resources/chemicals.json", 'r') as r:
        chemdict = load(r)
    chemlist = list()
    for name in chemdict.keys():
        chemlist.append(Chemical(name, chemdict))
    return chemlist
