#ifndef WIDGETCAPPI_H
#define WIDGETCAPPI_H

#ifndef DLL_EXPORT
#ifdef WIN32
#ifdef QT_PLUGIN
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif
#else
#define DLL_EXPORT
#endif
#endif

#include <string>
#include <vector>
#include <QDialog>
#include <QGLWidget>
#include <QResizeEvent>
#include <QImage>
#include <QTimer>
#include <QRubberBand>
#include <QPoint>

#include "ColorMap.h"
#include <boost/date_time/posix_time/posix_time.hpp>
using boost::posix_time::ptime;
using boost::posix_time::seconds;

/// A Qt widget that will display a Constant Altitude  (CAPPI)
/// display. 
///
/// A beam is the basic building block for the CAPPI. It has
/// a starting angle, ending angle, a fixed number of range 
/// gates, and a fixed number of variables that can be rendered 
/// on the beam.
///
/// The CAPPI is given color maps for each of the variables. 
/// It is then simply called with the data for all variables
/// for a given beam. GL display lists are created for each
/// of the variables, and the display list for the curently
/// selected variable is drawn. When the selected variable
/// is changed, the image is cleared, and all of the display lists
/// for all of the beams, for the newly selected variable,
/// are drawn. This allows or fairly quick (but not 
/// blazing) switches between variables.
///
/// The user specifies the number of gates along each beam, 
/// and the distance span of each beam. The latter is used to 
/// to create range rings in real world units.
///
/// By default, the CAPPI will display a full 360 degree sector. 
/// This sector may be clipped by specifying the clipping plane
/// locations as parameters to configure()
/// 
/// Internally (in the GL space), CAPPI uses a coordinate system
/// which places the display in the region x,y between -1.0 and 1.0.
/// As mentioned above, clipping planes can be specified to establish
/// what part of this region is displayed in the window.
///
/// The CAPPI can operate in a mode where the number of beams
/// are specified, and are preallocated. Incoming data are sent to
/// the nearest preallocated beam.
/// It has another mode, where the beams are created on the
/// fly, so that varying beam widths will be displayed 
/// more accurately. This requires a culling of covered beams,
/// and the constant generation of beam vertices, and so the
/// performance is significantly slower than in the preallocated
/// mode.
///
/// The graphics presentation is modeled as a 2D square box with the 
/// X-Y extents of +/1 1.0. The beams are drawn from the center (0,0)
/// of the box. Normalized beams have the OpenGL vertices (read "expensive
/// trigonomtric calcuations") compute only once.
///
/// Zooming and panning are accomplished by changing the limits of
/// the orthographic projection. To pan, the projection boundaries are 
/// adjusted in the specified directions. To achieve a desired zoom
/// factor, the extent of the boundaries are narrowed.
/// The _centerX and _centerY variables track where the 
/// center of the protetion is currently focused, in the coordinates
/// of the modelview. Mouse selected zooming will move the center of the 
/// rubberband box to the center of the display. This amounts to both a 
/// pan and a zoom.
///
/// It took a lot of fooling around before finally settling on the 
/// described method for panning and zooming. It turns out quite simple
/// and works well. The model provides the description of the display,
/// and the projection is used to visualize a desired region of the model.
/// first attempts used scaling and translation to move the projection around, 
/// but this was complicated since the scaling is always based on the 
/// distance from the origin. Changing the projection boundaries was
/// conceptually simpler.
class DLL_EXPORT CAPPI : public QGLWidget {

Q_OBJECT
	// must include this if you use Qt signals/slots

	/// Manage static data for one display beam that can
	/// render multiple variables.
	/// For the specified solid angle and number
	/// of gates:
	///
	/// - a vector of vertices is created.
	/// - nVars vectors, each of length matching the vertices vector,
	///   is allocated.
	/// - a vector of display list ids is allocated. These ids are
	///   used later when display lists are drawn.
	/// 
	/// The colors for each variable will be set dynamically
	/// by the owner as data need to be rendered. At that time, 
	/// a display list is also created (by the beam owner)
	/// that uses the display list id. By having display lists
	/// created for all variables, the display of beams can be
	/// rapidly switched between variables just by executing
	/// the correct display list.
	class beam {
    public:
		/// Constructor for ray type beams
    	/// @param timetag Timetag of the beam, used for culling
		/// @param span Display span.
		/// @param bx horizontal dist from center of display ()
		/// @param by vertical dist from center of display()
		/// @param gateWidth width of a gate, in 
		/// @param startAngle Start angle of the beam
		/// @param stopAngle Stop angle of the beam
		/// @param nGates Number of gates in the beam
		/// @param nVars  Number of variables for the beam
		beam(ptime timetag, double span, double bx, double by, double gateWidth, double startAngle,
				double stopAngle, int nGates, int nVars);

		/// Constructor for strip type beams
		/// @param timetag Tmetag of the beam, used for culling
		/// @param span Display span.
		/// @param bx horizontal dist from center of display ()
		/// @param by vertical dist from center of display()
		/// @param gateWidth width of a gate, in 
		/// @param angle angle of the beam
		/// @param nGates Number of gates in the beam
		/// @param nVars  Number of variables for the beam
		/// @param stripWidth The width of the strip in 
		beam(ptime timetag, double span, double bx, double by, double gateWidth, double angle,
				int nGates, int nVars, double stripWidth);

		/// destructor
		virtual ~beam();

		/// @param span Display span in .
		void rayGeometry(double span, double bx, double by, double gateWidth, double startAngle, double stopAngle);
		
		/// @param span Display span in .
		void stripGeometry(double span, double bx, double by, double gateWidth, double angle, double stripWidth);

		/// remove al existing beams from the display
		void emptyDisplay();
		
		/// Vector of display list ids; one per variable
		std::vector<GLuint> _glListId;
		/// Left end point store of segment(dynamic beams only).
		double _c;
		/// Right end point store of segment(dynamic beams only).
		double _d;
		/// If beam is hidden (dynamic beams only).
		bool _hidden;
		/// Number of variables in this beam.
		int _nVars;
		/// Number of gates in this beam.
		int _nGates;
		/// @return pointer to the array of triangle strip vertices. This
		/// will be used to generate the display list for the beam.
		GLfloat* vertices();
		/// @return pointer to the array of triangle strip colors for
		/// variable varN. This will be used to generate the display 
		/// lists for the beam.
		GLfloat* colors(int varN);
        // The beam timetag
        ptime _timetag;
        protected:
		/// internal storage for the colors for each variable.
		std::vector<std::vector<GLfloat> > _varColors;
		/// internal storage for the beam triangle strip vertices.
		std::vector<GLfloat> _triStripVertices;
	};
    ///////////////////////////////////////////////////////////////////////////////////
	
public:
	/// Constructor
	CAPPI(QWidget* parent = 0 ///< parent widget
			);
	/// Destructor
	virtual ~CAPPI();
	/// Configure the CAPPI for dynamically allocated beams. 
	/// @param nvars Number of data variables.
	/// @param maxGates Maximum number of gates in a beam
	/// @param gatewidth The width of each gate
	/// @param xorigin The x origin offset to be used for labeling
	/// @param yorigin The y origin offset to be used for labeling
	/// @param timespan If greater than zero, beams will be culled 
	/// if they are older than this value
	void configure(int nVars, 
			int maxGates,
			double span, ///< The distance spanned by the complete CAPPI.
			double gateWidth,
			double xorigin, 
			double yorigin,
			seconds timespan
			);
	/// Select the variable to display.
	void selectVar(int index ///< Index of the variable to display, zero based.
			);
	/// Clear the specified variable.
	void clearVar(int index ///< Index of the variable to be cleared, zero based
			);
	/// Apply data and color maps to an existing beam. See addBeam().
	void fillColors(beam* beam, ///< The beam.
			std::vector<std::vector<double> >& beamData, ///< Vectors of data, one for each variable
			int gates, ///< The number of gates (must match beamData vector sizes).
			int stride, ///< The stride of the data in each vector.
			std::vector<ColorMap*>& maps ///< Colormaps, one for each variable.
			);
	/// Add a new beam to the display in ray mode. Data for all variables and all gates are provided,
	/// as well as color maps for all variables. addBeam() will map the variable values to 
	/// the correct color, and create a wedge of triangle strips, one for each variable.
	/// The existing wedge for this beam will be discarded.
	void addBeam(ptime timetag,
			double xPos, ///< xposition for beam
			double yPos, ///< yposition for beam
			float startAngle, ///< The starting angle for the beam.
			float stopAngle, ///< The ending angle for the beam.
			int gates, ///< The number of gates (must match beamData vector sizes). 
			std::vector<std::vector<double> >& beamData, ///< Vectors of data, one for each variable
			int stride, ///< The stride of the data in each vector.
			std::vector<ColorMap*>& maps ///< Colormaps, one for each variable.
			);
	
	/// Add a new beam to the display in strip mode. Data for all variables and all gates are provided,
	/// as well as color maps for all variables. addBeam() will map the variable values to 
	/// the correct color, and create a wedge of triangle strips, one for each variable.
	/// The existing wedge for this beam will be discarded.
	void addBeam(ptime timetag,
			double xPos, ///< xposition for beam
			double yPos, ///< yposition for beam
			float angle, ///< The  angle for the beam.
			int gates, ///< The number of gates (must match beamData vector sizes). 
			std::vector<std::vector<double> >& beamData, ///< Vectors of data, one for each variable
			int stride, ///< The stride of the data in each vector.
			std::vector<ColorMap*>& maps, ///< Colormaps, one for each variable.
			double stripWidth ///< The width of the strip
			);

	/// if _timespan > 0, remove beams that are older than
	/// the youngest beam - timespan. Paint the removed beams with the
	/// background color.
	void cullbeams();
	/// remove all beams and repaint
	void clear();
	void fillBeams(std::vector<beam*>& newBeams, 
			int gates,
			std::vector<std::vector<double> >& _beamData, 
			int stride,
			std::vector<ColorMap*>& maps);
	/// Set the beam colors all to the background color, 
	/// for the currently seleced variable. This is done 
	/// so that the beam can be redawn in order to remove it from the display.
	void backgroundBeam(beam* beam);
	
	void clearDisplay();
	/// Specify the background color
	void backgroundColor(QColor color ///< The background color.
			);
	/// Specify the background color
	void gridColor(QColor color ///< The grid color.
			);
	/// Find the index in the _beams array of the 
	/// beam that corresponds to this angle. The
	/// beam angles must sweep in a counter clockwise,
	/// i.e. cartessian, direction.
	inline int beamIndex(double startAngle, ///< Beginning angle of the beam. 
			double stopAngle); ///< Ending angle of the beam.
	/// Set grids visibility.
	void grids(bool enabled ///< True to show them, false otherwise
			);
	/// @return The current zoom factor
	double getZoom();
	/// @return The current number of beams. This is interesting
	/// to monitor when CAPPI is operating in the dynamically
	/// allocated beam mode.
	int numBeams();
	/// refresh the display. Useful after resizes, etc.
	void refresh();
	/// Capture an mage of the display
	/// @returns The image. The caller must delete it when finished with it.
	QImage* getImage();
	/// Capture a pixmap of the display
	/// @returns The pixmap. The caller must delete it when finished with it.
	QPixmap* getPixmap();

public slots:
	/// Zoom the display to the specified zoom level.
	/// @param factor The desired final magnification 
	/// value. Note that this is the total maginification. 
	/// Thus if the display were already zoomed at 8x maginification,
	/// and factor were specifed as 16, then a 2x additional 
	/// magnification would be performed.
	void setZoom(double factor);
	/// Move the display by the specified amount.
	/// The parameters specify the percentage of the
	/// existing window that we wish to pan.
	/// @param deltax The normalized delta x value 
	/// @param deltay The normalized delta y value 
	void pan(double deltax, double deltay);
	///
	void resizeTimerTimeout();
	///
	void resetView();
	/// select zooming with the cursor
	void cursorZoom();
	/// select panning with the cursor
	void cursorPan();
	
signals:
    /// Emit when the grid spacing is established.
    /// @param spacing The distance between grid lines
	void gridDelta(double spacing);

protected:
	///
	void initializeGL();
	///
	void resizeGL(int w, int h);
	///
	void paintGL();
	/// Create a display list for the given variable in the beam.
	/// @param b The beam.
	/// @param varN The variable number
	void makeDisplayList(CAPPI::beam* b, int varN);
	/// Handle a resize event. A time is used to prevent refreshes until
	/// the resize is finished.
	virtual void resizeEvent(QResizeEvent * e);
	/// capture mouse move event for panning/zooming
	virtual void mouseMoveEvent(QMouseEvent* event);
	/// capture mouse press event which signals the start of panning/zooming
	virtual void mousePressEvent(QMouseEvent* event);
	/// Perform display panning in response to a mouse motion event.
	void mousePan(QMouseEvent* event);
	/// capture mouse release event which signals the start of panning/zooming
	virtual void mouseReleaseEvent(QMouseEvent* event);
	/// Create the display lists for the grid. The current
	/// value of _gridColor will be used for the color.
	void makeGrids();
	/// Dtermine a grid spacing which will give even distances,
	/// and fit a reasonable number of grid lines in the display
	double gridSpacing();
	/// Print the current display navigation factors.
	void dump();
	/// Pointers to all of the active beams are saved here.
	std::vector<beam*> _beams;
	/// The number of variables we are representing. A display list
	/// will be created for each variable in each beam.
	int _nVars;
	/// Maximum number of gates along a beam
	int _maxGates;
	/// 
	double _gateWidth;
	/// The timespan for beam culling
	seconds _timespan;
	/// The maximum timetag in the beam list
	ptime _maxtime;
	/// The index of the variable selected for display
	int _selectedVar;
	/// The display list id for the grid
	GLuint _gridListId;
	// The current zoom factor. as the zoom in increases, it will
	// increase. At full zoom out, it is equal to 1.
	double _zoom;
	/// The model x coordinate that will be at the center of the view
	double _centerX;
	/// The model y coordinate that will be at the center of the view
	double _centerY;
	/// The color for the grid
	QColor _gridColor;
	/// The color for the back ground
	QColor _backgroundColor;
	/// True if the grids display is enabled.
	bool _gridsEnabled;
	/// A timer used to block redraws while the component is being resized.
	QTimer _resizeTimer;
	/// set when resizing is active; used in conjunction
	/// with a timer in order to prevent many refresh cycles
	/// during the resize.
	bool _resizing;
	/// The distance across the 1/2 of display.
	/// It affects position scaling and the labelling of the grid
	double _span;
	/// Last X location of the mouse during mouse move events; used for panning
	int _oldMouseX;
	/// Last Y location of the mouse during mouse move events; used for panning
	int _oldMouseY;
	/// Set false until we have been configured with some operating paramters.
	bool _configured;
	/// X coordinate for the left clipping plane. Recall that the full disk falls in +/- 1.0
	double _left;
	/// X coordinate for the left clipping plane. Recall that the full disk falls in +/- 1.0
	double _right;
	/// Y coordinate for the lower clipping plane. Recall that the full disk falls in +/- 1.0
	double _bottom;
	/// Y coordinate for the upper clipping plane. Recall that the full disk falls in +/- 1.0
	double _top;
	/// Rubber band for zooming
	QRubberBand* _rubberBand;
	/// The rubber band origin
	QPoint _rubberBandOrigin;
	/// Set true if the mouse motion effects zooming, otherwise it is used for panning
	bool _cursorZoom;
	double _xorigin;
	double _yorigin;
	
};

#endif
