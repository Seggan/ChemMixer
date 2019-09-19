def dict_reduce(parent_dict, child_dict):
    for key in child_dict.keys():
        try:
            del parent_dict[key]
        except KeyError:
            pass
    return parent_dict


def mod_load(mod):
    import os
    from json import load, dump
    with open(os.path.join(os.getcwd(), 'Mods', mod + '.json'), 'r') as f:
        d = load(f)
    with open('Resources/chemicals.json', 'r') as f:
        c = load(f)
    with open('Resources/reactions.json', 'r') as f:
        r = load(f)
    c.update(d["chemicals"])
    r["reactions"].update(d["reactions"])
    with open('Resources/chemicals.json', 'w') as f:
        dump(c, f, indent=2)
    with open('Resources/reactions.json', 'w') as f:
        dump(r, f, indent=2)


def mod_unload(mod):
    import os
    from json import load, dump
    with open(os.path.join(os.getcwd(), 'Mods', mod + '.json'), 'r') as f:
        d = load(f)
    with open('Resources/chemicals.json', 'r') as f:
        c = load(f)
    with open('Resources/reactions.json', 'r') as f:
        r = load(f)
    c = dict_reduce(c, d["chemicals"])
    r["reactions"] = dict_reduce(r["reactions"], d["reactions"])
    with open('Resources/chemicals.json', 'w') as f:
        dump(c, f, indent=2)
    with open('Resources/reactions.json', 'w') as f:
        dump(r, f, indent=2)
