#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include <qtimer.h>
#include <QResizeEvent>
#include <QPalette>
#include <QPaintEngine>
#include <QBrush>
#include <QPen>

#include <GL/glut.h>

#include "CAPPI.h"
//
//
// Any drawing action must insure that the GL context is current. paintGL() and
// resize can be called by QGLWidget, and it will make sure that the context
// is current. However, external calls to zoom, pan, draw a new beam,
// and so forth will not have come through QGLWidget's GL code, and so we need
// to explicitly make sure that the context is current in these cases.
//
//
////////////////////////////////////////////////////////////////

CAPPI::beam::beam(double span, double xpos, double ypos, double gateWidth,
		double startAngle, double stopAngle, int nGates, int nVars) :
	_nVars(nVars), _nGates(nGates) {

	// construct the geometry for a ray presentation
	rayGeometry(span, xpos, ypos, gateWidth, startAngle, stopAngle);

	// Allocate space for the colors. Each vertex has an red, green and
	// blue component, and there are 2 vertices per gate.
	_varColors.resize(nVars);
	for (int v = 0; v < nVars; v++) {
		_varColors[v].resize(_nGates*6);
	}
	// there will be one display list id for each variable
	for (int i = 0; i < _nVars; i++) {
		///@todo add test to insure that the list has been created
		GLuint id = glGenLists(1);
		_glListId.push_back(id);
	}
}

////////////////////////////////////////////////////////////////
CAPPI::beam::beam(double span, double xpos, double ypos, double gateWidth,
		double angle, int nGates, int nVars, double stripWidth) :
	_nVars(nVars), _nGates(nGates) {

	// create the geometry for a strip presentation
	stripGeometry(span, xpos, ypos, gateWidth, angle, stripWidth);

	// Allocate space for the colors. Each vertex has an red, green and
	// blue component, and there are 2 vertices per gate.
	_varColors.resize(nVars);
	for (int v = 0; v < nVars; v++) {
		_varColors[v].resize(_nGates*6);
	}
	// there will be one display list id for each variable
	for (int i = 0; i < _nVars; i++) {
		///@todo add test to insure that the list has been created
		GLuint id = glGenLists(1);
		_glListId.push_back(id);
	}
}

////////////////////////////////////////////////////////////////
void CAPPI::beam::rayGeometry(double span, double xpos, double ypos,
		double gateWidth, double startAngle, double stopAngle) {
	_triStripVertices.clear();
	// compute offset in GL coordinates
	float gl_x = xpos / span;
	float gl_y = ypos / span;

	// calculate the x and y displacement for each 
	// tristrip edge of a single gate

	double scaledGatewidth = gateWidth/span;
	float cos1 = cos(M_PI*startAngle/180.0)*scaledGatewidth;
	float sin1 = sin(M_PI*startAngle/180.0)*scaledGatewidth;
	float cos2 = cos(M_PI*stopAngle/180.0) *scaledGatewidth;
	float sin2 = sin(M_PI*stopAngle/180.0) *scaledGatewidth;

	// now calculate the vertex values, to be used for all variables
	for (int j = 0; j < _nGates; j++) {
		_triStripVertices.push_back(j*cos1 + gl_x);
		_triStripVertices.push_back(j*sin1 + gl_y);
		_triStripVertices.push_back(j*cos2 + gl_x);
		_triStripVertices.push_back(j*sin2 + gl_y);
	}
}

////////////////////////////////////////////////////////////////
void CAPPI::beam::stripGeometry(double span, double xpos, double ypos,
		double gateWidth, double angle, double stripWidth) {

	_triStripVertices.clear();
	// compute offset in GL coordinates
	float gl_x = xpos / span;
	float gl_y = ypos / span;

	// calculate the x and y displacement for each 
	// tristrip edge of a single gate

	double scaledGatewidth = gateWidth/span;

	double xOffset = cos(M_PI*(angle+90.0)/180.0)*stripWidth/2.0/span;
	double yOffset = sin(M_PI*(angle+90.0)/180.0)*stripWidth/2.0/span;

	float cos1 = cos(M_PI*angle/180.0)*scaledGatewidth;
	float sin1 = sin(M_PI*angle/180.0)*scaledGatewidth;

	// now calculate the vertex values, to be used for all variables
	for (int j = 0; j < _nGates; j++) {
		_triStripVertices.push_back(j*cos1 + gl_x - xOffset);
		_triStripVertices.push_back(j*sin1 + gl_y - yOffset);
		_triStripVertices.push_back(j*cos1 + gl_x + xOffset);
		_triStripVertices.push_back(j*sin1 + gl_y + yOffset);
	}
}
////////////////////////////////////////////////////////////////

CAPPI::beam::~beam() {
	for (unsigned int i = 0; i < _glListId.size(); i++)
		glDeleteLists(_glListId[i], 1);

	_glListId.clear();
	_varColors.clear();
	_triStripVertices.clear();
}

////////////////////////////////////////////////////////////////
GLfloat* CAPPI::beam::vertices() {
	return &_triStripVertices[0];
}

////////////////////////////////////////////////////////////////
GLfloat* CAPPI::beam::colors(int varN) {
	return &(_varColors[varN])[0];
}

////////////////////////////////////////////////////////////////
static bool glutInitialized = false;

CAPPI::CAPPI(QWidget* parent) :
	QGLWidget(parent), _selectedVar(0), _zoom(1.0), _centerX(0.0),
			_centerY(0.0), _gridColor("black"),
			_backgroundColor("lightblue"),
			_gridsEnabled(false), _resizing(false),
			_configured(false),
			_left(-1), _right(1), _bottom(-1), _top(1), _rubberBand(0),
			_span(100), _cursorZoom(true),
			_xorigin(0.0), _yorigin(0.0) {
	
	initializeGL();

	if (!glutInitialized) {
		int argc = 1;
		char* argv[2];
		argv[0] = (char*)("dummy");
		argv[1] = 0;

		glutInit(&argc, argv);
		glutInitialized = true;
	}

	this->setAutoBufferSwap(false);

	// create the rubber band
	_rubberBand = new QRubberBand(QRubberBand::Rectangle, this);

	// connect the resize timer
	_resizeTimer.setSingleShot(true);
	connect(&_resizeTimer, SIGNAL(timeout()), this, SLOT(resizeTimerTimeout()));
}
////////////////////////////////////////////////////////////////

void CAPPI::configure(int nVars, int maxGates, double span, double gateWidth,
		double xorigin, double yorigin) {
	// Configure for dynamically allocated beams
	_nVars = nVars;
	_maxGates = maxGates;
	_span = span/2;
	_gateWidth = gateWidth;
	_left = -1.0;
	_right = 1.0;
	_bottom = -1.0;
	_top = 1.0;
	_centerX = _left + (_right-_left)/2.0;
	_centerY = _bottom + (_top-_bottom)/2.0;
	_zoom = 1.0;
	_xorigin = xorigin;
	_yorigin = yorigin;

	_configured = true;

	resetView();
}
////////////////////////////////////////////////////////////////

CAPPI::~CAPPI() {

	// delete all of the dynamically created beams
	for (unsigned int i = 0; i < _beams.size(); i++) {
		delete _beams[i];
	}
}

////////////////////////////////////////////////////////////////

void CAPPI::initializeGL() {
	glClearColor(_backgroundColor.red()/255.0, _backgroundColor.green()/255.0,
			_backgroundColor.blue()/255.0, 0.0f);

	glDrawBuffer(GL_FRONT);
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);

	glShadeModel(GL_FLAT);

	glLineWidth(1.0);

	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_POINT_SMOOTH);
	glDisable(GL_POLYGON_SMOOTH);
	glDisable(GL_DITHER);
	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_INDEX_ARRAY);
	glDisable(GL_EDGE_FLAG_ARRAY);
	glDisable(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_NORMAL_ARRAY);
	glDisableClientState(GL_INDEX_ARRAY);
	glDisableClientState(GL_EDGE_FLAG_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	glEnable(GL_COLOR_ARRAY);
	glEnable(GL_VERTEX_ARRAY);
	glEnable(GL_STENCIL_TEST);

	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	// set the stencil buffer clear value.
	glClearStencil((GLint)0);

	// get a display list id for the grids
	_gridListId = glGenLists(1);
	// get a display list id for the grid
	_gridListId = glGenLists(1);
}

////////////////////////////////////////////////////////////////

void CAPPI::resizeGL(int w, int h) {

	// setup viewport, projection etc.:
	glViewport( 0, 0, (GLint)w, (GLint)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(_left, _right, _bottom, _top);
	glMatrixMode(GL_MODELVIEW);

	_centerX = _left + (_right-_left)/2.0;
	_centerY = _bottom + (_top-_bottom)/2.0;
	_zoom = 1.0;

	dump();
}

////////////////////////////////////////////////////////////////

void CAPPI::paintGL() {
	if (_resizing) {
		// clear the image
		glClear(GL_COLOR_BUFFER_BIT);
		return;
	}

	// draw into the back buffer
	glDrawBuffer(GL_BACK);

	// clear the display
	glClear(GL_COLOR_BUFFER_BIT);

	// redraw the beams
	for (unsigned int i = 0; i < _beams.size(); i++) {
		glCallList(_beams[i]->_glListId[_selectedVar]);
	}

	// draw grid
	if (_gridsEnabled) {
		//createStencil();
		makeGrids();
		if (_gridsEnabled)
			glCallList(_gridListId);
	}

	// display the back buffer
	swapBuffers();

	// and resume drawing to the front buffer.
	glDrawBuffer(GL_FRONT);
}

////////////////////////////////////////////////////////////////
void CAPPI::grids(bool enabled) {
	_gridsEnabled = enabled;
	//redraw
	makeCurrent();
	paintGL();
}

////////////////////////////////////////////////////////////////
void CAPPI::dump() {

	// remove the return statement when you need to debug PPI
	return;

	std::cout << "_centerX:" << _centerX << "  _centerY:" << _centerY
			<< "   zoom:" << _zoom << "\n";
}

////////////////////////////////////////////////////////////////
void CAPPI::setZoom(double factor) {

	makeCurrent();

	// if the zoom request is to go smaller than 1:1, 
	// restore to centered normal display
	if (factor <= 1.0) {
		_centerX = _left + (_right-_left)/2.0;
		_centerY = _bottom + (_top-_bottom)/2.0;
		_zoom = 1.0;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(_left, _right, _bottom, _top);
		glMatrixMode(GL_MODELVIEW);

	} else {
		// determine the new size of the projection.
		// It will retain its current center position.
		_zoom = factor;
		double w = (_right-_left);
		double h = (_top-_bottom);
		double l = _centerX - w/_zoom/2.0;
		double r = _centerX + w/_zoom/2.0;
		double b = _centerY - h/_zoom/2.0;
		double t = _centerY + h/_zoom/2.0;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(l, r, b, t);
		glMatrixMode(GL_MODELVIEW);

	}

	// redraw
	paintGL();

	dump();

	return;
}

////////////////////////////////////////////////////////////////
double CAPPI::getZoom() {
	return _zoom;
}

////////////////////////////////////////////////////////////////

void CAPPI::refresh() {
	if (_resizing)
		return;
	//redraw
	makeCurrent();
	glClear(GL_COLOR_BUFFER_BIT);
	paintGL();
}
////////////////////////////////////////////////////////////////

void CAPPI::pan(double deltax, double deltay) {
	makeCurrent();

	// move the center of the projection.
	_centerX = _centerX + deltax*(_right-_left)/_zoom;
	_centerY = _centerY + deltay*(_top-_bottom)/_zoom;

	// determine new locations for the projection boundaries
	double w = (_right-_left);
	double h = (_top-_bottom);
	double l = _centerX - w/_zoom/2.0;
	double r = _centerX + w/_zoom/2.0;
	double b = _centerY - h/_zoom/2.0;
	double t = _centerY + h/_zoom/2.0;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(l, r, b, t);
	glMatrixMode(GL_MODELVIEW);

	// redraw
	paintGL();

	dump();
}

////////////////////////////////////////////////////////////////
void CAPPI::resetView() {

	makeCurrent();

	resizeGL(width(), height());

	// redraw
	paintGL();

	return;
}

////////////////////////////////////////////////////////////////
void CAPPI::resizeEvent(QResizeEvent * e) {

	if (_resizing) {
		makeCurrent();
		glClear(GL_COLOR_BUFFER_BIT);
		return;
	}

	_resizing = true;

	_resizeTimer.start(500);
}

////////////////////////////////////////////////////////////////
void CAPPI::mousePressEvent(QMouseEvent * e) {
	if (_cursorZoom) {
		_rubberBand->setGeometry(QRect(e->pos(), QSize()));
		_rubberBand->show();
	}

	_oldMouseX = e->x();
	_oldMouseY = e->y();

}

////////////////////////////////////////////////////////////////
void CAPPI::mouseMoveEvent(QMouseEvent * e) {

	if (_cursorZoom) {
		// zooming with the mouse
		int x = e->x();
		int y = e->y();
		int deltaX = x - _oldMouseX;
		int deltaY = y - _oldMouseY;
		// get the current window aspec ratio
		double wAspect = ((double)width())/height();
		// make the rubberband aspect ratio match that
		// of the window
		double dx;
		double dy;
		dx = fabs(deltaY*wAspect);
		dy = fabs(dx/wAspect);
		// preserve the signs
		dx *= fabs(deltaX)/deltaX;
		dy *= fabs(deltaY)/deltaY;
		QRect newRect = QRect(_oldMouseX, _oldMouseY, (int)dx, (int)dy);

		newRect = newRect.normalized();
		_rubberBand->setGeometry(newRect);

	} else {
		// panning with the mouse
		mousePan(e);
	}
}

////////////////////////////////////////////////////////////////
void CAPPI::mouseReleaseEvent(QMouseEvent * e) {
	if (_cursorZoom) {

		// we are zooming with the mouse
		makeCurrent();

		QRect g = _rubberBand->geometry();

		// find the center of the rubber band box in QRect coordinates,
		// but with y running from bottom to top
		double avgX = g.x() + g.width()/2.0;
		double avgY = g.y() + g.height()/2.0;

		// normalize to -0.5:0.5, sine we are panning to the 
		// center of the display.
		double newX = avgX/width() - 0.5;
		double newY = 0.5 -avgY/height();

		// translate by this amount
		pan(newX, newY);

		// calculate new zoom. Base it on the largest edge of 
		// the rubberband box
		double newZoom = 1.0;
		int deltaX = g.width();
		int deltaY = g.height();
		if (deltaX > 0 || deltaY > 0) {
			double zoomX = deltaX / (double)width();
			double zoomY = deltaY / (double)height();
			if (zoomX >= zoomY) {
				newZoom = 1.0/zoomX;
			} else {
				newZoom = 1.0/zoomY;
			}
		}

		// scale by this amount
		setZoom(newZoom*_zoom);

		// hide the rubber band
		_rubberBand->hide();

	} else {

		// we are panning with the mouse
		mousePan(e);
	}
}

////////////////////////////////////////////////////////////////
void CAPPI::mousePan(QMouseEvent * e) {
	int x = e->x();
	int y = e->y();

	double deltaX = -(_oldMouseX - x);
	double deltaY = -(y - _oldMouseY);

	_oldMouseX = x;
	_oldMouseY = y;

	// Convert delta distances to our model coordinates
	// 0.0 to 1.0 of the window, across each axis
	deltaX = deltaX/width();
	deltaY = deltaY/height();

	pan(-deltaX, -deltaY);
}

////////////////////////////////////////////////////////////////
void CAPPI::resizeTimerTimeout() {
	makeCurrent();
	resizeGL(this->width(), this->height());
	_resizing = false;
	refresh();
}

////////////////////////////////////////////////////////////////

int CAPPI::numBeams() {
	return _beams.size();
}

////////////////////////////////////////////////////////////////////////

void CAPPI::selectVar(int index) {
	_selectedVar = index;
	updateGL();
	return;
}

////////////////////////////////////////////////////////////////////////

void CAPPI::clearVar(int index) {
	if (index >= _nVars)
		return;

	// calling makeDisplayList with data == 0 causes the display list to 
	// be drawn completely with the background color.
	float r = _backgroundColor.red()/255.0;
	float g = _backgroundColor.green()/255.0;
	float b = _backgroundColor.blue()/255.0;

	for (unsigned int i = 0; i < _beams.size(); i++) {
		int cIndex = 0;
		GLfloat* colors = _beams[i]->colors(index);
		for (int gate = 0; gate < _maxGates; gate++) {
			colors[cIndex++] = r;
			colors[cIndex++] = g;
			colors[cIndex++] = b;
			colors[cIndex++] = r;
			colors[cIndex++] = g;
			colors[cIndex++] = b;
		}
	}

	if (index == _selectedVar) {
		selectVar(index);
	}

}

////////////////////////////////////////////////////////////////

void CAPPI::addBeam(double xPos, double yPos, float startAngle,
		float stopAngle, int gates,
		std::vector<std::vector<double> >& _beamData, int stride,
		std::vector<ColorMap*>& maps) {

	makeCurrent();

	// add a new beam to the display. 
	// The steps are:
	// 1. preallocate mode: find the beam to be drawn, or dynamic mode:
	//    create the beam(s) to be drawn.
	// 2. fill the colors for all variables in the beams to be drawn
	// 3. make the display list for the selected variables in the beams
	//    to be drawn.
	// 4. call the new display list(s)

	beam* b;
	std::vector<beam*> newBeams;

	// the start and stop angle MUST specify a counterclockwise fill for the sector. Thus 
	// if startAngle > stopAngle, we know that we have crossed the 0 boundary, and must
	// break it up into 2 beams.

	// create the new beam(s), to keep track of the display information
	// Beam start and stop angles are adjusted here so that they always 
	// increase counterclockwise. 

	startAngle = startAngle - ((int)(startAngle/360.0))*360.0;
	stopAngle = stopAngle - ((int)(stopAngle/360.0))*360.0;

	if (startAngle <= stopAngle) {

		b
				= new beam(_span, xPos, yPos, _gateWidth, startAngle, stopAngle, _maxGates, _nVars);
		_beams.push_back(b);
		newBeams.push_back(b);
	} else {
		b
				= new beam(_span, xPos, yPos, _gateWidth, startAngle, 360.0, _maxGates, _nVars);
		_beams.push_back(b);
		newBeams.push_back(b);

		b
				= new beam(_span, xPos, yPos, _gateWidth, 0.0, stopAngle, _maxGates, _nVars);
		_beams.push_back(b);
		newBeams.push_back(b);
	}

	fillBeams(newBeams, gates, _beamData, stride, maps);

}
////////////////////////////////////////////////////////////////

void CAPPI::addBeam(double xPos, double yPos, float angle, int gates,
		std::vector<std::vector<double> >& _beamData, int stride,
		std::vector<ColorMap*>& maps, double stripWidth) {

	makeCurrent();

	// add a new beam to the display. 
	// The steps are:
	// 1. preallocate mode: find the beam to be drawn, or dynamic mode:
	//    create the beam(s) to be drawn.
	// 2. fill the colors for all variables in the beams to be drawn
	// 3. make the display list for the selected variables in the beams
	//    to be drawn.
	// 4. call the new display list(s)

	beam* b;
	std::vector<beam*> newBeams;

	// the start and stop angle MUST specify a counterclockwise fill for the sector. Thus 
	// if startAngle > stopAngle, we know that we have crossed the 0 boundary, and must
	// break it up into 2 beams.

	// create the new beam(s), to keep track of the display information
	// Beam start and stop angles are adjusted here so that they always 
	// increase counterclockwise. 

	angle = angle - ((int)(angle/360.0))*360.0;
	b
			= new beam(_span, xPos, yPos, _gateWidth, angle, _maxGates, _nVars, stripWidth);

	_beams.push_back(b);

	newBeams.push_back(b);

	fillBeams(newBeams, gates, _beamData, stride, maps);

}

////////////////////////////////////////////////////////////////
void CAPPI::fillBeams(std::vector<beam*>& newBeams, int gates,
		std::vector<std::vector<double> >& _beamData, int stride,
		std::vector<ColorMap*>& maps) {

	beam* b;

	// newBeams has collected the beams to be rendered; now fill in 

	// their colors and draw them
	for (unsigned int i = 0; i < newBeams.size(); i++) {
		b = newBeams[i];
		fillColors(b, _beamData, gates, stride, maps);

		for (int v = 0; v < _nVars; v++) {
			makeDisplayList(b, v);
		}
	}

	// draw it
	for (unsigned int i = 0; i < newBeams.size(); i++) {
		b = newBeams[i];
		if (!_resizing)
			glCallList(b->_glListId[_selectedVar]);
	}

	// draw the grids if they are enabled. Don't worry,
	// it is only a display list call. 
	if (_gridsEnabled)
		glCallList(_gridListId);

	if (!_resizing)
		glFlush();

}

////////////////////////////////////////////////////////////////
void CAPPI::fillColors(beam* beam,
		std::vector<std::vector<double> >& _beamData, int gates, int stride,
		std::vector<ColorMap*>& maps) {

	float red, green, blue;
	for (int v = 0; v < _nVars; v++) {

		ColorMap* map = maps[v];
		GLfloat* colors = beam->colors(v);
		int cIndex = 0;

		double* varData = &(_beamData[v][0]);
		for (int gate = 0; gate < gates; gate++) {
			double data = varData[gate];
			map->dataColor(data, red, green, blue);
			colors[cIndex++] = red;
			colors[cIndex++] = green;
			colors[cIndex++] = blue;
			colors[cIndex++] = red;
			colors[cIndex++] = green;
			colors[cIndex++] = blue;
		}
		float r = _backgroundColor.red()/255.0;
		float g = _backgroundColor.green()/255.0;
		float b = _backgroundColor.blue()/255.0;

		for (int gate = gates; gate < _maxGates; gate++) {
			colors[cIndex++] = r;
			colors[cIndex++] = g;
			colors[cIndex++] = b;
			colors[cIndex++] = r;
			colors[cIndex++] = g;
			colors[cIndex++] = b;
		}
	}
}

////////////////////////////////////////////////////////////////
void CAPPI::clearDisplay() {
	for (unsigned int i = 0; i < _beams.size(); i++) {
		delete _beams[i];
	}
	
	_beams.clear();
	
}

////////////////////////////////////////////////////////////////
void CAPPI::makeDisplayList(beam* b, int v) {
	//	glGenBuffers();

	// create a display list to hold the gl commands
	glNewList(b->_glListId[v], GL_COMPILE);

	// set the vertex pointer
	glVertexPointer(2, GL_FLOAT, 0, b->vertices());

	// set the colors pointer
	glColorPointer(3, GL_FLOAT, 0, b->colors(v));

	// draw a triangle strip. 
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 2*_maxGates);

	// end the display list
	glEndList();

}

////////////////////////////////////////////////////////////////////////

int CAPPI::beamIndex(double startAngle, double stopAngle) {
	int i = (int)(_beams.size()*(startAngle + (stopAngle-startAngle)/2)/360.0);
	if (i<0)
		i = 0;
	if (i>(int)_beams.size()-1)
		i = _beams.size()-1;

	return i;
}

////////////////////////////////////////////////////////////////////////
void CAPPI::backgroundColor(QColor color) {
	_backgroundColor = color;
	glClearColor(_backgroundColor.red()/255.0, _backgroundColor.green()/255.0,
			_backgroundColor.blue()/255.0, 0.0f);

	makeCurrent();
	updateGL();
}
////////////////////////////////////////////////////////////////////////
void CAPPI::gridColor(QColor color) {
	_gridColor = color;

	makeCurrent();
	updateGL();
}

////////////////////////////////////////////////////////////////////////
void CAPPI::makeGrids() {

	// don't try to draw grids if we haven't been configured yet
	if (!_configured)
		return;

	// or if the grids aren't enabled
	if (!_gridsEnabled)
		return;

	// grid spacing
	double delta = gridSpacing();

	// tell the world about the grid spacing
emit 	gridDelta(delta);

	double lineWidth = 0.004/ _zoom;
	GLdouble x;
	GLdouble y;
	
	// do the grid
	if (delta > 0 && _gridsEnabled) {

		// create a display list to hold the gl commands
		glNewList(_gridListId, GL_COMPILE);

		// set the color
		glColor3f(_gridColor.red()/255.0, _gridColor.green()/255.0,
				_gridColor.blue()/255.0);

		glLineWidth(1);

		glBegin(GL_LINES);
		// First the vertical lines.
		// set the first x value
		x = -2.0;
		while (x <= 2.0) {
			glVertex2d(x, -2.0);
			glVertex2d(x, 2.0);
			x += delta/_span;
		}
		// Now horizontial lines
		// set the first y value to an even increment of the grid spacing.
		y = -2.0;
		while (y <= 2.0) {
			glVertex2d(-2.0, y);
			glVertex2d( 2.0, y);
			y += delta/_span;
		}
		glEnd();

		// label the grid
		if (delta > 0.0) {
			for (x = -2.0; x <= 2.0; x += delta/_span) {
				for (y = -2.0; y <= 2.0; y += delta/_span) {

					std::ostringstream xlabel;
					std::ostringstream ylabel;
					const char* c;

					xlabel << std::fixed << std::setprecision(1) << x*_span + _xorigin;
					ylabel << std::fixed << std::setprecision(1) << y*_span + _yorigin;

					c = ylabel.str().c_str();
					glRasterPos2d(x+0.01/_zoom, y+0.01/_zoom);
					while (*c)
						glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,*c++);

					c = xlabel.str().c_str();
					glRasterPos2d(x-0.12/_zoom, y-0.045/_zoom);
					while (*c)
						glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,*c++);

				}
			}
		}

		glEndList();
	}
}

////////////////////////////////////////////////////////////////////////
double CAPPI::gridSpacing() {

	// R is the visible distance from center to edge
	double R = 2.0*(_span / _zoom);
	double e = (int)floor(log10(R));
	double Rn = R / pow(10.0, e);

	double delta = 2.0;
	if (Rn <= 5.0) {
		delta = 1.0;
	}
	if (Rn <= 3.0) {
		delta = 0.5;
	}
	if (Rn <= 1.0) {
		delta = 0.2;
	}
	if (Rn <= 0.5) {
		delta = 0.1;
	}

	delta = delta * pow(10.0, e);
	delta = delta;

	return delta;

}
////////////////////////////////////////////////////////////////////////
QImage* CAPPI::getImage() {
	makeCurrent();
	updateGL();
	glReadBuffer(GL_FRONT);
	QImage* pImage = new QImage(grabFrameBuffer(true));
	return pImage;
}

////////////////////////////////////////////////////////////////////////
QPixmap* CAPPI::getPixmap() {
	QPixmap* pImage = new QPixmap(renderPixmap());
	return pImage;
}

////////////////////////////////////////////////////////////////////////
void CAPPI::cursorZoom() {
	_cursorZoom = true;
}

////////////////////////////////////////////////////////////////////////
void CAPPI::cursorPan() {
	_cursorZoom = false;
}
