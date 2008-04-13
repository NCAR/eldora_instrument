from PyQt4.QtCore  import *

class QtConfig:
    """
    Provide an interface for persistent configuration
    management. Configuration items are specified
    as key:value pairs.
    
    Calls which fetch a configuration value 
    also specify a default value for that key.
    The configuration is written with each fetch,
    so that a configuration will contain the default 
    values if they haven't been set.
    
    Configurations are always synced when a value is written.
    """
###################################################################################
    def __init__(self, organization, application):
        self._settings = QSettings(QSettings.IniFormat, 
                                   QSettings.UserScope, 
                                   organization, 
                                   application)
        
###################################################################################
    def childGroups(self, topGroup):
        """
        Get the configuration groups below the given group.
        @param topGroup Limit list to keys below this group.
        @returns A list of the groups below this key.
        """
        result = []
        
        # set the top group
        self._settings.beginGroup(topGroup)
        
        # get the child groups
        children = self._settings.childGroups()
        
        # save them
        for i in range(len(children)):
            result.append(children[i])
        
        self._settings.endGroup()
        
        return result

###################################################################################
    def setString(self, key, t):
        """
        Set the value of the configuration item.
        @param key Configuration item name.
        @param t Configuration item value.
        """
        self._settings.setValue(key, QVariant(t))
        self.sync();

###################################################################################
    def getString(self, key, defaultValue):
        """
        Fetch the value of the configuration item.
        @param key The configuration item name.
        @param defaultValue The default value assigned to this configuration item.
        @returns The configuration item value. If the configuration item has 
        not yet been defined, it is created with the default value,
        and the default value is returned.
        """
        s = self._settings.value(key, QVariant(defaultValue)).toString()
        self._settings.setValue(key, QVariant(s))
        self.sync()
        return str(s)

###################################################################################
    def setDouble(self, key, value):
        """        
        Set the value of the configuration item.
        @param key Configuration item name.
        @param t Configuration item value.
        """

        self._settings.setValue(key, QVariant(value))
        self.sync();

###################################################################################
    def getDouble(self, key, defaultValue):
        """        
        Fetch the value of the configuration item.
        @param key The configuration item name.
        @param defaultValue The default value assigned to this configuration item.
        @returns The configuration item value. If the configuration item has 
        not yet been defined, it is created with the default value,
        and the default value is returned.
        """

        d = self._settings.value(key, QVariant(defaultValue)).toDouble()[0]
        self._settings.setValue(key, QVariant(d))
        self.sync();
        return d;
    
###################################################################################
    def setFloat(self, key, value):
        """        
        Set the value of the configuration item.
        @param key Configuration item name.
        @param f Configuration item value.
        """
        self._settings.setValue(key, QVariant(value))
        self.sync()

###################################################################################
    def getFloat(self, key, defaultValue):
        """        
        Fetch the value of the configuration item.
        @param key The configuration item name.
        @param defaultValue The default value assigned to this configuration item.
        @returns The configuration item value. If the configuration item has 
        not yet been defined, it is created with the default value,
        and the default value is returned.
        """
        d = self._settings.value(key, QVariant(defaultValue)).toDouble()[0]
        self._settings.setValue(key, QVariant(d))
        self.sync()
        return d;

###################################################################################
    def setInt(self, key, value):
        """        
        Set the value of the configuration item.
        @param key Configuration item name.
        @param i Configuration item value.
        """
        self._settings.setValue(key, QVariant(value))
        self.sync()

###################################################################################
    def getInt(self, key, defaultValue): 
        """        
        Fetch the value of the configuration item.
        @param key The configuration item name.
        @param defaultValue The default value assigned to this configuration item.
        @returns The configuration item value. If the configuration item has 
        not yet been defined, it is created with the default value,
        and the default value is returned.
        """
        i = self._settings.value(key, QVariant(defaultValue)).toInt()[0]
        self._settings.setValue(key, QVariant(i))
        self.sync()
        return i

###################################################################################
    def setBool(self, key, value):
        """        
        Set the value of the configuration item.
        @param key Configuration item name.
        @param b Configuration item value.
        """
        self._settings.setValue(key, QVariant(value))
        self.sync()

###################################################################################
    def getBool(self, key, defaultValue): 
        """        
        Fetch the value of the configuration item.
        @param key The configuration item name.
        @param defaultValue The default value assigned to this configuration item.
        @returns The configuration item value. If the configuration item has 
        not yet been defined, it is created with the default value,
        and the default value is returned.
        """               
        b = self._settings.value(key, QVariant(defaultValue)).toBool()
        self._settings.setValue(key, QVariant(b))
        self.sync()
        return b;
        
###################################################################################
    def sync(self):
        self._settings.sync()
        return
    
###################################################################################
    @staticmethod
    def test():
        """
        A test harness for QtConfig
        """
        c = QtConfig('NCAR', 'testingConfig')
        
        v = c.getString('String', 'astring')
        print v, type(v)
        c.setString('String', 'bstring')
        v = c.getString('String', 'astring')
        print v, type(v)
        
        v = c.getDouble('Double', 10000.1)
        print v, type(v)
        c.setDouble('Double', 20340.0)
        v = c.getDouble('Double', 100.0)
        print v, type(v)
        
        v = c.getFloat('Float', 10000.1)
        print v, type(v)
        c.setFloat('Float', 20340.0)
        v = c.getFloat('Float', 100.0)
        print v, type(v)
        
        v = c.getInt('Int', 100)
        print v, type(v)
        c.setInt('Int', 200)
        v = c.getInt('Int', 100)
        print v, type(v)
        
        v = c.getBool('Bool', 1)
        print v, type(v)
        c.setBool('Bool', 0)
        v = c.getBool('Bool', 1)
        print v, type(v)
                   