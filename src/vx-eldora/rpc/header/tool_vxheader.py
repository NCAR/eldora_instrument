tools = ['gcc68k']
env = Environment(tools = ['default'] + tools)

sources = Split("""
Header.cc
""")

rpcdefs = Split("""
HeaderRpc.x
""")

headers = Split("""
Header.h
TapeHeader.h
""")

headers += env.RPCGenHeader(rpcdefs)

env.Install("../include", headers)

env.AppendUnique(CPPPATH = ['../include'])
l = env.Library('VxHeader', sources)
Default(l)

def vxheader(env):
    for t in tools:
        env.Tool(t)
    env.AppendUnique(CPPPATH = ['../include'])
    env.AppendLibrary('VxHeader')
    
Export('vxheader')
