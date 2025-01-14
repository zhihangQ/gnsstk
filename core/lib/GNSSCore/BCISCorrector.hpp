//==============================================================================
//
//  This file is part of GNSSTk, the ARL:UT GNSS Toolkit.
//
//  The GNSSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 3.0 of the License, or
//  any later version.
//
//  The GNSSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GNSSTk; if not, write to the Free Software Foundation,
//  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
//
//  This software was developed by Applied Research Laboratories at the
//  University of Texas at Austin.
//  Copyright 2004-2022, The Board of Regents of The University of Texas System
//
//==============================================================================

//==============================================================================
//
//  This software was developed by Applied Research Laboratories at the
//  University of Texas at Austin, under contract to an agency or agencies
//  within the U.S. Department of Defense. The U.S. Government retains all
//  rights to use, duplicate, distribute, disclose, or release this software.
//
//  Pursuant to DoD Directive 523024
//
//  DISTRIBUTION STATEMENT A: This software has been approved for public
//                            release, distribution is unlimited.
//
//==============================================================================


#ifndef GNSSTK_BCISCCORRECTOR_HPP
#define GNSSTK_BCISCCORRECTOR_HPP

#include "GroupPathCorrector.hpp"
#include "NavLibrary.hpp"

namespace gnsstk
{
      /// @ingroup GNSSsolutions
      //@{

      /** Compute group path correction due to inter-signal bias on
       * the satellite, using information available from the broadcast
       * navigation message.
       *
       * @attention The #navLib variable must be set to a valid
       * object for the getCorr() methods to return successfully. */
   class BCISCorrector : public GroupPathCorrector
   {
   public:
         /** Set the #corrType to ISC for GroupPathCorr.
          * @param[in] nl The NavLibrary to use to obtain ISC data. */
      BCISCorrector(NavLibrary& nl);
         /// @copydoc GroupPathCorrector::getCorr(const Position&, const Position&, const SatID&, const ObsID&, const CommonTime&, NavType, double&)
      bool getCorr(const Position& rxPos, const Position& svPos,
                   const SatID& sat, const ObsID& obs,
                   const CommonTime& when, NavType nav,
                   double& corrOut) override;
         /// @copydoc GroupPathCorrector::getCorr(const Position&, const Position&, const SatID&, const ObsID&, const CommonTime&, NavType, double&)
      bool getCorr(const Position& rxPos, const Xvt& svPos,
                   const SatID& sat, const ObsID& obs,
                   const CommonTime& when, NavType nav,
                   double& corrOut) override;

         /// Reference to the nav library from which we will get ISC data.
      NavLibrary& navLib;
   }; // class BCISCorrector

      //@}

} // namespace gnsstk

#endif // GNSSTK_BCISCCORRECTOR_HPP
