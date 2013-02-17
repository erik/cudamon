cudamon
=======

GPU monitor for CUDA devices. It is very much in progress, and not 
generally useful quite yet.

Check back later.

Building
--------

cudamon is currently very simple, not feature complete, and has no way
of being customized outside of editing the source. This will hopefully
change in the future, but for now, the build process is as simple as
running `make`. There is currently no install support, mainly because
it is not featureful enough to warrant an installation yet.

If you do decide to put `cudamond` somewhere, do keep in mind that the
`cudamond` binary and the `www/` directory need to be in the same
directory. Or you could modify the source in `www/server.c` to change
this location.

This will be configurable at some point in the future.

Device Support
--------------

This program relies on the NVML library to gather its information from
the GPU devices. How feature-complete it is on any one device is
entirely dependent on how well that device is supported by the
library.

From NVIDIA's NVML documentation:

  * Full Support
    - NVIDIA Tesla Line: S2050, C2050, C2070, C2075, M2050, M2070, M2075, M2090,
        X2070, X2090, K10, K20, K20X
    - NVIDIA Quadro Line: 4000, 5000, 6000, 7000, M2070-Q, 600, 2000, 3000M and 410
    - NVIDIA GeForce Line: None

  * Limited Support
    - NVIDIA Tesla Line:   S1070, C1060, M1060
    - NVIDIA Quadro Line:  All other current and previous generation Quadro-branded parts
    - NVIDIA GeForce Line: All current and previous generation GeForce-branded parts

Dependencies
------------

cudamon is meant to be as simple and quick to build and use as
possible, and therefore is very light on run- and build-time
dependencies. The only library actually required to run (outside of
normal libc, pthreads, etc) is the CUDA device driver, which you
should have anyway if you plan on using the device in any sense.

Other build time dependencies are included, and are the mongoose
embedded HTTP server and NVIDIA's NVML library. See the appropriate
README in `lib/*/` for more information on these libraries.

The web-based client utilizes Shutterstock's Rickshaw graphing
library, which itself is based on d3.js. jQuery and jQuery-ui are also
pulled in by these libraries.

Development
-----------

How useful this monitor is to you will depend heavily on how much my
needs match yours. I haven't used CUDA for all that long, so my needs
may well not match the average person's. If you have something you
want to add to the server or the client, please, feel free to do so
and shoot me a pull request. I am also open to criticism / hatred /
suggestions.

### Server

The server component of cudamon is very simple, a few hundred lines of
C based off of a couple of solid libraries doing all the heavy
lifting. I didn't implement all of NVML's functionality, so if you
find some metric that you think should be tracked, feel free to add it
or just let me know that it should be implemented. This is currently a
one-person project done for my own benefit while working on CUDA
programs, so I have no idea what other people would want or need.

### Client

If you think the client is horribly ugly (I don't blame you) and want
to focus on fixing that, try running `./faker.rb`, which just serves
up some random fake data from port `4567`. This could be useful if you
want to test locally and don't have a CUDA device installed, or have a
device that doesn't implement many of the features. You should install
Sinatra (`gem install sinatra`) if you want to use `faker.rb`.

My design abilities are pretty poor, so I am especially open to any
improvements on the client side.

License
-------

See `lib/nvml/COPYING.txt` for NVML's (non-F/LOSS) license.

Copyright (c) 2013 Erik Price

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
