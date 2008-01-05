#
# Rules to build ddssvc and export it as a SCons tool
#
import os

tools = ['svninfo']
env = Environment(tools = ['default'] + tools)

# create the svnInfo.h
env.SvnInfo('svnInfo.h', '#')

def utilities(env):
    env.AppendLibrary('utilities')
    env.AppendUnique(CPPPATH = ['#/utilities',])
    env.Require(tools)

Export('utilities')


# sources
sources = Split("""
ArgvParams.cpp
SignalCatcher.cpp
SvnVersion.cpp
""")

utilities = env.Library('utilities', sources)

Default(utilities)
