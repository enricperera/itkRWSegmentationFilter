
**Description:** 

  This itk-based filter is used to obtain an image segmentation based 
  on the RW algorithm proposed by Grady (2006) [1]. 

  Two filters are provided: itkRWSegmentationFilter and 
  itkCudaRWSegmentationFilter, the former is processed in the CPU and 
  the latter benefits also from GPU computation (a CUDA compatible 
  graphic card is required).
  
  Given an input image (set by SetInput(TInputImage*)), and a label 
  image containing the seed points corresponding to different regions
  (set by SetLabelImage(TOutputImage*)), returns a segmentation image
  (TOutputImage* GetOutput()). The label image is required, if 
  SetLabelImage(TOutputImage*) is not called before calling Update(),
  the output will be an empty TOutputImage pointer.

  WARNING! This filter only works for 2D and 3D images

  WARNING! This filter has only been tested for TInputImage of float 
  pixel type and TOutputImage of unsigned char pixel type.

  Additional parameters can be set: Beta (for the weighting function
  default 1), NumThreads (CPU only, to specify how many openmp threads 
  will be used on the iterative solver BiCGStab, default 1), Tolerance 
  and MaximumNumberOfIterations (for the stopping criteria of the solver,
  default 1e-3 and 100, respectively) and WriteBackground (to specify
  if the region corresponding to the highest label will be assigned 
  its label or a value of 0, useful when considering that the last 
  label is background and its segmentation is irrelevant, default true).
  To speed up the computation, the algorithm computes the segmentation
  for L-1 labels, and obtains the last by subtraction. If a slightly 
  finer result is desired at the expense of some computation time, 
  SolveForAllLabels can be set to true (default false).

  Although it is an itk-like filter, it requires of Eigen3 libraries
  and openMP for the CPU approach, and CUDA for the GPU. Following you 
  can find the homepages for these libraries, and the versions which have 
  been used during the development, although other versions might also 
  work.

  - ITK (4.12): https://github.com/InsightSoftwareConsortium/ITK
  - Eigen3 (3.3.4-4): http://eigen.tuxfamily.org/index.php?title=Main_Page#Download
  - OpenMP: https://www.openmp.org/resources/openmp-compilers-tools/
  - CUDA Toolkit (9.1): https://developer.nvidia.com/cuda-toolkit 

  [1] L. Grady, “Random Walks for Image Segmentation,”IEEE 
      Transactions on Pattern Analysis and Machine Intelligence, 
      vol. 28, no. 11, pp. 1768–1783, nov 2006.


**Example:**

  Clone or download repository and enter the download folder.

    git clone https://github.com/enricperera/itkRWSegmentationFilter.git
    cd itkRWSegmentationFilter

  Create a new "build" directory. Configure and compile source.

    mkdir build
    cmake ../Example
    make

  Two executables will be provided which can be executed. Notice that if
  CUDA is not installed only the cpuExample will be built.

    ./bin/cpuExample InputImage LabelImage OutputImageName
    ./bin/gpuExample InputImage LabelImage OutputImageName

  First argument is the original image, second argument is the image
  (with the same size as the original image) that contains the seeded
  pixels, and the third argument is the name of the output image.


**Copyright:**

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

  - http://www.apache.org/licenses/LICENSE-2.0.txt

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
 