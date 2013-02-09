cudamon
=======

GPU monitoring daemon for CUDA devices.

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

License
-------

See `lib/nvml/LICENSE.txt` for NVML's license.

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
