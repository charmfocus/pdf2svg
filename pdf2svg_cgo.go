package pdf2svg

/*
#cgo pkg-config: poppler-glib
#include "pdf2svg.h"
*/
import "C"
import (
	"fmt"
	"unsafe"
)

func init() {
	C.pdf2svg_init()
}

func convert(pdfFilename string, svgFilename string) error {
	cPdfFilename := C.CString(pdfFilename)
	defer freeCString(cPdfFilename)

	cSvgFilename := C.CString(svgFilename)
	defer freeCString(cSvgFilename)

	errValue := int(C.pdf2svg_convert(cPdfFilename, cSvgFilename))
	if errValue != 0 {
		return fmt.Errorf("pdf2svg failed, err = %d", errValue)
	}
	return nil
}

func freeCString(s *C.char) {
	C.free(unsafe.Pointer(s))
}
