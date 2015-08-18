// -*- mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
// vi: set et ts=4 sw=4 sts=4:
/*
  Copyright (C) 2009-2013 by Andreas Lauser

  This file is part of the Open Porous Media project (OPM).

  OPM is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  OPM is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OPM.  If not, see <http://www.gnu.org/licenses/>.
*/
/*!
 * \file
 * \copydoc Opm::PiecewiseLinearTwoPhaseMaterialParams
 */
#ifndef OPM_PIECEWISE_LINEAR_TWO_PHASE_MATERIAL_PARAMS_HPP
#define OPM_PIECEWISE_LINEAR_TWO_PHASE_MATERIAL_PARAMS_HPP

#include <vector>

#include <cassert>

namespace Opm {
/*!
 * \ingroup FluidMatrixInteractions
 *
 * \brief Specification of the material parameters for a two-phase material law which
 *        uses a table and piecewise constant interpolation.
 */
template<class TraitsT>
class PiecewiseLinearTwoPhaseMaterialParams
{
    typedef typename TraitsT::Scalar Scalar;

public:
    typedef std::vector<Scalar> ValueVector;

    typedef TraitsT Traits;

    PiecewiseLinearTwoPhaseMaterialParams()
    {
#ifndef NDEBUG
        finalized_ = false;
#endif
    }

    /*!
     * \brief Calculate all dependent quantities once the independent
     *        quantities of the parameter object have been set.
     */
    void finalize()
    {
#ifndef NDEBUG
        finalized_ = true;
#endif

        // revert the order of the sampling points if they were given
        // in reverse direction.
        if (SwPcwnSamples_.front() > SwPcwnSamples_.back())
            swapOrder_(SwPcwnSamples_, pcwnSamples_);

        if (SwKrwSamples_.front() > SwKrwSamples_.back())
            swapOrder_(SwKrwSamples_, krwSamples_);


        if (SwKrnSamples_.front() > SwKrnSamples_.back())
            swapOrder_(SwKrnSamples_, krnSamples_);

    }

    /*!
     * \brief Return the wetting-phase saturation values of all sampling points.
     */
    const ValueVector& SwKrwSamples() const
    { assertFinalized_(); return SwKrwSamples_; }

    /*!
     * \brief Return the wetting-phase saturation values of all sampling points.
     */
    const ValueVector& SwKrnSamples() const
    { assertFinalized_(); return SwKrnSamples_; }

    /*!
     * \brief Return the wetting-phase saturation values of all sampling points.
     */
    const ValueVector& SwPcwnSamples() const
    { assertFinalized_(); return SwPcwnSamples_; }

    /*!
     * \brief Return the sampling points for the capillary pressure curve.
     *
     * This curve is assumed to depend on the wetting phase saturation
     */
    const ValueVector& pcnwSamples() const
    { assertFinalized_(); return pcwnSamples_; }

    /*!
     * \brief Set the sampling points for the capillary pressure curve.
     *
     * This curve is assumed to depend on the wetting phase saturation
     */
    template <class Container>
    void setPcnwSamples(const Container& SwValues, const Container& values)
    {
        assert(SwValues.size() == values.size());

        int n = SwValues.size();
        SwPcwnSamples_.resize(n);
        pcwnSamples_.resize(n);

        std::copy(SwValues.begin(), SwValues.end(), SwPcwnSamples_.begin());
        std::copy(values.begin(), values.end(), pcwnSamples_.begin());
    }

    /*!
     * \brief Return the sampling points for the relative permeability
     *        curve of the wetting phase.
     *
     * This curve is assumed to depend on the wetting phase saturation
     */
    const ValueVector& krwSamples() const
    { assertFinalized_(); return krwSamples_; }

    /*!
     * \brief Set the sampling points for the relative permeability
     *        curve of the wetting phase.
     *
     * This curve is assumed to depend on the wetting phase saturation
     */
    template <class Container>
    void setKrwSamples(const Container& SwValues, const Container& values)
    {
        assert(SwValues.size() == values.size());

        int n = SwValues.size();
        SwKrwSamples_.resize(n);
        krwSamples_.resize(n);

        std::copy(SwValues.begin(), SwValues.end(), SwKrwSamples_.begin());
        std::copy(values.begin(), values.end(), krwSamples_.begin());
    }

    /*!
     * \brief Return the sampling points for the relative permeability
     *        curve of the non-wetting phase.
     *
     * This curve is assumed to depend on the wetting phase saturation
     */
    const ValueVector& krnSamples() const
    { assertFinalized_(); return krnSamples_; }

    /*!
     * \brief Set the sampling points for the relative permeability
     *        curve of the non-wetting phase.
     *
     * This curve is assumed to depend on the wetting phase saturation
     */
    template <class Container>
    void setKrnSamples(const Container& SwValues, const Container& values)
    {
        assert(SwValues.size() == values.size());

        int n = SwValues.size();
        SwKrnSamples_.resize(n);
        krnSamples_.resize(n);

        std::copy(SwValues.begin(), SwValues.end(), SwKrnSamples_.begin());
        std::copy(values.begin(), values.end(), krnSamples_.begin());
    }

private:
#ifndef NDEBUG
    void assertFinalized_() const
    { assert(finalized_); }

    bool finalized_;
#else
    void assertFinalized_() const
    { }
#endif

    void swapOrder_(ValueVector& swValues, ValueVector& values) const
    {
        if (swValues.front() > values.back()) {
            for (unsigned origSampleIdx = 0;
                 origSampleIdx < swValues.size() / 2;
                 ++ origSampleIdx)
            {
                unsigned newSampleIdx = swValues.size() - origSampleIdx - 1;

                std::swap(swValues[origSampleIdx], swValues[newSampleIdx]);
                std::swap(values[origSampleIdx], values[newSampleIdx]);
            }
        }
    }

    ValueVector SwPcwnSamples_;
    ValueVector SwKrwSamples_;
    ValueVector SwKrnSamples_;
    ValueVector pcwnSamples_;
    ValueVector krwSamples_;
    ValueVector krnSamples_;
};
} // namespace Opm

#endif
