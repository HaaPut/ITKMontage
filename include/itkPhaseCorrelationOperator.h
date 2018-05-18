/*=========================================================================
 *
 *  Copyright Insight Software Consortium
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
#ifndef itkPhaseCorrelationOperator_h
#define itkPhaseCorrelationOperator_h

#include "itkImageToImageFilter.h"
#include <complex>

namespace itk
{

/** \class PhaseCorrelationOperator
 *  \brief Computes the spectrum ratio in phase correlation method.
 *
 *  The class is templated over the type of the real-valued pixel it will be
 *  operating on and the image dimension.
 *
 *  The two input spectrums may have different size, while their real size is
 *  the same. To subsample them to same resolution, high frequencies must be
 *  excluded.
 *
 *  This frequency ratio is computed at every index of output correlation
 *  surface.
 *
 *  This class provides interface for further techniques to improve the
 *  registration performance. Method AdjustOutputInformation() enables for
 *  example to limit the computation only to low frequencies.
 *
 * \author Jakub Bican, jakub.bican@matfyz.cz, Department of Image Processing,
 *         Institute of Information Theory and Automation,
 *         Academy of Sciences of the Czech Republic.
 *
 * \ingroup Montage
 */
template < typename TRealPixel, unsigned int VImageDimension >
class ITK_TEMPLATE_EXPORT PhaseCorrelationOperator :
  public ImageToImageFilter<
      Image< std::complex< TRealPixel >, VImageDimension >,
      Image< std::complex< TRealPixel >, VImageDimension > >
{

public:
  ITK_DISALLOW_COPY_AND_ASSIGN(PhaseCorrelationOperator);

  using Self = PhaseCorrelationOperator;
  using Superclass = ImageToImageFilter<Image< std::complex< TRealPixel >, VImageDimension >, Image< std::complex< TRealPixel >, VImageDimension > >;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(PhaseCorrelationOperator, ImageToImageFilter);

  /** ImageDimension enumeration. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      VImageDimension);

  /** Image type aliases. */
  using PixelType = TRealPixel;
  using ComplexType = std::complex<PixelType>;
  using ImageType = Image< ComplexType, ImageDimension >;
  using ImagePointer = typename ImageType::Pointer;
  using ImageConstPointer = typename ImageType::ConstPointer;
  using OutputImageRegionType = typename Superclass::OutputImageRegionType;

  /** Connect the fixed image. */
  void SetFixedImage( ImageType * fixedImage );

  /** Connect the moving image. */
  void SetMovingImage( ImageType * movingImage );

protected:
  PhaseCorrelationOperator();
  virtual ~PhaseCorrelationOperator() {};
  void PrintSelf(std::ostream& os, Indent indent) const override;

  /** PhaseCorrelationOperator produces an image which is a different
   * resolution and with a different pixel spacing than its input
   * images. */
  void GenerateOutputInformation() override;

  /** PhaseCorrelationOperator needs a larger input requested region than the
   *  output requested region. */
  void GenerateInputRequestedRegion() override;
  void EnlargeOutputRequestedRegion(DataObject *output) override;

  /** PhaseCorrelationOperator can be implemented as a multithreaded filter.
   *  This method performs the computation. */
  void DynamicThreadedGenerateData( const OutputImageRegionType& outputRegionForThread ) override;

  /** After the largest possible output data size is determined, this method
   * is called to additionally adjust the output parameters (reduce the size).
   *
   *  The method is called in GenerateOutputInformation() method, so the input
   *  spacing, index and size can be determined from the inputs 0 (fixed image)
   *  and 1 (moving image).
   *
   *  This method empty here and can be reimplemented by child filters.
   */
  virtual void AdjustOutputInformation(
                 typename ImageType::SpacingType & spacing,
                 typename ImageType::IndexType   & index,
                 typename ImageType::SizeType    & size      ) {};

  /** Override VerifyInputInformation() since this filter's inputs do not need
   * to occupy the same physical space.
   *
   * \sa ProcessObject::VerifyInputInformation
   */
  void VerifyInputInformation() override {}
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkPhaseCorrelationOperator.hxx"
#endif

#endif
