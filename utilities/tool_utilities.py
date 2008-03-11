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
""")

utilities = env.Library('utilities', sources)
Default(utilities)

# Now build together the test programs. In this tool_*.py scheme,
# these will not get built unless another application requests this tool
testtools = ['default', 'utilities']
testenv = Environment(tools = testtools)

rpcservertest = testenv.Program('rpcservertest', 'RPCServerTest.cpp')
Default (rpcservertest)
