#!/usr/bin/env/python3

import os
import sys
import typing

THIS_SCRIPT_DIR = sys.path[0]
INCLUDE_DIR = THIS_SCRIPT_DIR + "/../src/include/cleantype/"
ALL_IN_ONE_FILE = THIS_SCRIPT_DIR + "/include/cleantype/cleantype.hpp"

MAIN_TITLE = """
// CleanType : amalgamated version
//
// This file is part of CleanType: Clean Types for C++
// Copyright Pascal Thomet - 2018
// Distributed under the Boost Software License, Version 1.0. (see LICENSE.md)
"""

TITLE = """
//////////////////////////////////////////
////   Header: HEADER_FILE
//////////////////////////////////////////
"""

FileName = typing.NewType("Filename", str)
FileContent = typing.NewType("FileContent", str)
CodeLine = typing.NewType("CodeLine", str)


def read_file(file: FileName) -> FileContent:
    with open(file, "r") as f:
        r = f.read()
        return r


def get_included_file(codeline: CodeLine) -> typing.Optional[FileName]:
    search = "#include <cleantype/"
    if search in codeline:
        included_file = codeline.replace(search, "")
        included_file = included_file[:-1]
        return included_file
    else:
        return None


def process_header_file(header_file: FileName, already_processed_headers: [FileName] = []) -> FileContent:
    if header_file in already_processed_headers:
        return ""
    already_processed_headers.append(header_file)
    dst_file_content = TITLE.replace("HEADER_FILE", header_file)
    original_file_content = read_file(INCLUDE_DIR + header_file)
    for line in original_file_content.split("\n"):
        included_file = get_included_file(line)
        if included_file is not None:
            dst_file_content = dst_file_content + \
                process_header_file(included_file, already_processed_headers)
        else:
            if "#pragma once" not in line:
                dst_file_content = dst_file_content + line + "\n"
    return dst_file_content

def make_all_in_one() -> str:
    c = process_header_file("cleantype.hpp")
    c = "#pragma once\n" + MAIN_TITLE + c
    return c


if __name__ == "__main__":
    content = make_all_in_one()
    # print(make_all_in_one())
    with open(ALL_IN_ONE_FILE, "w") as f:
        f.write(content)
    print("{} was created".format(ALL_IN_ONE_FILE))
