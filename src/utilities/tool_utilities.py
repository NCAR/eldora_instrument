#
# Rules to build the general ELDORA utilities library and export it
# as a SCons tool
#
import os

tools = ['default', 'svninfo', 'xmlrpc']
env = Environment(tools = tools)

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
RPCServer.cpp
TimetagUtils.cpp
EldoraRadarParams.cpp
""")

utilities = env.Library('utilities', sources)
Default(utilities)

