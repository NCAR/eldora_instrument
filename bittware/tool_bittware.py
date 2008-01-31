#
# Rules to build rr314 and export it as a SCons tool
#
import os

def bittware(env):
    # redrapids include location:
    #pidsDevelDir = os.path.join('#', 'redrapids', 
    #                             'DSK-320-002-R05 (CA C API and Sample Code)', 
    #                             'channeladapterlib', 'R05')
    #redRapidsDevelLibDir = os.path.join(redRapidsDevelDir, 'linux_x86-32')
    #env.AppendUnique(CPPPATH=[redRapidsDevelDir, ])
    #
    # The bittware library
    env.AppendUnique(CPPPATH = ['#/bittware',])
    env.AppendLibrary('rr314')
    #
    # redrapids required defines:
    #env.AppendUnique(CPPDEFINES = ['LINUX', ])
    #
    # channel adapter library paths:
    #redRapidsDevelLibDir = os.path.join(redRapidsDevelDir, 'linux_x86-32')
    #env.AppendUnique(LIBPATH = [redRapidsDevelLibDir, '.'])
    #
    # channel adapter lib:
    #libs = Split(['channeladapter'])
    #env.Append(LIBS = libs)
    #
    #
    tools = ['utilities']
    env.Require(tools)

Export('bittware')

# The redrapids and library sources

tools = ['utilities']
env = Environment(tools = ['default'] + tools)

#redRapidsDevelDir = os.path.join('#', 'redrapids', 
#                                 'DSK-320-002-R05 (CA C API and Sample Code)', 
#                                 'channeladapterlib', 'R05')
#redRapidsDevelLibDir = os.path.join(redRapidsDevelDir, 'linux_x86-32')
#env.AppendUnique(CPPPATH=[redRapidsDevelDir, ])
#env.AppendUnique(CPPDEFINES = ['LINUX', ])

#rrsources = [os.path.join(redRapidsDevelDir, 'ca_functions.c'), 
#             os.path.join(redRapidsDevelDir, 'ca_membuffer.c')]

libsources = Split("""
Bittware.cpp
""")

libbittware = env.Library('bittware', libsources)

Default(libbittware)
