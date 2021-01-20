import reg_parse_v2 as r

def test_GetVersion():
    hive = r.Hive("SYSTEM.hive")

    assert hive.GetVersion() == '1.5'

def test_hive_major():
    hive = r.Hive("SYSTEM.hive")
    assert hive.major == 1

def test_hive_minor():
    hive = r.Hive("SYSTEM.hive")
    assert hive.minor == 5

def test_getValue():
    hive = r.Hive("SYSTEM.hive")

    value = hive.GetValue("CsiTool-CreateHive-{00000000-0000-0000-0000-000000000000}/ControlSet001/Control/", "LastBootSucceeded")
    assert value.name == "LastBootSucceeded"
    assert int(value.type) == 4

def test_getValues():
    hive = r.Hive("SYSTEM.hive")

    values = hive.GetValues("CsiTool-CreateHive-{00000000-0000-0000-0000-000000000000}/ControlSet001/Control/")
    assert len(values) == 12
    assert values[-1].name == "DirtyShutdownCount"
    assert int(values[-1].type) == 4

def test_getSubkeys():
    hive = r.Hive("SYSTEM.hive")

    subkeys = hive.GetSubkeys("CsiTool-CreateHive-{00000000-0000-0000-0000-000000000000}/ControlSet001/Control/")
    print(subkeys)

if __name__=="__main__":
    test_GetVersion()
    test_hive_major()
    test_hive_minor()
    test_getValue()
    test_getValues()
    test_getSubkeys()

    print("Tests passed successfully")