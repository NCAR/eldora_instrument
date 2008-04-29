tools = ['default', 'boost_date_time']
env = Environment(tools = ['default'] + tools)

def dorade(env):
    env.AppendLibrary('dorade')
    env.AppendUnique(CPPPATH = ['#/dorade'])
    env.Require(tools)

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
