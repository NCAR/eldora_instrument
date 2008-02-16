#
# Rules to build rr314 and export it as a SCons tool
#
import os

tools = ['utilities']
env = Environment(tools = ['default'] + tools)

rr314dir = env.Dir('.').srcnode().abspath

redRapidsDevelDir = os.path.join(rr314dir, 
                                 'DSK-320-002-R05 (CA C API and Sample Code)', 
                                 'channeladapterlib', 'R05')
redRapidsDevelLibDir = os.path.join(redRapidsDevelDir, 'linux_x86-32')
env.AppendUnique(CPPPATH=[redRapidsDevelDir, ])
env.AppendUnique(CPPDEFINES = ['LINUX', ])

rrsources = [os.path.join(redRapidsDevelDir, 'ca_functions.c'), 
             os.path.join(redRapidsDevelDir, 'ca_membuffer.c')]

libsources = Split("""
FilterSpec.cpp
RR314.cpp
RR314isr.cpp
RR314sim.cpp
""")

librr314 = env.Library('rr314', rrsources + libsources)

Default(librr314)

def rr314(env):
    # redrapids include location:
    env.AppendUnique(CPPPATH=[rr314dir, redRapidsDevelDir])
    #
    # redrapids required defines:
    env.AppendUnique(CPPDEFINES = ['LINUX', ])
    #
    # channel adapter library paths:
    env.AppendUnique(LIBPATH = [rr314dir, redRapidsDevelLibDir])
    #
    # rr314 and channeladapter libraries:
    libs = ['rr314', 'channeladapter']
    env.Append(LIBS = libs)
    #
    tools = ['utilities']
    env.Require(tools)

Export('rr314')
