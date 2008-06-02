# -*- python -*-
tools = ['gcc68k', 'vxworks']
env = Environment(tools = ['default'] + tools)

sources = Split("""
cio.c
dport.c
mbox.c
""")

env.Append(CPPDEFINES = ['TP41LIB'])
l = env.Library('tp41', sources)

def tp41(env):
    for t in tools:
        env.Tool(t)
    env.AppendLibrary('tp41')

Export('tp41')