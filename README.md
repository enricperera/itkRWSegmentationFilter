
**Description:** 

  This itk-based filter is used to obtain an image segmentation based 
  on the RW algorithm proposed by Grady (2006) [1].

  Given an input image (set by SetInput(TInputImage*)), and a label 
  image containing the seed points corresponing to different regions
  (set by SetLabelImage(TOutputImage*)), returns a segmentation image
  (TOutputImage* GetOutput()). The label image is required, if 
  SetLabelImage(TOutputImage*) is not called before calling Update(),
  the output will be an empty TOutputImage pointer.

  WARNING! This filter only works for 2D and 3D images

  WARNING! This filter has only been tested for TInputImage of float 
  pixel type and TOutputImage of unsigned char pixel type.

  Additional parameters can be set: Beta (for the weighting function
  default 1), NumThreads (to specify how many openmp threads will be 
  used on the iterative solver BiCGStab, default 1), Tolerance and 
  MaximumNumberOfIteratins (for the stopping criteria of the solver,
  default 1e-3 and 100, respectively) and WriteBackground (to specify
  if the region corresponding to the highest label will be assigned 
  its label or a value of 0, useful when considering that the last 
  label is background and its segmentation is irrelevant, default true).

  Although it is an itk-based filter, it requires of Eigen3 libraries
  and openMP to speed the solver. Eigen3 include dir should be indicated
  when compiling the filter. The flag -fopenmp has to be specified to
  allow multithreading.

  [1] L. Grady, “Random Walks for Image Segmentation,”IEEE 
      Transactionson Pattern Analysis and Machine Intelligence, 
      vol. 28, no. 11, pp. 1768–1783, nov 2006.


**Example:**

  An example program is provided and can be executed from the terminal:

    ./mySegmentationProgram InputImage LabelImage OutputImageName

  First argument is the original image, second argument is the image
  (with the same size as the original image) that contains the seeded
  pixels, and the third argument is the name of the output image.


**Copyright:**

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0.txt

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
 
 **External Sources**

  ITK can be downloaded from:

    https://itk.org/ITK/resources/software.html

  Eigen can be downloaded from:

    http://eigen.tuxfamily.org/index.php?title=Main_Page
