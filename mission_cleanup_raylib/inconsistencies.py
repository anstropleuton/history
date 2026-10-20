from dataclasses import dataclass
import os
import re

if not os.getcwd().endswith("examples"):
    print("Run in raylib/examples folder")

@dataclass
class Example:
    path: str
    module: str
    name: str # underscore separated

examples = []

for root, _, files in os.walk("."):
    for file in files:
        path = os.path.join(root, file)
        if path == './examples_template.c':
            continue

        if not file.endswith(".c"):
            continue

        example = Example(path=path, module="", name="")

        file_regex = re.search(r"./(.*?)/(.*).c", path)
        if not file_regex:
            print(f"\tregex did not match ./module/module_name.c (example path: {path})")
            continue

        file_module = file_regex.group(1)
        name = file_regex.group(2)

        if file_module == 'others':
            example.module = 'others'
            example.name = name
            continue

        name_regex = re.search(r"(.*?)_(.*)", name)
        if not name_regex:
            print(f"\tregex did not match module_name")
            continue

        module = name_regex.group(1)
        name = name_regex.group(2)

        if module != file_module:
            print(f"\texample/module does not match ./module/module_name.c: {file_module}, {module}")
            continue

        example.module = module
        example.name = name
        examples.append(example)

for example in examples:
    with open(example.path, "r"):
        pass
