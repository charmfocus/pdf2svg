#ifndef __PDF_TO_SVG_H__
#define __PDF_TO_SVG_H__

#include <assert.h>
#include <stdlib.h>
#include <glib.h>
#include <poppler.h>
#include <poppler-document.h>
#include <poppler-page.h>
#include <cairo.h>
#include <cairo-svg.h>
#include <stdio.h>
#include <string.h>

void pdf2svg_init(void);
int pdf2svg_convert(char* pdfFilename, char* svgFilename);

#endif /* __PDF_TO_SVG_H__ */
