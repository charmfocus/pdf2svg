package pdf2svg

import (
	"io"
	"os"
)

func Convert(reader io.Reader) ([]byte, error) {
	pdfFile, err := os.CreateTemp("", "pdf2svg_*.pdf")
	if err != nil {
		return nil, err
	}
	defer os.Remove(pdfFile.Name())

	_, err = io.Copy(pdfFile, reader)
	pdfFile.Close()
	if err != nil {
		return nil, err
	}

	svgFilename := pdfFile.Name() + ".svg"
	err = convert(pdfFile.Name(), svgFilename)
	if err != nil {
		return nil, err
	}
	defer os.Remove(svgFilename)
	svgData, err := os.ReadFile(svgFilename)
	if err != nil {
		return nil, err
	}

	return svgData, nil
}
