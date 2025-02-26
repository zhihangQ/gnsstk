/** @warning This code is automatically generated.
 *
 *  DO NOT EDIT THIS CODE BY HAND.
 *
 *  Refer to the documenation in the toolkit_docs gitlab project.
 */

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

#ifndef GNSSTK_NAVTYPE_HPP
#define GNSSTK_NAVTYPE_HPP

#include <string>
#include "EnumIterator.hpp"

namespace gnsstk
{
      /// @ingroup GNSSEph
      //@{

      /// Supported navigation types
   enum class NavType
   {
      Unknown,   ///< Uninitialized value
      Any,       ///< Used to match any nav code
      GPSLNAV,
      GPSCNAVL2,
      GPSCNAVL5,
      GPSCNAV2,
      GPSMNAV,
      BeiDou_D1,
      BeiDou_D2,
      GloCivilF,
      GloCivilC,
      GalFNAV,
      GalINAV,
      IRNSS_SPS,
      Last,      ///< Used to verify that all items are described at compile time
   }; // enum class NavType

      /** Define an iterator so C++11 can do things like
       * for (NavType i : NavTypeIterator()) */
   typedef EnumIterator<NavType, NavType::Unknown, NavType::Last> NavTypeIterator;

   namespace StringUtils
   {
         /// Convert a NavType to a whitespace-free string name.
      std::string asString(NavType e) noexcept;
         /// Convert a string name to an NavType
      NavType asNavType(const std::string& s) noexcept;
   }
      /** Translate nav type enumeration to its string representation.
       * @note The string representation is being used in file
       *   formats, e.g. RawNavCSVHeader.  The string values should
       *   not be changed if at all possible, as that would break
       *   the ability to read older files.
       * @note Any new nav codes should not contain spaces in the
       *   string values.
       * @param[in] s The nav type to get the string name of.
       * @return A space-free string containing the name of the nav code.
       */
   inline std::string convertNavTypeToString(NavType e)
   { return StringUtils::asString(e); }

      /** Translate nav type names as strings into enumeration
       * equivalents.
       * @see convertNavTypeToString
       * @param[in] s The nav type name to convert to enumeration.
       * @return An enumeration equivalent of the given string.
       *   Unknown is returned for any names that do not
       *   exactly match known values.
       */
   inline NavType convertStringToNavType(const std::string& s)
   { return StringUtils::asNavType(s); }

} // namespace gnsstk

#endif // GNSSTK_NAVTYPE_HPP
