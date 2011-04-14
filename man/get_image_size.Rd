\name{get_image_size}
\alias{get_image_size}
\title{Get the size of an image}
\usage{get_image_size(filename)

}
\description{ This function returns the width and
height of an image. Supported formats are jpg/jpeg, png, gif and
svg.}
\details{This function is mainly used internally to decide the width and
height of the SWF file according to the sizes of input images.

For jpg/jpeg, png and gif formats, the function returns the image
size in pixels. For svg file, width is always set to 640 px and
height is adjusted according to the width/height ratio. If the
image size could not be determined, then default values (640x640,
in pixels) are used.}
\value{A list containing the width and height of the image.}
\author{Yixuan Qiu \email{yixuan.qiu@cos.name}}
\arguments{\item{filename}{the path of the image file.}
}
\examples{get_image_size(normalizePath(file.path(R.home("doc"), 
    "html/logo.jpg")))}
