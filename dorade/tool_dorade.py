tools = ['default', 'qt4']
env = Environment(tools = ['default'] + tools)

qt4modules = ['QtCore',]
env.EnableQt4Modules(qt4modules)

def dorade(env):
    env.AppendLibrary('dorade')
    env.AppendUnique(CPPPATH = ['#/dorade'])
    env.Require(tools)
    env.EnableQt4Modules(qt4modules)

Export('dorade')

sources = Split("""
DoradeDescriptor.cpp
DoradeASIB.cpp
DoradeFRAD.cpp
DoradeRYIB.cpp
""")

dorade_lib = env.Library('dorade', sources)
Default(dorade_lib)

# Now build together the test programs. In this tool_*.py scheme,
# these will not get built unless another application requests this tool
testtools = ['default', 'dorade']
testenv = Environment(tools = testtools)

readhskp = testenv.Program('ReadHousekeeping', ['ReadHousekeeping.cpp'])
Default (readhskp)
