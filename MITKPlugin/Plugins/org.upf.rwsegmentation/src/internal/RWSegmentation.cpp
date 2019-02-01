/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/


// Blueberry
#include <berryISelectionService.h>
#include <berryIWorkbenchWindow.h>

// Qmitk
#include "RWSegmentation.h"

// Qt
#include <QMessageBox>
#include <QCursor>

// mitk image
#include <mitkImage.h>
#include <mitkImageCast.h>
#include <mitkNodePredicateDataType.h>
#include <mitkNodePredicateProperty.h>

// ITK
#include <itkImageRegionIterator.h>
#include "itkImageDuplicator.h"


#include "itkRWSegmentationFilter/itkRWSegmentationFilter.h"


const std::string RWSegmentation::VIEW_ID = "org.mitk.views.rwsegmentation";

void RWSegmentation::SetFocus()
{
  m_Controls.segmentPushButton->setFocus();
}

void RWSegmentation::CreateQtPartControl(QWidget *parent)
{
  m_DataStorage = this->GetDataStorage();
                                                      
  // create GUI widgets from the Qt Designer's .ui file
  m_Controls.setupUi(parent);

  m_Controls.inputImageComboBox->SetDataStorage(m_DataStorage);
  m_Controls.inputImageComboBox->SetPredicate(mitk::TNodePredicateDataType<mitk::Image>::New() );

  connect(m_Controls.segmentPushButton, SIGNAL(clicked()), this, SLOT(DoImageProcessing()));
  connect(m_Controls.addPushButton, SIGNAL(clicked()), this, SLOT(OnAddPushButtonClicked()));
  connect(m_Controls.removePushButton, SIGNAL(clicked()), this, SLOT(OnRemovePushButtonClicked()));
  connect(m_Controls.removeAllPushButton, SIGNAL(clicked()), this, SLOT(OnRemoveAllPushButtonClicked()));
}

void RWSegmentation::OnAddPushButtonClicked()
{
  QList<mitk::DataNode::Pointer> nodes = this->GetDataManagerSelection();
  if (nodes.empty())
    return;

  for (int i = 0; i != nodes.size() ; ++i)
  {
    if (  nodes[i]->GetData()->GetClassHierarchy()[0] == "Image" || 
          nodes[i]->GetData()->GetClassHierarchy()[0] == "LabelSetImage" )
    {
      QString nodeName = QString::fromStdString( nodes[i]->GetName() );
      m_Controls.priorListWidget->addItem( nodeName );
    }
  }
}

void RWSegmentation::OnRemovePushButtonClicked()
{
  m_Controls.priorListWidget->takeItem( m_Controls.priorListWidget->currentRow() );
}

void RWSegmentation::OnRemoveAllPushButtonClicked()
{
  m_Controls.priorListWidget->clear();
}

void RWSegmentation::DoImageProcessing()
{
  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

  mitk::Image::Pointer inputImage = dynamic_cast<mitk::Image*>(m_Controls.inputImageComboBox->GetSelectedNode()->GetData());

  InputImageType::Pointer itkInputImage = InputImageType::New();
  mitk::CastToItkImage(inputImage, itkInputImage);

  bool succeed = this->getPrior();

  if (!succeed)
  {
    QApplication::restoreOverrideCursor();
    MITK_INFO << "Priors are not of the same size";
    return;
  }

  typedef  itk::RWSegmentationFilter< InputImageType , LabelImageType > RWFilterType;
  RWFilterType::Pointer RWFilter = RWFilterType::New();
  RWFilter->SetInput( itkInputImage );
  RWFilter->SetLabelImage( m_Prior );
  RWFilter->SetBeta( m_Controls.betaSpinBox->value() );
  RWFilter->SetWriteBackground( false );
  RWFilter->Update();

  mitk::Image::Pointer outputImage = mitk::Image::New();
  mitk::CastToMitkImage(RWFilter->GetOutput(), outputImage);

  mitk::DataNode::Pointer node = mitk::DataNode::New();
  node->SetName("Segmentation" + std::to_string(m_numSegs++) );
  node->SetData(outputImage);

  m_DataStorage->Add(node, m_Controls.inputImageComboBox->GetSelectedNode());

  QApplication::restoreOverrideCursor();
}

// Non-slot
bool RWSegmentation::getPrior()
{
  QString nodeName = m_Controls.priorListWidget->item(0)->text();
  mitk::DataNode::Pointer node = m_DataStorage->GetNamedNode(nodeName.toStdString());
  mitk::Image::Pointer referenceImage = dynamic_cast<mitk::Image *>(node->GetData());

  LabelImageType::Pointer ref = LabelImageType::New();
  mitk::CastToItkImage(referenceImage, ref);

  typedef itk::ImageDuplicator< LabelImageType > DuplicatorType;
  DuplicatorType::Pointer duplicator = DuplicatorType::New();
  duplicator->SetInputImage(ref);
  duplicator->Update();
  m_Prior = duplicator->GetOutput();

  LabelImageType::RegionType region = m_Prior->GetLargestPossibleRegion();

  typedef itk::ImageRegionIterator< LabelImageType > IteratorImageType;
  IteratorImageType it( m_Prior , region );

  unsigned char constant = 2;

  int numberOfImages = m_Controls.priorListWidget->count();

  for (int i = 1; i != numberOfImages; ++i)
  {
    QString nodeName1 = m_Controls.priorListWidget->item(i)->text();
    mitk::DataNode::Pointer node1 = m_DataStorage->GetNamedNode(nodeName1.toStdString());
    mitk::Image::Pointer image1 = dynamic_cast<mitk::Image *>(node1->GetData());

    LabelImageType::Pointer priorImage1 = LabelImageType::New();
    mitk::CastToItkImage(image1, priorImage1);

    LabelImageType::RegionType region1 = priorImage1->GetLargestPossibleRegion();

    if (region != region1)
      return false;

    typedef itk::ImageRegionConstIterator< LabelImageType > ConstIteratorImageType;
    ConstIteratorImageType it1( priorImage1 , region );

    it.GoToBegin();
    it1.GoToBegin();
    while( !it.IsAtEnd() )
    {
        if (it.Get() == 0 && it1.Get() != 0)
            it.Set( constant );
        ++it;
        ++it1;
    }
    ++constant;      
  }

  return true;
}

