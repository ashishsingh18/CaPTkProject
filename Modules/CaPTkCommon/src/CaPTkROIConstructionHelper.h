#ifndef CaPTkROIConstructionHelperBase_h
#define CaPTkROIConstructionHelperBase_h

// The following header file is generated by CMake and thus it's located in
// the build directory. It provides an export macro for classes and functions
// that you want to be part of the public interface of your module.
#include <MitkCaPTkCommonExports.h>

#include <mitkLabelSetImage.h>
#include <mitkLabelSet.h>

#include "itkNeighborhoodIterator.h"

#include <string>
#include <vector>

#include "CaPTkROIConstructionHelperBase.h"
#include "CaPTkROIConstructionCreateLatticePoints.h"

namespace captk
{
/** \class ROIConstructionHelper
 * \brief Templated class to create the lattice ROIs
 */
template <typename TPixel, unsigned int VImageDimension>
class MITKCAPTKCOMMON_EXPORT ROIConstructionHelper : public ROIConstructionHelperBase
{
public:

    using ImageType        = itk::Image<TPixel, VImageDimension>;
    using ImageTypePointer = typename ImageType::Pointer;
    using IndexType        = typename ImageType::IndexType;
    using SizeType         = typename ImageType::SizeType;

    ROIConstructionHelper(ImageTypePointer mask)
    {
        m_Mask = mask;
    }

    ~ROIConstructionHelperBase()
    {

    }

    void Update(
        float radius,
        float step) override
    {
        m_Indeces = captk::ROIConstructionCreateLatticePoints(m_Mask, step);
        m_CurrentIndex = 0;

        // Radius for the neighborhood iterator
        for (int i = 0; i < ImageType::ImageDimension; i++)
        {
            if (i + 1 > 3)
            {
                m_RadiusSize[i] = 1;
            }
            else
            {
                m_RadiusSize[i] = radius; // TODO: radius IN MILLIMETERS (Spacing?)
            }
        }

        // Initialize the iterator once so it doesn't get recreated all the time
        m_Iter  = itk::ImageRegionIteratorWithIndex<ImageType>(mask, mask->GetRequestedRegion());
    }

    bool HasNext() override
    {
        return ( m_CurrentIndex + 1 < m_Indeces.size() );
    }

    // void SetValuesAndNames(mitk::LabelSet::Pointer labelSet) override
    // {
    //     mitk::LabelSet::LabelContainerConstIteratorType it;
    //     for (it = labelSet->IteratorConstBegin();
    //          it != labelSet->IteratorConstEnd();
    //          ++it)
    //     {
    //         if (it->second->GetValue() != 0)
    //         {
    //             // std::cout << "Found label set name: "
    //             //           << it->second->GetName() << "\n";
    //             m_Values.push_back(it->second->GetValue());
    //             m_Names.push_back(it->second->GetName());
    //         }
    //     }
    // }

    /** \brief Populate empty mask with the next lattice ROI patch
     * \param rMask an empty but initialized LabelSetImage
     * 
     * \return weight (what percentage of voxels of the patch were used)
     */
    float PopulateMask(mitk::LabelSetImage::Pointer rMask) override
    {
        // Convert mitk::LabelSetImage::Pointer to ImageType
        using MitkToItkImageConverterType = mitk::ImageToItk<ImageType>;
        typename MitkToItkImageConverterType::Pointer mitktoitk = MitkToItkImageConverterType::New();
        mitktoitk->SetInput(rMask);
        mitktoitk->Update();
        ImageTypePointer rMaskItk = mitktoitk->GetOutput();

        // Iterate through the neighborhood until you find the current lattice index
        auto ind = m_Indeces[m_CurrentIndex];
        itk::NeighborhoodIterator<ImageType> niter(m_RadiusSize, rMaskItk,
                                                   rMaskItk->GetRequestedRegion());
        for (niter.GoToBegin(); !niter.IsAtEnd(); ++niter)
        {
            // Check if the index is the same as the one we want
            if (niter.GetIndex() == ind)
            {
                // Iterate through the neighborhood
                for (unsigned int i = 0; i < niter.Size(); ++i)
                {
                    // Check if that pixel is enabled
                    m_Iter.SetIndex(niter.GetIndex(i));
                    niter.SetPixel(i, 1);
                }
            }
        }
    }

protected:
    void OnIncrement() override
    {
        m_CurrentIndex++;
    }

private:
    // std::vector<int>         m_Values;
    // std::vector<std::string> m_Names;
    size_t                   m_CurrentIndex;
    ImageTypePointer        m_Mask;
    std::vector<IndexType>  m_Indeces;
    typename ImageType::SizeType m_RadiusSize;
    itk::ImageRegionIteratorWithIndex <ImageType> m_Iter;
};
} // namespace captk

#endif // ! CaPTkROIConstructionHelperBase_h