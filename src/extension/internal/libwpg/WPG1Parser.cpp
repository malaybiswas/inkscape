/* libwpg
 * Copyright (C) 2006 Ariya Hidayat (ariya@kde.org)
 * Copyright (C) 2005 Fridrich Strba (fridrich.strba@bluewin.ch)
 * Copyright (C) 2004 Marc Oude Kotte (marc@solcon.nl)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the 
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 * Boston, MA  02111-1301 USA
 *
 * For further information visit http://libwpg.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by
 * Corel Corporation or Corel Corporation Limited."
 */

#include "WPG1Parser.h"
#include "WPGPaintInterface.h"
#include "libwpg_utils.h"

static const unsigned char defaultWPG1PaletteRed[] = {
	0x00, 0x00, 0x00, 0x00, 0x7F, 0x7F, 0x7F, 0x7F,
	0xC0, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
	0x00, 0x14, 0x20, 0x2C, 0x38, 0x45, 0x51, 0x61,
	0x71, 0x82, 0x92, 0xA2, 0xB6, 0xCB, 0xE3, 0xFF,
	0x00, 0x41, 0x7D, 0xBE, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xBE, 0x7D, 0x41,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x7D, 0x9E, 0xBE, 0xDF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xDF, 0xBE, 0x9E,
	0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D,
	0xB6, 0xC7, 0xDB, 0xEB, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEB, 0xDB, 0xC7,
	0xB6, 0xB6, 0xB6, 0xB6, 0xB6, 0xB6, 0xB6, 0xB6,
	0x00, 0x1C, 0x38, 0x55, 0x71, 0x71, 0x71, 0x71,
	0x71, 0x71, 0x71, 0x71, 0x71, 0x55, 0x38, 0x1C,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x38, 0x45, 0x55, 0x61, 0x71, 0x71, 0x71, 0x71,
	0x71, 0x71, 0x71, 0x71, 0x71, 0x61, 0x55, 0x45,
	0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38,
	0x51, 0x59, 0x61, 0x69, 0x71, 0x71, 0x71, 0x71,
	0x71, 0x71, 0x71, 0x71, 0x71, 0x69, 0x61, 0x59,
	0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51,
	0x00, 0x10, 0x20, 0x30, 0x41, 0x41, 0x41, 0x41,
	0x41, 0x41, 0x41, 0x41, 0x41, 0x30, 0x20, 0x10,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x20, 0x28, 0x30, 0x38, 0x41, 0x41, 0x41, 0x41,
	0x41, 0x41, 0x41, 0x41, 0x41, 0x38, 0x30, 0x28,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x3C, 0x30, 0x34, 0x3C, 0x41, 0x41, 0x41, 0x41,
	0x41, 0x41, 0x41, 0x41, 0x41, 0x3C, 0x34, 0x30,
	0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static const unsigned char defaultWPG1PaletteGreen[] = {
	0x00, 0x00, 0x7F, 0x7F, 0x00, 0x00, 0x3F, 0x7F,
	0xC0, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF,
	0x00, 0x14, 0x20, 0x2C, 0x38, 0x45, 0x51, 0x61,
	0x71, 0x82, 0x92, 0xA2, 0xB6, 0xCB, 0xE3, 0xFF,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x41, 0x7D, 0xBE, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xBE, 0x7D, 0x41,
	0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D,
	0x7D, 0x9E, 0xBE, 0xDF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xDF, 0xBE, 0x9E,
	0xB6, 0xB6, 0xB6, 0xB6, 0xB6, 0xB6, 0xB6, 0xB6,
	0xB6, 0xC7, 0xDB, 0xEB, 0xFF, 0xFF, 0xFF, 0xFF,
	0xDF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEB, 0xDB, 0xC7,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x1C, 0x38, 0x55, 0x71, 0x71, 0x71, 0x71,
	0x71, 0x71, 0x71, 0x71, 0x71, 0x55, 0x38, 0x1C,
	0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38,
	0x38, 0x45, 0x55, 0x61, 0x71, 0x71, 0x71, 0x71,
	0x71, 0x71, 0x71, 0x71, 0x71, 0x61, 0x55, 0x45,
	0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51,
	0x51, 0x59, 0x61, 0x69, 0x71, 0x71, 0x71, 0x71,
	0x71, 0x71, 0x71, 0x71, 0x71, 0x69, 0x61, 0x59,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x10, 0x20, 0x30, 0x41, 0x41, 0x41, 0x41,
	0x41, 0x41, 0x41, 0x41, 0x41, 0x30, 0x20, 0x10,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x20, 0x28, 0x30, 0x38, 0x41, 0x41, 0x41, 0x41,
	0x41, 0x41, 0x41, 0x41, 0x41, 0x38, 0x30, 0x28,
	0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C,
	0x3C, 0x30, 0x34, 0x3C, 0x41, 0x41, 0x41, 0x41,
	0x41, 0x41, 0x41, 0x41, 0x41, 0x3C, 0x34, 0x30,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static const unsigned char defaultWPG1PaletteBlue[] = {
	0x00, 0x7F, 0x00, 0x7F, 0x00, 0x7F, 0x00, 0x7F,
	0xC0, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
	0x00, 0x14, 0x20, 0x2C, 0x38, 0x45, 0x51, 0x61,
	0x71, 0x82, 0x92, 0xA2, 0xB6, 0xCB, 0xE3, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xBE, 0x7D, 0x41,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x41, 0x7D, 0xBE, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xDF, 0xBE, 0x9E,
	0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D,
	0x7D, 0x9E, 0xBE, 0xDF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xEB, 0xDB, 0xC7,
	0xB6, 0xB6, 0xB6, 0xB6, 0xB6, 0xB6, 0xB6, 0xB6,
	0xB6, 0xC7, 0xDB, 0xEB, 0xFF, 0xFF, 0xFF, 0xFF,
	0x71, 0x71, 0x71, 0x71, 0x71, 0x55, 0x38, 0x1C,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x1C, 0x38, 0x55, 0x71, 0x71, 0x71, 0x71,
	0x71, 0x71, 0x71, 0x71, 0x71, 0x61, 0x55, 0x45,
	0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38,
	0x38, 0x45, 0x55, 0x61, 0x71, 0x71, 0x71, 0x71,
	0x71, 0x71, 0x71, 0x71, 0x71, 0x69, 0x61, 0x59,
	0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51,
	0x51, 0x59, 0x61, 0x69, 0x71, 0x71, 0x71, 0x71,
	0x41, 0x41, 0x41, 0x41, 0x41, 0x30, 0x20, 0x10,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x10, 0x20, 0x30, 0x41, 0x41, 0x41, 0x41,
	0x41, 0x41, 0x41, 0x41, 0x41, 0x38, 0x30, 0x28,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x20, 0x28, 0x30, 0x38, 0x41, 0x41, 0x41, 0x41,
	0x41, 0x41, 0x41, 0x41, 0x41, 0x3C, 0x34, 0x30,
	0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C, 0x3C,
	0x2C, 0x30, 0x34, 0x3C, 0x41, 0x41, 0x41, 0x41,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};


WPG1Parser::WPG1Parser(WPGInputStream *input, WPGPaintInterface* painter):
	WPGXParser(input, painter),
	m_success(true), m_exit(false),
	m_width(0), m_height(0)
{
}

bool WPG1Parser::parse()
{
	typedef void (WPG1Parser::*Method)();

	struct RecordHandler
	{
		int type;
		const char *name;
		Method handler;
	};

	static const struct RecordHandler handlers[] =
	{
		{ 0x01, "Fill Attributes",   &WPG1Parser::handleFillAttributes },	 
		{ 0x02, "Line Attributes",   &WPG1Parser::handleLineAttributes },	 
		{ 0x03, "Marker Atttibutes", 0 },	 
		{ 0x04, "Polymarker",        0 },	 
		{ 0x05, "Line",              &WPG1Parser::handleLine },	 
		{ 0x06, "Polyline",          &WPG1Parser::handlePolyline },	 
		{ 0x07, "Rectangle",         &WPG1Parser::handleRectangle },	 
		{ 0x08, "Polygon",           &WPG1Parser::handlePolygon },	 
		{ 0x09, "Ellipse",           &WPG1Parser::handleEllipse },	 
		{ 0x0e, "Colormap",          &WPG1Parser::handleColormap },
		{ 0x0f, "Start WPG",         &WPG1Parser::handleStartWPG },
		{ 0x10, "End WPG",           &WPG1Parser::handleEndWPG },
		{ 0x00, 0, 0 } // end marker
	};	 

	// initialization
	m_success = true;
	m_exit = false;
	
	// default style
	m_pen.foreColor = WPGColor(0,0,0);
	m_pen.backColor = WPGColor(0,0,0);
	m_pen.width = 0.001;
	m_pen.height = 0.001;
	m_pen.solid = true;
	m_pen.dashArray = WPGDashArray();
	m_brush.foreColor = WPGColor(0,0,0);
	m_brush.backColor = WPGColor(0,0,0);
	resetPalette();

	while(!m_input->atEnd())
	{
		long recordPos = m_input->tell();
		int recordType = readU8();
		int length = readVariableLengthInteger();
		long nextPos = m_input->tell() + length;
#if !defined(DEBUG)
		(void)recordPos;
#endif // !defined(DEBUG)

		// search function to handler this record
		int index = -1;
		for(int i = 0; (index < 0) && handlers[i].name; i++)
			if(handlers[i].type == recordType)
				index = i;
				
		WPG_DEBUG_MSG(("\n"));
		if(index < 0)
			WPG_DEBUG_MSG(("Unknown record type 0x%02x at %d  size %d\n", 
				recordType, recordPos, length));
		else
		{
			Method recordHandler = handlers[index].handler;
			if(!recordHandler)
				WPG_DEBUG_MSG(("Record '%s' (ignored) type 0x%02x at %d  size %d\n", 
					handlers[index].name, recordType, recordPos, length));
			else
			{
				WPG_DEBUG_MSG(("Record '%s' type 0x%02x at %d  size %d\n", 
					handlers[index].name, recordType, recordPos, length));
					
				// invoke the handler for this record
				(this->*recordHandler)();
			}
		}

		//if(m_input->tell() > nextPos)
		{
			//WPG_DEBUG_MSG(("Record 0x%x consumes more bytes than necessary!\n", recordType));
			WPG_DEBUG_MSG(("Current stream position: %d\n", m_input->tell()));
		}
	
		if(m_exit) break;
		
		m_input->seek(nextPos);
	}

	return m_success;
}

void WPG1Parser::handleStartWPG()
{
	unsigned char version = readU8();
	unsigned char bitFlags = readU8();
	(void)version;
	(void)bitFlags;
	m_width = readU16();
	m_height = readU16();

	double width = (double)m_width / 1200.0;
	double height = (double)m_height / 1200.0;
	m_painter->startDocument(width, height);

	WPG_DEBUG_MSG(("StartWPG\n"));
}

void WPG1Parser::handleEndWPG()
{
	m_painter->endDocument();
	m_exit = true;
	
	WPG_DEBUG_MSG(("EndWPG\n"));
}

void WPG1Parser::handleColormap()
{
	unsigned startIndex = readU16();
	unsigned numEntries = readU16();

	WPG_DEBUG_MSG(("Colormap\n"));
	for(int i = 0; i < static_cast<int>(numEntries); i++)
	{
		WPGColor color;
		color.red = readU8();
		color.green = readU8();
		color.blue = readU8();
		m_colorPalette[startIndex+i] = color;
		WPG_DEBUG_MSG(("Index#%d: RGB %d %d %d\n", startIndex+i, color.red, color.green, color.blue));
	}
}

void WPG1Parser::handleFillAttributes()
{
	unsigned char style = readU8();
	unsigned char color = readU8();

	if(style == 0)
		m_brush.style = WPGBrush::NoBrush;
	if(style == 1)
		m_brush.style = WPGBrush::Solid;

	m_brush.foreColor = m_colorPalette[color];

	WPG_DEBUG_MSG(("Fill Attributes\n"));
	WPG_DEBUG_MSG(("         Fill style: %d\n", style));
	WPG_DEBUG_MSG(("   Fill color index: %d\n", color));
}

void WPG1Parser::handleLineAttributes()
{
	unsigned char style = readU8();
	unsigned char color = readU8();
	unsigned int width = readU16();

	m_pen.solid = style != 0;
	m_pen.foreColor = m_colorPalette[color];
	m_pen.width = (double)width / 1200.0;

	WPG_DEBUG_MSG(("Line Attributes\n"));
	WPG_DEBUG_MSG(("         Line style: %d\n", style));
	WPG_DEBUG_MSG(("   Line color index: %d\n", color));
	WPG_DEBUG_MSG(("         Line width: %d\n", width));
}

void WPG1Parser::handleLine()
{
	int sx = readS16();
	int sy = readS16();
	int ex = readS16();
	int ey = readS16();

	WPGPointArray points;
	points.add(WPGPoint((double)sx/1200.0, (double)(m_height-sy)/1200.0));
	points.add(WPGPoint((double)ex/1200.0, (double)(m_height-ey)/1200.0));

	m_painter->setBrush(m_brush);
	m_painter->setPen(m_pen);
	m_painter->drawPolygon(points);

	WPG_DEBUG_MSG(("Line\n"));
	WPG_DEBUG_MSG(("    Starting point: %d,%d\n", sx, sy));
	WPG_DEBUG_MSG(("         End point: %d,%d\n", ex, ey));
}

void WPG1Parser::handlePolyline()
{
	unsigned int count = readU16();

	WPGPointArray points;
	for(unsigned int i = 0; i < count; i++ )
	{
		long x = readS16();
		long y = readS16();
		points.add(WPGPoint((double)x/1200.0, (double)(m_height-y)/1200.0));
	}

	m_painter->setBrush(WPGBrush()); // not filled
	m_painter->setPen(m_pen);
	m_painter->drawPolygon(points);

	WPG_DEBUG_MSG(("Polyline\n"));
}

void WPG1Parser::handleRectangle()
{
	int x = readS16();
	int y = readS16();
	int w = readS16();
	int h = readS16();

	WPGRect rect;
	rect.x1 = (double)x/1200.0;
	rect.y1 = (double)(m_height-y)/1200.0;
	rect.x2 = rect.x1 + (double)w/1200.0;
	rect.y2 = rect.y1 + (double)h/1200.0;

	m_painter->setBrush(m_brush);
	m_painter->setPen(m_pen);
	m_painter->drawRectangle(rect, 0, 0);

	WPG_DEBUG_MSG(("Line\n"));
	WPG_DEBUG_MSG(("    Corner point: %d,%d\n", x, y));
	WPG_DEBUG_MSG(("           Width: %d\n", w));
	WPG_DEBUG_MSG(("          Height: %d\n", h));
}

void WPG1Parser::handlePolygon()
{
	unsigned int count = readU16();

	WPGPointArray points;
	for(unsigned int i = 0; i < count; i++ )
	{
		long x = readS16();
		long y = readS16();
		points.add(WPGPoint((double)x/1200.0, (double)(m_height-y)/1200.0));
	}

	m_painter->setBrush(m_brush);
	m_painter->setPen(m_pen);
	m_painter->drawPolygon(points);

	WPG_DEBUG_MSG(("Polygon\n"));
}

void WPG1Parser::handleEllipse()
{
	int cx = readS16();
	int cy = readS16();
	int rx = readS16();
	int ry = readS16();

	WPGPoint center;
	center.x = (double)cx/1200.0;
	center.y = (double)(m_height-cy)/1200.0;

	double radx = (double)rx/1200.0;
	double rady = (double)ry/1200.0;

	m_painter->setBrush(m_brush);
	m_painter->setPen(m_pen);
	m_painter->drawEllipse(center, radx, rady);

	WPG_DEBUG_MSG(("Ellipse\n"));
	WPG_DEBUG_MSG(("    Center point: %d,%d\n", cx, cy));
	WPG_DEBUG_MSG(("        Radius x: %d\n", rx));
	WPG_DEBUG_MSG(("        Radius y: %d\n", ry));
}

void WPG1Parser::resetPalette()
{
	m_colorPalette.clear();
	for (int i=0; i<256; i++)
	{
		WPGColor color;
		color.red = defaultWPG1PaletteRed[i];
		color.green = defaultWPG1PaletteGreen[i];
		color.blue = defaultWPG1PaletteBlue[i];
		m_colorPalette[i] = color;
	}
}

