 # -*- coding: utf-8 -*-
import re

with open('ettoday.rec') as IN:
    for line in IN.readlines():
        if '@B' in line:
            result = re.split('，|？|。|！', line[3:])
            with open('result', 'a+') as OUT:
                result = list(filter(lambda x: x != '' or x != '\n', result))
                OUT.write('\n'.join(result))