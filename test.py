import pefile

pe = pefile.PE('new.exe', fast_load=True)

print(any(b".token" in section.Name for section in pe.sections))
