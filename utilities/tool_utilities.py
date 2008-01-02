#
# Rules to build ddssvc and export it as a SCons tool
#
import os

tools = []
env = Environment(tools = ['default'] + tools)

def utilities(env):
    env.AppendLibrary('utilities')
    env.AppendUnique(CPPPATH = ['#/utilities',])
    env.Require(tools)

Export('utilities')


# sources
sources = Split("""
ArgvParams.cpp
SignalCatcher.cpp
""")

utilities = env.Library('utilities', sources)

Default(utilities)
