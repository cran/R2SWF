\name{dev2swf}
\alias{dev2swf}
\title{Convert R graphics to SWF using different graphics devices}
\usage{dev2swf(expr, interval = 1, output = "movie.swf", 
    dev = "png", file.ext = "png", img.name = "Rplot", ...)

}

\description{ Given
an R expression that can produce a sequence of images, this
function will record the images with the device provided (e.g.
\code{\link[grDevices]{png}()} or \code{\link[grDevices]{jpeg}()})
and convert them to a Flash file.}
\details{You can also use devices which are not in the \pkg{grDevices}
package by setting the \code{dev} argument to the name of the
function that opens a device, e.g. \code{\link[Cairo]{CairoPNG}}
in the \pkg{Cairo} package.}
\value{The path of the generated swf file if succeeded}
\author{Yixuan Qiu \email{yixuan.qiu@cos.name}}
\arguments{\item{expr}{an expression to generate a sequence of images.}
\item{interval}{the time interval between animation frames.}
\item{output}{the name of the output swf file.}
\item{dev}{the name of the graphics device to use
(e.g. \code{'png'} or \code{'jpeg'})}
\item{file.ext}{the file extension for the images}
\item{img.name}{the file name of the images without the extension}
\item{...}{other arguments to be passed to the graphics device.}
}
\examples{dev2swf({
    for (i in 1:10) plot(runif(20), ylim = c(0, 1))
}, dev = "png", file.ext = "png", output = "movie-png.swf")

dev2swf({
    for (i in 1:10) plot(runif(20), ylim = c(0, 1))
}, dev = "jpeg", file.ext = "jpeg", output = "movie-jpeg.swf")}






