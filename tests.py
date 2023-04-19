import reg_parse_v2 as r

def test_GetVersion():
    hive = r.Hive("hives/SYSTEM")

    assert hive.GetVersion() == '1.5'

def test_hive_major():
    hive = r.Hive("hives/SYSTEM")
    
    assert hive.major == 1

def test_hive_minor():
    hive = r.Hive("hives/SYSTEM")

    assert hive.minor == 5

def test_getValue():
    hive = r.Hive("hives/SYSTEM")

    value = hive.GetValue("CsiTool-CreateHive-{00000000-0000-0000-0000-000000000000}/ControlSet001/Control/", "LastBootSucceeded")
    assert value.name == "LastBootSucceeded"
    assert int(value.type) == 4

def test_getValues():
    hive = r.Hive("hives/SYSTEM")

    values = hive.GetValues("CsiTool-CreateHive-{00000000-0000-0000-0000-000000000000}/ControlSet001/Control/")
    assert len(values) == 11

def test_getSubkeys():
    hive = r.Hive("hives/SYSTEM")
    subkeys = hive.GetSubkeys("CsiTool-CreateHive-{00000000-0000-0000-0000-000000000000}/ControlSet001/Control/")
    assert subkeys[1].name == "ACPI"
    assert len(subkeys) == 127

def test_getParent():
    hive = r.Hive("hives/SYSTEM")

    subkeys = hive.GetSubkeys("CsiTool-CreateHive-{00000000-0000-0000-0000-000000000000}/ControlSet001/Control/")
    for i in subkeys:
        assert subkeys[subkeys.index(i)].parent.name == "Control"

def test_getDate():
    filetimestamp = 0
    months = {
        '1': 31,
        '2': 28,
        '3': 31,
        '4': 30,
        '5': 31,
        '6': 30,
        '7': 31,
        '8': 31,
        '9': 30,
        '10': 31,
        '11': 30,
        '12': 31,}
    year = 1601
    month = 1
    day = 1

    for i in range(0, int((2021-1601)*365.2425)):
        filetimestamp+=8.64e11
        day+=1
        if (day > months[str(month)]):
            # print(day, month, year, year%4)
            if ((((year%4 == 0) & (year%100!=0))| (year%400==0)) & (day==29) & (month == 2)):
                continue
            else:
                day = 1
                month+=1
        if (month == 13):
            month = 1
            year+=1

        date = r.getDate(int(filetimestamp))
        # print(f"{day}-{month}-{year} ?=", date, filetimestamp)

        date_split = list(map(int, date.split("-")))
        assert date_split[2] == year 
        assert date_split[1] == month 
        assert date_split[0] == day

if __name__=="__main__":
    test_GetVersion()
    test_hive_major()
    test_hive_minor()
    test_getValue()
    test_getValues()
    test_getSubkeys()

    test_getDate()

    print("Tests passed successfully")