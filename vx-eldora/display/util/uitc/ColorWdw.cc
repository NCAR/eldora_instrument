/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 ColorWdw.cc
 *	Original Author: Richard E. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include <stream.h>

#include "ColorWdw.hh"

ColorWdw::ColorWdw(int argc, char **argv, char *file) : Cmap(DYNAMIC)
{
    Base.initUI(argc, argv);
    Base.setWidth(558);
    Base.setHeight(448);
    Base.setBackgroundColor("Dark Turquoise");
    Base.setLabel("Colormap  Tool");
    Base.show(TRUE);
    Base.setDisplayFooter(TRUE);
    Base.setResizable(TRUE);
    Base.setNoConfirm(FALSE);

    Cntrl.setX(0);
    Cntrl.setY(0);
    Cntrl.setExtendToEdge(WIDTH);
    Cntrl.setHeight(388);
    Cntrl.setBackgroundColor("Wheat");
    Cntrl.setDisplayBorders(FALSE);
    Base.addDisplay(Cntrl);

    FileName.setX(16);
    FileName.setY(8);
    FileName.setInputDisplayLength(12);
    FileName.setMaxInputLength(256);
    FileName.setLabel("File:");
    FileName.setLabelPosition(LEFT);
    FileName.setReadOnly(FALSE);
    Cntrl.addComponent(FileName);

    SaveButton.setX(192);
    SaveButton.setY(8);
    SaveButton.setLabel("Save");
    SaveButton.setNotifyHandler((UICHandlerFunction)ColorWdw::SaveHandler);
    setThis(SaveButton);
    Cntrl.addComponent(SaveButton);
    
    LoadButton.setX(272);
    LoadButton.setY(8);
    LoadButton.setLabel("Load");
    LoadButton.setNotifyHandler((UICHandlerFunction)ColorWdw::LoadHandler);
    setThis(LoadButton);
    Cntrl.addComponent(LoadButton);
    
    BrowseButton.setX(348);
    BrowseButton.setY(8);
    BrowseButton.setLabel("Browse");
    BrowseButton.setNotifyHandler((UICHandlerFunction)ColorWdw::BrowseHandler);
    setThis(BrowseButton);
    Cntrl.addComponent(BrowseButton);
    
    Quit.setX(504);
    Quit.setY(8);
    Quit.setLabel("Exit");
    Quit.setNotifyHandler((UICHandlerFunction)ColorWdw::QuitHandler);
    setThis(Quit);
    Cntrl.addComponent(Quit);
    
    ColorBins.setMultipleChoice(FALSE);
    ColorBins.setX(16);
    ColorBins.setY(68);
    ColorBins.setDisplayedRows(1);
    ColorBins.setLabelPosition(LEFT);
    ColorBins.setForegroundColor("Black");
    ColorBins.setSelectionRequired(TRUE);
    ColorBins.setLabel("Color Set:");
    ColorBins.setNotifyHandler((UICHandlerFunction)ColorWdw::ColorBinsHandler);
    setThis(ColorBins);
    (void) ColorBins.addChoice("1");
    (void) ColorBins.addChoice("2");
    (void) ColorBins.addChoice("3");
    Cntrl.addComponent(ColorBins);
    
    RgbMsg.setX(12);
    RgbMsg.setY(112);
    RgbMsg.setLabel("RGB Values:");
    RgbMsg.setLabelBold(TRUE);
    Cntrl.addComponent(RgbMsg);

    RedSlider.setX(16);
    RedSlider.setY(148);
    RedSlider.setSliderWidth(256);
    RedSlider.setTicks(25);
    RedSlider.setForegroundColor("Red");
    RedSlider.setLabel("Red:");
    RedSlider.setLabelPosition(LEFT);
    RedSlider.setOrientation(HORIZONTAL);
    RedSlider.setDisplayEndBoxes(TRUE);
    RedSlider.setDisplayRange(TRUE);
    RedSlider.setDisplayValue(TRUE);
    RedSlider.setMinValue(0);
    RedSlider.setMaxValue(255);
    RedSlider.setNotifyHandler((UICHandlerFunction)ColorWdw::ColorHandler);
    setThis(RedSlider);
    Cntrl.addComponent(RedSlider);
    
    GreenSlider.setX(16);
    GreenSlider.setY(180);
    GreenSlider.setSliderWidth(256);
    GreenSlider.setTicks(25);
    GreenSlider.setForegroundColor("Yellow Green");
    GreenSlider.setLabel("Green:");
    GreenSlider.setLabelPosition(LEFT);
    GreenSlider.setOrientation(HORIZONTAL);
    GreenSlider.setDisplayEndBoxes(TRUE);
    GreenSlider.setDisplayRange(TRUE);
    GreenSlider.setDisplayValue(TRUE);
    GreenSlider.setMinValue(0);
    GreenSlider.setMaxValue(255);
    GreenSlider.setNotifyHandler((UICHandlerFunction)ColorWdw::ColorHandler);
    setThis(GreenSlider);
    Cntrl.addComponent(GreenSlider);
    
    BlueSlider.setX(16);
    BlueSlider.setY(216);
    BlueSlider.setSliderWidth(256);
    BlueSlider.setTicks(25);
    BlueSlider.setForegroundColor("Blue");
    BlueSlider.setLabel("Blue:");
    BlueSlider.setLabelPosition(LEFT);
    BlueSlider.setOrientation(HORIZONTAL);
    BlueSlider.setDisplayEndBoxes(TRUE);
    BlueSlider.setDisplayRange(TRUE);
    BlueSlider.setDisplayValue(TRUE);
    BlueSlider.setMinValue(0);
    BlueSlider.setMaxValue(255);
    BlueSlider.setNotifyHandler((UICHandlerFunction)ColorWdw::ColorHandler);
    setThis(BlueSlider);
    Cntrl.addComponent(BlueSlider);
    
    UndoButton.setX(16);
    UndoButton.setY(248);
    UndoButton.setLabel("Undo");
    UndoButton.setNotifyHandler((UICHandlerFunction)ColorWdw::UndoHandler);
    setThis(UndoButton);
    Cntrl.addComponent(UndoButton);
    
    ColorIndex.setX(16);
    ColorIndex.setY(300);
    ColorIndex.setSliderWidth(93);
    ColorIndex.setTicks(0);
    ColorIndex.setLabel("Color Number:");
    ColorIndex.setLabelPosition(LEFT);
    ColorIndex.setOrientation(HORIZONTAL);
    ColorIndex.setDisplayEndBoxes(TRUE);
    ColorIndex.setDisplayRange(TRUE);
    ColorIndex.setDisplayValue(TRUE);
    ColorIndex.setMinValue(0);
    ColorIndex.setMaxValue(30);
    ColorIndex.setNotifyHandler((UICHandlerFunction)ColorWdw::ColorIndexHandler);
    setThis(ColorIndex);
    Cntrl.addComponent(ColorIndex);
    
    ColorMsg.setX(184);
    ColorMsg.setY(348);
    ColorMsg.setLabel("Approximate  color values -");
    ColorMsg.setLabelBold(TRUE);
    Cntrl.addComponent(ColorMsg);
    
    TheColor.setX(0);
    TheColor.setY(388);
    TheColor.setExtendToEdge(WIDTH);
    TheColor.setHeight(60);
    TheColor.setDrawingModel(X11);
    TheColor.setDynamic(TRUE);
    TheColor.setRepaintHandler((UICHandlerFunction)ColorWdw::RepaintHandler);
    setThis(TheColor);
    Base.addDisplay(TheColor);

    Browser = new FileBrowser();

    Browser->SetAcceptCallback(this);
    Browser->Title("File browser for color tables.");

    if (file != NULL)
      {
          Load(file);
          Browser->File(file);
      }
    active = False;

    BuildMap();
}

int ColorWdw::Connect(char *target)
{
    int i = Target(target);

    if (!i)
      active = True;

    return(i);
}

void ColorWdw::Save(char *file)
{
    Write(file);
}

void ColorWdw::Load(char *file)
{
    Read(file);

    BuildMap();

    Repaint();
}

void ColorWdw::Callback(void *ptr)
{
    FileName.setValue(Browser->File());
}

void ColorWdw::BuildMap(void)
{
    int j = 31;

    char name[8];

    int old = Index();

    if (strcmp(Cmap.getColorMapName(),"ColorWdwMap"))
      {
	  Cmap.setColorMapName("ColorWdwMap");

	  TheColor.setPaintColorMap(Cmap);
	  TheColor.setPaintHeight(60);
	  TheColor.setPaintWidth(Base.getRootWindowWidth());

	  for (int i = 0; i < j; i++)
	    {
		sprintf(name,"color%d",i);

		Index(i);

		Cmap.addColor(name,Red(),Green(),Blue());
	    }
      } 
    else 
      {
	  for (int i = 0; i < j; i++)
	    {
		sprintf(name,"color%d",i);
		Index(i);
		Cmap.changeColor(name,Red(),Green(),Blue());
	    }
	  TheColor.setPaintColorMap(Cmap);
      }
    Index(old);
}

void ColorWdw::RepaintHandler(UIObject *object)
{
    object = getObject(this);
    this = getThis(object);

    Repaint();
}

void ColorWdw::Repaint(void)
{
    int j = 30;

    char name[8];

    Window wdw = TheColor.getXWindow();
    Display *disp = TheColor.getXDisplay();

    XGCValues  gcval;

    gcval.function = GXcopy;
    GC gc = XCreateGC (disp,wdw,GCFunction,&gcval);

    for (int i = 0; i < j; i++)
      {
	  sprintf(name,"color%d",i);

	  XSetForeground(disp,gc,Cmap.getXColorIndex(name));

	  XFillRectangle(disp,wdw,gc,i * 18,0,18,60);
      }
    XFlush(disp);
}
