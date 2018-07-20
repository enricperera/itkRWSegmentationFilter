/*=========================================================================
 *
 * Copyright Universitat Pompeu Fabra, Department of Information and
 * Comunication Technologies.
 * 
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#include "itkImage.h"
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include "../itkRWSegmentationFilter/itkRWSegmentationFilter.h"

typedef     float               InputPixelType;
const       int                 Dimension = 3;
typedef     itk::Image< InputPixelType, Dimension >  InputImageType;

typedef     unsigned char       LabelPixelType;
typedef     itk::Image< LabelPixelType , Dimension >  LabelImageType;

int main( int argc, char *argv[] )
{
  if( argc < 4 )
  {
      std::cerr << "Missing Parameters " << std::endl;
      std::cerr << "Usage: " << argv[0];
      std::cerr << "InputImage LabelImage OutputImageName [Beta] " << std::endl;
  return 1;
  }
  
  // Define image readers and load original image and label image
  typedef  itk::ImageFileReader< InputImageType > ImageReaderType;
  ImageReaderType::Pointer readerImage = ImageReaderType::New();
  readerImage->SetFileName( argv[1] ); // Set input image
  readerImage->Update();

  typedef  itk::ImageFileReader< LabelImageType > LabelReaderType;
  LabelReaderType::Pointer readerLabel = LabelReaderType::New();
  readerLabel->SetFileName( argv[2] ); // Set label image
  readerLabel->Update();

  typedef  itk::RWSegmentationFilter< InputImageType , LabelImageType > RWFilterType;
  RWFilterType::Pointer RWFilter = RWFilterType::New();
  RWFilter->SetInput( readerImage->GetOutput() );
  RWFilter->SetLabelImage( readerLabel->GetOutput() );
  if (argv[4])
    RWFilter->SetBeta( atof(argv[4]) );
  RWFilter->SetNumberOfThreads( 8 );
  // RWFilter->SetWriteBackground( false );

  // Set writer and write image
  typedef  itk::ImageFileWriter<  LabelImageType  > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( argv[3] );
  writer->SetInput( RWFilter->GetOutput() );

  try
  {
    writer->Update();
  }
  catch( itk::ExceptionObject & excep )
  {
    std::cerr << "Exception caught !" << std::endl;
    std::cerr << excep << std::endl;
  }
}