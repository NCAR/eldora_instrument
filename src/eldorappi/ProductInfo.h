#ifndef PRODUCTINFO_H_
#define PRODUCTINFO_H_

#include <string>

/// Keep track of the configuration information for each 
/// product type.
class ProductInfo {
public:
    /// Default constructor.
    ProductInfo();
    /// Useful constructor.
    ProductInfo(int id,             ///< An id value.
        int userData,           ///< Used for whatever desired by the client.
        std::string key,        ///< An alphanumeric key.
        std::string shortName,  ///< A short name.
        std::string longName,   ///< A long name.
        std::string colorMapName, ///< The name of the color map
        double scaleMin,        ///< The minimum for the colormap scale.
        double scaleMax         ///< The maximum for the colormap scale.
        );
    /// Destructor.
    virtual ~ProductInfo();
    /// Get the id.
    /// @return The id.
    int getId();
    /// Get the userData.
    /// @return The user data.
    int getUserData();
    /// Get the key.
    /// @return The key.
    std::string getKey();
    /// Set the color scale extrema.
    /// @param min The minimum value.
    /// @param max The maximum value.
    void setScale(double min, double max);
    /// Set the colormap name.
    /// @param mapName The colormap name.
    void setColorMapName(std::string mapName);
    /// Get the colormap name.
    /// @return The name.
    std::string getColorMapName();
    /// Get the color scale minimum.
    /// @return The minimum.
    double getScaleMin();
    /// Get the color scale maximum.
    /// @return The maximum.
    double getScaleMax();
    /// Get the short name.
    /// @return The short name.
    std::string getShortName();
    /// Get the long name.
    /// @return The long name.
    std::string getLongName();

protected:
    /// The id
    int _id;
    /// The user data.
    int _userData;
    /// The key.
    std::string _key;
    /// The short name.
    std::string _shortName;
    /// The long name.
    std::string _longName;
    /// The color scale minimum.
    double _scaleMin;
    /// The color scale maximum.
    double _scaleMax;
    /// The color map name.
    std::string _colorMapName;
};

#endif /*PPIINFO_H_*/
