/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 ColorSvr.cc
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "ColorSvr.hh"
#include "ColorSvrP.hh"

static void RpcTask(Task &self, ColorSvr *server);

static ColorSvr *server;

ColorSvr::ColorSvr(GraphicController *agc) : Window(agc,0,0,0,1280,1024,0,0)
{
    setTextScale(2,1);

    Point a;

    a.y = TEXT_Y;

    FAST int j = MAX_COLORS_SET;

    for (FAST k = 0; k < 3; k++)
      {
	  a.x = X_WIDTH / 3;
	  FAST char **text = Labels;

	  for (FAST int i = 0; i < j; i++, text++)
	    {
		horText(a,*text,WHITE);

		a.x += X_WIDTH;
	    }

	  a.y += Y_WIDTH;
      }

    display();

    taskDelay(5);

    ctlr = agc;

    int arg[1];

    arg[0] = (int)this;

    rpc = new Task((FUNCPTR)RpcTask,arg,1);
}

void ColorSvr::Redraw(FAST ColorCmd *cblk)
{
    FAST int index = cblk->index;
    FAST long *colors = &cblk->colors[0];

    if (index == USE_DEF)
      Draw();
    else if (index == DO_ALL)
      ctlr->setColorMap(colors,93);
    else
      ctlr->setColorMap(colors,++index);
}

void ColorSvr::Draw(void)
{
    Point a;
    FAST int j = 31;
    FAST int k = 3;
    FAST int color = 0;
    a.y = 0;

    for (FAST i = 0; i < k; i++)
      {
	  a.x = 0;

	  for (FAST int l = 0; l < j; l++, color++)
	    {
		frect(a,BOXW,BOXH,color);

		a.x += X_WIDTH;
	    }
	  a.y += Y_WIDTH;
      }
}

static void RpcTask(Task &self, ColorSvr *svr)
{
    rpcTaskInit();

    if (ColorRpcInit() != 0)
      {
	  fprintf(stderr,"Failed to initialize rpc code!\n");
	  exit(1);
      }

    server = svr;

    svc_run();
}

void *colorsetproc_1(FAST void *cblk, struct svc_req *req)
{
    server->Redraw(cblk);

    return((void *)1);
}
