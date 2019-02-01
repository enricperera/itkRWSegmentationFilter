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


#ifndef RWSegmentation_h
#define RWSegmentation_h

#include <berryISelectionListener.h>

#include <QmitkAbstractView.h>

#include "ui_RWSegmentationControls.h"

#include "itkImage.h"


/**
  \brief RWSegmentation

  \warning  This class is not yet documented. Use "git blame" and ask the author to provide basic documentation.

  \sa QmitkAbstractView
  \ingroup ${plugin_target}_internal
*/

  // ITK types
  typedef     float               InputPixelType;
  const       int                 Dimension = 3;
  typedef     itk::Image< InputPixelType, Dimension >  InputImageType;

  typedef     unsigned char       LabelPixelType;
  typedef     itk::Image< LabelPixelType , Dimension >  LabelImageType;


class RWSegmentation : public QmitkAbstractView
{
  // this is needed for all Qt objects that should have a Qt meta-object
  // (everything that derives from QObject and wants to have signal/slots)
  Q_OBJECT

public:
  static const std::string VIEW_ID;

protected slots:

  /// \brief Called when the user clicks the GUI button
  void DoImageProcessing();
  void OnAddPushButtonClicked();
  void OnRemovePushButtonClicked();
  void OnRemoveAllPushButtonClicked();

protected:

  virtual void CreateQtPartControl(QWidget *parent) override;

  virtual void SetFocus() override;
  bool getPrior();

  mitk::DataStorage::Pointer m_DataStorage;

  Ui::RWSegmentationControls m_Controls;

  LabelImageType::Pointer m_Prior;
  int m_numSegs = 0;
};

#endif // RWSegmentation_h
