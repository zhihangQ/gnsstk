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

/**
 * @file ObsRngDev.hpp
 * Observed range deviation computation & storage.
 */

#include <typeinfo>

#include "EphemerisRange.hpp"
#include "EngEphemeris.hpp"
#include "MiscMath.hpp"
#include "GNSSconstants.hpp"
#include "TimeString.hpp"
#include "ObsRngDev.hpp"

namespace gnsstk
{

   bool ObsRngDev::debug=false;

   ObsRngDev::ObsRngDev(
      const double prange,
      const SatID& svid,
      const CommonTime& time,
      const Position& rxpos,
      NavLibrary& navLib,
      EllipsoidModel& em,
      bool svTime,
      NavSearchOrder order,
      SVHealth xmitHealth,
      NavValidityType valid)
         : obstime(time), svid(svid), ord(0), wonky(false)
   {
      computeOrd(prange, rxpos, navLib, em, svTime, order, xmitHealth, valid);
      Position gx(rxpos);
      gx.asGeodetic(&em);
      NBTropModel nb(gx.getAltitude(),
                     gx.getGeodeticLatitude(),
                     static_cast<YDSTime>(time).doy);
      computeTrop(nb);
   }

   ObsRngDev::ObsRngDev(
      const double prange,
      const SatID& svid,
      const CommonTime& time,
      const Position& rxpos,
      NavLibrary& navLib,
      EllipsoidModel& em,
      const IonoModelStore& ion,
      CarrierBand band,
      bool svTime,
      NavSearchOrder order,
      SVHealth xmitHealth,
      NavValidityType valid)
         : obstime(time), svid(svid), ord(0), wonky(false)
   {
      computeOrd(prange, rxpos, navLib, em, svTime, order, xmitHealth, valid);
      Position gx(rxpos);
      gx.asGeodetic(&em);
      NBTropModel nb(gx.getAltitude(),
                     gx.getGeodeticLatitude(),
                     static_cast<YDSTime>(time).doy);
      computeTrop(nb);
      iono = ion.getCorrection(time, gx, elevation, azimuth, band);
      ord -= iono;
   }

   ObsRngDev::ObsRngDev(
      const double prange,
      const SatID& svid,
      const CommonTime& time,
      const Position& rxpos,
      NavLibrary& navLib,
      EllipsoidModel& em,
      const TropModel& tm,
      bool svTime,
      NavSearchOrder order,
      SVHealth xmitHealth,
      NavValidityType valid)
         : obstime(time), svid(svid), ord(0), wonky(false)
   {
      computeOrd(prange, rxpos, navLib, em, svTime, order, xmitHealth, valid);
      computeTrop(tm);
   }

   ObsRngDev::ObsRngDev(
      const double prange,
      const SatID& svid,
      const CommonTime& time,
      const Position& rxpos,
      NavLibrary& navLib,
      EllipsoidModel& em,
      const TropModel& tm,
      const IonoModelStore& ion,
      CarrierBand band,
      bool svTime,
      NavSearchOrder order,
      SVHealth xmitHealth,
      NavValidityType valid)
         : obstime(time), svid(svid), ord(0), wonky(false)
   {
      computeOrd(prange, rxpos, navLib, em, svTime, order, xmitHealth, valid);
      computeTrop(tm);
      Position gx(rxpos);
      gx.asGeodetic(&em);
      iono = ion.getCorrection(time, gx, elevation, azimuth, band);
      ord -= iono;
   }


   ObsRngDev::ObsRngDev(
      const double prange1,
      const double prange2,
      const SatID& svid,
      const CommonTime& time,
      const Position& rxpos,
      NavLibrary& navLib,
      EllipsoidModel& em,
      bool svTime,
      double gamma,
      NavSearchOrder order,
      SVHealth xmitHealth,
      NavValidityType valid)
         : obstime(time), svid(svid), ord(0), wonky(false)
   {
      // for dual-frequency see IS-GPS-200, section 20.3.3.3.3.3
      double icpr = (prange2 - gamma * prange1)/(1-gamma);
      iono = prange1 - icpr;

      computeOrd(icpr, rxpos, navLib, em, svTime, order, xmitHealth, valid);
      Position gx(rxpos);
      gx.asGeodetic(&em);
      NBTropModel nb(gx.getAltitude(),
                     gx.getGeodeticLatitude(),
                     static_cast<YDSTime>(time).doy);
      computeTrop(nb);
   }


   ObsRngDev::ObsRngDev(
      const double prange1,
      const double prange2,
      const SatID& svid,
      const CommonTime& time,
      const Position& rxpos,
      NavLibrary& navLib,
      const EllipsoidModel& em,
      const TropModel& tm,
      bool svTime,
      double gamma,
      NavSearchOrder order,
      SVHealth xmitHealth,
      NavValidityType valid)
         : obstime(time), svid(svid), ord(0), wonky(false)
   {
      // for dual-frequency see IS-GPS-200, section 20.3.3.3.3.3
      double icpr = (prange2 - gamma * prange1)/(1-gamma);
      iono = prange1 - icpr;

      computeOrd(icpr, rxpos, navLib, em, svTime, order, xmitHealth, valid);
      computeTrop(tm);
   }

   // This should be used for obs in GPS (i.e. receiver) time.
   void ObsRngDev::computeOrdRx(
      const double obs,
      const Position& rxpos,
      NavLibrary& navLib,
      const EllipsoidModel& em,
      NavSearchOrder order,
      SVHealth xmitHealth,
      NavValidityType valid)
   {
      CorrectedEphemerisRange cer;
      rho = cer.ComputeAtTransmitTime(obstime,obs, rxpos, svid, navLib, order,
                                      xmitHealth, valid);
      azimuth = cer.azimuth;
      elevation = cer.elevation;
      ord = obs - rho;
      iodc = cer.iodc;
      health = cer.health;

      if (debug)
      {
         std::ios::fmtflags oldFlags = std::cout.flags();
         std::cout << *this << std::endl
                   << std::setprecision(3) << std::fixed
                   << "  obs=" << obs
                   << ", rho=" << (double)rho
                   << ", obs-rho=" << (double)ord
                   << std::endl
                   << "  rx.x=" << rxpos
                   << std::setprecision(4) << std::scientific
                   << ", sv bias=" << cer.svclkbias
                   << ", sv drift=" << cer.svclkdrift
                   << std::endl;
         std::cout.flags(oldFlags);
      }
   }


// This should be used for obs tagged in SV time
   void ObsRngDev::computeOrdTx(
      double obs,
      const Position& rxpos,
      NavLibrary& navLib,
      const EllipsoidModel& em,
      NavSearchOrder order,
      SVHealth xmitHealth,
      NavValidityType valid)
   {
      CorrectedEphemerisRange cer;
      rho = cer.ComputeAtTransmitSvTime(obstime, obs, rxpos, svid, navLib,
                                        order, xmitHealth, valid);
      azimuth = cer.azimuth;
      elevation = cer.elevation;
      ord = obs - rho;
      if (debug)
      {
         std::ios::fmtflags oldFlags = std::cout.flags();
         std::cout << *this << std::endl
                   << std::setprecision(3) << std::fixed
                   << "  obs=" << obs
                   << ", rho=" << (double)rho
                   << ", obs-rho=" << (double)ord
                   << std::endl
                   << std::setprecision(3)
                   << "  sv.x=" << cer.svPosVel.x
                   << ", sv.v=" << cer.svPosVel.v
                   << std::endl
                   << "  rx.x=" << rxpos
                   << std::setprecision(4) << std::scientific
                   << ", sv bias=" << cer.svPosVel.clkbias
                   << ", sv drift=" << cer.svPosVel.clkdrift
                   << std::endl;
         std::cout.flags(oldFlags);
      }
   }

   void ObsRngDev::computeTrop(const TropModel& tm)
   {
      trop = tm.correction(elevation);
      ord -= trop;
   }

   std::ostream& operator<<(std::ostream& s, const ObsRngDev& ord)
      noexcept
   {
      std::ios::fmtflags oldFlags = s.flags();
      s << "t=" << printTime(ord.obstime,"%Y/%03j %02H:%02M:%04.1f")
        << " prn=" << std::setw(2) << ord.svid.id
        << std::setprecision(4)
        << " az=" << std::setw(3) << ord.azimuth
        << " el=" << std::setw(2) << ord.elevation
        << std::hex
        << " h=" << std::setw(1) << ord.health
        << std::dec << std::setprecision(4)
        << " ord=" << ord.ord
        << " ion=" << ord.iono
        << " trop=" << ord.trop
        << std::hex
        << " iodc=" << ord.iodc
        << " wonky=" << ord.wonky;
      s.flags(oldFlags);
      return s;
   }
}
