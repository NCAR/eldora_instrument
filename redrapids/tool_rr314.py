#
# Rules to build rr314 and export it as a SCons tool
#
import os

def rr314(env):
    # redrapids include location:
    redRapidsDevelDir = os.path.join('#', 'redrapids', 
                                 'DSK-320-002-R05 (CA C API and Sample Code)', 
                                 'channeladapterlib', 'R05')
    redRapidsDevelLibDir = os.path.join(redRapidsDevelDir, 'linux_x86-32')
    env.AppendUnique(CPPPATH=[redRapidsDevelDir, ])
    #
    # The rr314 library
    env.AppendUnique(CPPPATH = ['#/redrapids',])
    env.AppendLibrary('rr314')
    #
    # redrapids required defines:
    env.AppendUnique(CPPDEFINES = ['LINUX', ])
    #
    # channel adapter library paths:
    redRapidsDevelLibDir = os.path.join(redRapidsDevelDir, 'linux_x86-32')
    env.AppendUnique(LIBPATH = [redRapidsDevelLibDir, '.'])
    #
    # channel adapter lib:
    libs = Split(['channeladapter'])
    env.Append(LIBS = libs)
    #
    # The rr314 library mentioned again, due to the circular dependency
    # on Adapter_ISR between channellib and librr314
    env.AppendUnique(CPPPATH = ['#/redrapids',])
    env.AppendLibrary('rr314')
    #
    tools = ['utilities']
    env.Require(tools)

Export('rr314')

# The redrapids and library sources

tools = ['utilities']
env = Environment(tools = ['default'] + tools)

redRapidsDevelDir = os.path.join('#', 'redrapids', 
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
