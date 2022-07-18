// Copyright (C) 2007-2013 David Barton (davebarton@cityinthesky.co.uk)
// <http://www.cityinthesky.co.uk/>

// Copyright (C) 2007 Matthew Flaschen (matthew.flaschen@gatech.edu)
// Updated to allow conversion of all pages at once.

//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

// Definitely works with Cairo v1.2.6 and Poppler 0.5.4

#include "pdf2svg.h"

/*
int main(int argn, char *args[])
{
	pdf2svg_init();

	// Get command line arguments
	if (argn != 3) {
		printf("Usage: pdf2svg <in file.pdf> <out file.svg>\n");
		return -2;
	}

	return pdf2svg_convert(args[1], args[2]);
}
*/

gchar *pdf2svg_get_absolute_file_name(const gchar *fileName);
int pdf2svg_convert_page(PopplerPage *page, const char* svgFilename);

void pdf2svg_init(void)
{
	#ifndef G_GNUC_BEGIN_IGNORE_DEPRECATIONS
	// Initialise the GType library
	g_type_init();
	#endif
}

int pdf2svg_convert(char* pdfFilename, char* svgFilename)
{
	// Poppler stuff
	PopplerDocument *pdffile;
	PopplerPage *page;

	// Open the PDF file
	gchar *absoluteFileName = pdf2svg_get_absolute_file_name(pdfFilename);
	gchar *filename_uri = g_filename_to_uri(absoluteFileName, NULL, NULL);
	g_free(absoluteFileName);
	pdffile = poppler_document_new_from_file(filename_uri, NULL, NULL);
	g_free(filename_uri);
	if (pdffile == NULL) {
		fprintf(stderr, "Unable to open file\n");
		return -3;
	}

	// Get the page
	int conversionErrors = 0;
	page = poppler_document_get_page(pdffile, 0);
	conversionErrors = pdf2svg_convert_page(page, svgFilename);

	g_object_unref(pdffile);

	if(conversionErrors != 0) {
		return -4;
	}
	else {
		return 0;
	}
}

// Begin theft from ePDFview (Copyright (C) 2006 Emma's Software) under the GPL
gchar *pdf2svg_get_absolute_file_name(const gchar *fileName)
{
	gchar *absoluteFileName = NULL;
	if (g_path_is_absolute(fileName)) {
		absoluteFileName = g_strdup(fileName);
	}
	else {
		gchar *currentDir = g_get_current_dir();
		absoluteFileName = g_build_filename(currentDir, fileName, NULL);
		g_free(currentDir);
	}
	return absoluteFileName;
}
// End theft from ePDFview


int pdf2svg_convert_page(PopplerPage *page, const char* svgFilename)
{
	// Poppler stuff 
	double width, height;

	// Cairo stuff
	cairo_surface_t *surface;
	cairo_t *drawcontext;

	if (page == NULL) {
		fprintf(stderr, "Page does not exist\n");
		return -1;
	}
	poppler_page_get_size (page, &width, &height);

	// Open the SVG file
	surface = cairo_svg_surface_create(svgFilename, width, height);
	drawcontext = cairo_create(surface);

	// Render the PDF file into the SVG file
	poppler_page_render_for_printing(page, drawcontext);
	cairo_show_page(drawcontext);

	// Close the SVG file
	cairo_destroy(drawcontext);
	cairo_surface_destroy(surface);

	// Close the PDF file
	g_object_unref(page);
	
	return 0;     
}
