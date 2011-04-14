\name{image2swf}
\alias{image2swf}
\title{Convert a sequence of images to SWF file}
\usage{image2swf(filenames, output = "movie.swf", interval = 1)

}
\description{
Given the file names of a sequence of images, this function can
convert them to a Flash file. Supported input formats are jpg/jpeg,
png, tiff and svg.}
\details{This function creates a temporary XML file and then calls the
swfmill program (http://swfmill.org/) to implement the
conversion. Currently the support of SVG file is not perfect yet.}
\value{The path of the generated swf file if successful, or NULL is returned.}
\author{Yixuan Qiu \email{yixuan.qiu@cos.name}}
\arguments{\item{filenames}{the file names of the images to be converted.}
\item{output}{the name of the output swf file.}
\item{interval}{the time interval between animation frames.}
}
\examples{if (capabilities("png")) {
    png("Rplot\%03d.png")
    for (i in 1:9) plot(runif(20), ylim = c(0, 1))
    dev.off()
    image2swf(sprintf("Rplot\%03d.png", 1:9))
}}





