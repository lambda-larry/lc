#!/usr/bin/env python

import sys
import re

TOC_PAT = r'^(///?|#{1,3}) (.*?) *(\[[A-Z0-9_]+\]) \1$'
TOC_RE = re.compile(TOC_PAT)

filename = sys.argv[1]

lines = []

with open(filename) as f:
    lines = f.readlines()


sections = []
for i, line in enumerate(lines):
    match TOC_RE.match(line):
        case None:
            continue

        case Match as match:
            sections.append({
                "comment": match[1],
                "title": match[2],
                "shortcode": match[3],
                "line": i,
            })

max_line = len(lines)
for section in sections[::-1]:
    tmp = section['line']
    section['line'] = max_line - section['line']
    max_line = tmp


def fmt_section(section):
    LINE_LENGHT = 80
    padding = LINE_LENGHT - 7

    comment = section['comment']
    title = section['title']
    shortcode = section['shortcode']
    line_nr = str(section['line'])

    padding -= len(shortcode)
    padding -= len(title)
    padding -= len(comment)
    padding -= len(line_nr)

    padding = '.' * padding

    return f'{comment} - {title} {padding}.{line_nr}. {shortcode}\n'


def replace_toc(lines, sections):
    inside_toc = False
    injected = False
    ret = []

    for idx, line in enumerate(lines):
        if not injected and inside_toc:
            ret.extend(map(fmt_section, sections))
            injected = True

        if "toc_end" in line:
            inside_toc = False

        if not inside_toc:
            ret.append(line)

        if "toc_begin" in line:
            inside_toc = True

    return ret


lines = replace_toc(lines, sections)

with open(filename, 'w') as f:
    f.write(''.join(lines))
