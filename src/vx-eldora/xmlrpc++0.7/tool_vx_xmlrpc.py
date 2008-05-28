tools = ['gcc68k']
env = Environment(tools = ['default'] + tools)
# Remove -lpthread added by global tool Eldora
if ('-lpthread' in env['_LIBFLAGS']):
    env['_LIBFLAGS'].remove('-lpthread')
env.Append(CPPPATH = ['src'])

sources = Split("""
src/XmlRpcClient.cpp
src/XmlRpcDispatch.cpp
src/XmlRpcServer.cpp
src/XmlRpcServerConnection.cpp
src/XmlRpcServerMethod.cpp
src/XmlRpcSocket.cpp
src/XmlRpcSource.cpp
src/XmlRpcUtil.cpp
src/XmlRpcValue.cpp
""")

libxmlrpc = env.Library('src/XmlRpc', sources)
Default(libxmlrpc)

tooldir = env.Dir('src')

def vx_xmlrpc(env):
    for t in tools:
        env.Tool(t)
    # Remove -lpthread added by global tool Eldora
    if ('-lpthread' in env['_LIBFLAGS']):
        env['_LIBFLAGS'].remove('-lpthread')
    env.Append(LIBPATH = [tooldir])
    env.Append(CPPPATH = [tooldir])
    env.AppendLibrary('XmlRpc')
    
Export('vx_xmlrpc')
