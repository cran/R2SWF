#include <libming.h>
#include <stdlib.h>
#include "rsvg-path.h"

#include <R.h>
#include <Rdefines.h>
#include <Rinternals.h>

typedef struct {
	unsigned short lwd;
	byte red;
	byte green;
	byte blue;
	byte alpha;
    int lineType;
	float miterLimit;
} StrokeStyle;

typedef struct {
	byte red;
	byte green;
	byte blue;
	byte alpha;
} FillStyle;

typedef enum {
    STYIND_STROKE = 0,
	STYIND_STROKE_WIDTH,
	STYIND_STROKE_LINECAP,
	STYIND_STROKE_LINEJOIN,
	STYIND_STROKE_MITERLIMIT,
	STYIND_STROKE_OPACITY,
	STYIND_FILL,
	STYIND_FILL_RULE,
	STYIND_FILL_OPACITY
} StyleIndex;


void SWFShape_setStrokeStyle(SWFShape s, StrokeStyle *ty)
{
    SWFShape_setLine2(s, ty->lwd, ty->red, ty->green, ty->blue,
			          ty->alpha, ty->lineType, ty->miterLimit);
}

void SWFShape_setFillStyle(SWFShape s, FillStyle *ty)
{
    SWFFill f = SWFShape_addSolidFill(s, ty->red, ty->green,
			                          ty->blue, ty->alpha);
    SWFShape_setRightFill(s, f);
	destroySWFFill(f);
}

void SWFShape_setStyleFromR(SWFShape s, SEXP style, StrokeStyle *sstyle,
		                    FillStyle *fstyle)
{
    const char *stroke_width = CHAR(STRING_ELT(style, STYIND_STROKE_WIDTH));
    double lwd = atof(stroke_width);
	int cap, endcap; 
	const char *stroke_linecap;
	int join;
	const char *stroke_linejoin;
	const char *stroke_miterlimit;
	const char *stroke_opacity;
	float r, g, b;
	const char *stroke;
	const char *fill_opacity;
	const char *fill;

	sstyle->lwd = (unsigned short) floor(lwd > 255 ? 255 : lwd);

    stroke_linecap = CHAR(STRING_ELT(style, STYIND_STROKE_LINECAP));
    if(!strcmp("round", stroke_linecap))
	{
		cap = SWF_LINESTYLE_CAP_ROUND;
		endcap = SWF_LINESTYLE_FLAG_ENDCAP_ROUND;
	} else if(!strcmp("square", stroke_linecap)) {
		cap = SWF_LINESTYLE_CAP_SQUARE;
		endcap = SWF_LINESTYLE_FLAG_ENDCAP_SQUARE;
	} else {
		cap = SWF_LINESTYLE_CAP_NONE;
		endcap = SWF_LINESTYLE_FLAG_ENDCAP_NONE;
	}
	
   	stroke_linejoin = CHAR(STRING_ELT(style, STYIND_STROKE_LINEJOIN));
	if(!strcmp("round", stroke_linejoin))
	{
		join = SWF_LINESTYLE_JOIN_ROUND;
	} else if(!strcmp("bevel", stroke_linejoin)) {
		join = SWF_LINESTYLE_JOIN_BEVEL;
	} else {
		join = SWF_LINESTYLE_JOIN_MITER;
	}
	sstyle->lineType = cap | endcap | join;
	
    stroke_miterlimit = CHAR(STRING_ELT(style, STYIND_STROKE_MITERLIMIT));
	sstyle->miterLimit = (float) atof(stroke_miterlimit);
	
    stroke_opacity = CHAR(STRING_ELT(style, STYIND_STROKE_OPACITY));
	sstyle->alpha = (byte) floor(atof(stroke_opacity) * 255 + 0.5);

	stroke = CHAR(STRING_ELT(style, STYIND_STROKE));
	if(!strcmp("none", stroke))
	{
        sstyle->alpha = 0;
	} else if(!strncmp("rgb", stroke, 3)) {
        sscanf(stroke, "rgb(%f%%,%f%%,%f%%)", &r, &g, &b);
		sstyle->red = (byte) floor(r / 100 * 255 + 0.5);
		sstyle->green = (byte) floor(g / 100 * 255 + 0.5);
		sstyle->blue = (byte) floor(b / 100 * 255 + 0.5);
	} else {
		sstyle->red = sstyle->green = sstyle->blue = 0;
	}

	fill_opacity = CHAR(STRING_ELT(style, STYIND_FILL_OPACITY));
	fstyle->alpha = (byte) floor(atof(fill_opacity) * 255 + 0.5);

	fill = CHAR(STRING_ELT(style, STYIND_FILL));
	if(!strcmp("none", fill))
	{
        fstyle->alpha = 0;
	} else if(!strncmp("rgb", fill, 3)) {
        sscanf(fill, "rgb(%f%%,%f%%,%f%%)", &r, &g, &b);
		fstyle->red = (byte) floor(r / 100 * 255 + 0.5);
		fstyle->green = (byte) floor(g / 100 * 255 + 0.5);
		fstyle->blue = (byte) floor(b / 100 * 255 + 0.5);
	} else {
		fstyle->alpha = 0;
	}

    SWFShape_setStrokeStyle(s, sstyle);
    SWFShape_setFillStyle(s, fstyle);
}

void SWFShape_drawFromR(SWFShape s, SEXP data, SEXP xy)
{
	const char *d = CHAR(STRING_ELT(data, 0));
	double transX = REAL(xy)[0];
	double transY = REAL(xy)[1];

    RsvgBpathDef *bpd = rsvg_parse_path(d);
    int i;
    RsvgBpath bpath;
    for(i = 0; i < bpd->n_bpath; i++)
    {
        bpath = bpd->bpath[i];
        switch(bpath.code)
        {
        case RSVG_MOVETO:
			SWFShape_drawLineTo(s, bpath.x3 + transX, bpath.y3 + transY);
        case RSVG_MOVETO_OPEN:
            SWFShape_movePenTo(s, bpath.x3 + transX, bpath.y3 + transY);
            break;
        case RSVG_CURVETO:
            SWFShape_drawCubicTo(s, bpath.x1 + transX, bpath.y1 + transY,
					                bpath.x2 + transX, bpath.y2 + transY,
								    bpath.x3 + transX, bpath.y3 + transY);
            break;
        case RSVG_LINETO:
            SWFShape_drawLineTo(s, bpath.x3 + transX, bpath.y3 + transY);
            break;
        case RSVG_END:
            break;
        }
    }
    rsvg_bpath_def_free(bpd);
}

SEXP svg2swf(SEXP filesData, SEXP outName, SEXP size,
		     SEXP bgColor, SEXP interval)
{
    SWFMovie mainMovie = newSWFMovieWithVersion(8);
	SWFMovieClip mcFrame = NULL;
	SWFShape shapeInMcFrame = NULL;
	SWFDisplayItem dispItem = NULL;
	SWFMovieBlockType ublock;

	StrokeStyle strokeStyle;
	FillStyle fillStyle;

	int nFiles = LENGTH(filesData);
    int i, j;
	int pathsListLen;
	SEXP pathsList, path, style, data, xyOffset;

    SWFMovie_setDimension(mainMovie, (float) REAL(size)[2], (float) REAL(size)[3]);
    SWFMovie_setBackground(mainMovie,
			               INTEGER(bgColor)[0],
						   INTEGER(bgColor)[1],
						   INTEGER(bgColor)[2]);
	SWFMovie_setRate(mainMovie, (float) 1.0 / REAL(interval)[0]);
	SWFMovie_setNumberOfFrames(mainMovie, nFiles);

    Ming_setCubicThreshold(1);
 
	for(i = 0; i < nFiles; i++)
	{
		pathsList = VECTOR_ELT(filesData, i);
		pathsListLen = LENGTH(pathsList);

		mcFrame = newSWFMovieClip();
		SWFMovieClip_setNumberOfFrames(mcFrame, 1);
		
		for(j = 0; j < pathsListLen; j++)
		{
		    path = VECTOR_ELT(pathsList, j);
    		style = VECTOR_ELT(path, 0);
	        data = VECTOR_ELT(path, 1);
	        xyOffset = VECTOR_ELT(path, 2);

			shapeInMcFrame = newSWFShape();
    		SWFShape_setStyleFromR(shapeInMcFrame, style, &strokeStyle, &fillStyle);
	    	SWFShape_drawFromR(shapeInMcFrame, data, xyOffset);
		    SWFMovieClip_add(mcFrame, (SWFBlock) shapeInMcFrame);
		}
		SWFMovieClip_nextFrame(mcFrame);
		if(dispItem) SWFMovie_remove(mainMovie, dispItem);
		ublock.mc = mcFrame;
		dispItem = SWFMovie_add_internal(mainMovie, ublock);
		SWFMovie_nextFrame(mainMovie);
	}
    SWFMovie_save(mainMovie, CHAR(STRING_ELT(outName, 0)));
	destroySWFMovie(mainMovie);

    return R_NilValue;
}
